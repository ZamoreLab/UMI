# chip-seq pipeline

# global variables used in this module
declare -a InputFastq1
declare -a InputFastq2
declare -a IpFastq1
declare -a IpFastq2
declare -i NumOfRepeats
declare -x RunName
declare -x GenomeFa
declare -x BowtieIndex
declare -x OutputDir="$PWD"
declare -xi Threads=$DEFAULT_NUM_THREADS 
declare -xi UMI_LEN1=$DEFAULT_UMI_LEN1
declare -xi UMI_LEN2=$DEFAULT_UMI_LEN2
declare -xi UMI_PADDING1=$DEFAULT_UMI_PADDING1
declare -xi UMI_PADDING2=$DEFAULT_UMI_PADDING2

# overwrite it if it's higher than the number of cores available
if which nproc &>/dev/null; then 
    if [[ $(nproc) -lt $Threads ]]; then 
        Threads=$(nproc); 
    fi
fi

declare -x FigureOut
declare -x WorkingDir
declare -x FqOutDir
declare -x BamOutDir
declare -x LogDir

# required binaries, make sure they exist
declare -a RequiredPrgrams=( \
    readlink sed awk \
    python \
    samtools bowtie2 \
)
for program in "${RequiredPrgrams[@]}"; do assertBinExists $program; done

# usage
usage () {
cat << EOF

This pipeline takes paired-end Illumina run as input and perform Chip-Seq analysis.

Version: $PROG_VERSION

OPTIONS:
    -h      Show this message

${REQUIRED}[ required ]
    -l      Path to the \1 file of Chip-Seq input fastq file
    -r      Path to the \2 file of Chip-Seq input fastq file
    -L      Path to the \1 file of Chip-Seq IP fastq file
    -R      Path to the \2 file of Chip-Seq IP fastq file
            * You can provide multiple files for biological repleats, each with their own -l and -r tags, but the order has to match.
            
    -g      The genome sequence fasta file
    -b      The bowtie index prefix

${OPTIONAL}[ optional ]
    -o      Output directory, default: $OUTPUT_DIR
    -c      Number of cores/CPU to use, default: $Threads
    -M      Length of UMI on \1, default: $UMI_LEN1
    -N      Length of UMI on \2, default: $UMI_LEN2
    -P      Length of UMI padding on \1, default: $UMI_PADDING1
    -Q      Length of UMI padding on \2, default: $UMI_PADDING1

${FORDEBUG} [ Debug ]
    -D      Debug mode, default: off

${RESET}

EOF
}

# argument parse
while getopts "hl:r:L:R:g:b:o:c:M:N:P:Q:D" OPTION; do
    case $OPTION in
        h) # usage 
            usage && exit 0
        ;;
        l)
            declare fullpath=$(readlink -f $OPTARG)
            assertFileExists $fullpath
            InputFastq1+=($fullpath)
        ;;
        r)
            declare fullpath=$(readlink -f $OPTARG)
            assertFileExists $fullpath
            InputFastq2+=($fullpath)
        ;;
        L)
            declare fullpath=$(readlink -f $OPTARG)
            assertFileExists $fullpath
            IpFastq1+=($fullpath)
        ;;
        R)
            declare fullpath=$(readlink -f $OPTARG)
            assertFileExists $fullpath
            IpFastq2+=($fullpath)
        ;;
        g)
            GenomeFa=$(readlink -f $OPTARG)
            assertFileExists $GenomeFa
        ;;
        b)
            BowtieIndex=$(readlink -f $OPTARG)
            bowtie2Check $BowtieIndex
        ;;
        c) # number of cores 
            Threads=$OPTARG
        ;;
        o) # output directory
            OutputDir=$(readlink -f "$OPTARG")
        ;;
        M)
            UMI_LEN1=$OPTARG
        ;;
        N)
            UMI_LEN2=$OPTARG
        ;;
        P)
            UMI_PADDING1=$OPTARG
        ;;
        Q)
            UMI_PADDING2=$OPTARG
        ;;
        D) # turn on debug
            set -x
        ;;
        *) # unrecognized
            echo2 "unknown option" error 
        ;;
    esac
done

if [[ -z $InputFastq1 || -z $InputFastq2 ]]; then
    echo2 "Missing Chip-Seq input files, please provide them with -l and -r options" error
fi

if [[ -z $IpFastq1 || -z $IpFastq2 ]]; then
    echo2 "Missing Chip-Seq IP files, please provide them with -L and -R options" error
fi

if [[ ${#InputFastq1[@]} -ne ${#InputFastq2[@]} ]]; then 
    echo2 "the number of -l and -r do not match" error 
fi

if [[ ${#IpFastq1[@]} -ne ${#IpFastq2[@]} ]]; then 
    echo2 "the number of -L and -R do not match" error 
fi

declare -xi numOfRep=${#InputFastq1[@]}

# double-check variables beein defined
declare -a RequiredVariables=(\
    InputFastq1 InputFastq2 GenomeFa BowtieIndex \
    UMI_LEN1 UMI_LEN2 UMI_PADDING1 UMI_PADDING2 \
    Threads OutputDir \
)

for var in ${RequiredVariables[@]}; do 
    if [[ -z ${!var} ]]; then echo2 "$var is not set, please check usage" error; fi
done

RunName=$(echo -e "$(basename ${IpFastq1[0]})\n$(basename ${IpFastq2[0]})" | sed -e 'N;s/^\(.*\).*\n\1.*$/\1/') 
RunName=${RunName%_*}
if [[ -z $RunName ]]; then echo2 "LCP of -l and -r is empty" error; fi

# enter working directory
OutputDir="$OutputDir"/$RunName
mkdir -p "$OutputDir" || echo2 "Cannot create $OutputDir" error
assertDirWritable "${OutputDir}"
cd "${OutputDir}" || echo2 "Cannot create $OutputDir" error 
FigureOutDir=$OutputDir/figures && mkdir -p "$FigureOutDir" || echo2 "Failed to create directory $FigureOutDir" error
WorkingDir=$OutputDir/jobs && mkdir -p "$WorkingDir" || echo2 "Failed to create directory $WorkingDir" error
FqOutDir=$OutputDir/fastq && mkdir -p "$FqOutDir" || echo2 "Failed to create directory $FqOutDir" error 
BamOutDir=$OutputDir/bam && mkdir -p "$BamOutDir" || echo2 "Failed to create directory $BamOutDir" error 
LogDir=$OutputDir/log && mkdir -p "$LogDir" || echo2 "Failed to create directory $LogDir" error 

# aligning
declare Fq1
declare Fq2
declare Fq1Base
declare Fq2Base
declare Type
declare Bam
declare Log
declare -a InputBams
declare -a IpBams
for i in $(seq 0 $(( numOfRep - 1 )) ); do 
    Fq1=${InputFastq1[$i]}
    Fq1Base=$(basename $Fq1)
    Fq1Prefix=${Fq1Base%.f[aq]*}
    FqClippedOut1=$FqOutDir/${Fq1Prefix}.clipped.fq

    Fq2=${InputFastq2[$i]}
    Fq2Base=$(basename $Fq2)
    Fq2Prefix=${Fq2Base%.f[aq]*}
    FqClippedOut2=$FqOutDir/${Fq2Prefix}.clipped.fq

    # pre-process fq file
    echo2 "Clipping UMI on ChIP-Seq Input $Fq1"
    if [[ ! -s $FqClippedOut1 ]]; then
        clip_umi -i "$Fq1" -l $UMI_LEN1 -p $UMI_PADDING1 -o $FqClippedOut1 \
        || echo2 "Failed to run clip_umi on $Fq1"
    else
        echo2 "$FqClippedOut1 has been generated previously" warning 
    fi 

    echo2 "Clipping UMI on ChIP-Seq Input $Fq2"
    if [[ ! -s $FqClippedOut2 ]]; then
        clip_umi -i "$Fq2" -l $UMI_LEN2 -p $UMI_PADDING2 -o $FqClippedOut2 \
        || echo2 "Failed to run clip_umi on $Fq2"
    else
        echo2 "$FqClippedOut2 has been generated previously" warning 
    fi 

    Type=input
    Bam=$BamOutDir/${Type}.${i}.bam
    Log=$LogDir/bowtie2.${Type}.${i}.log
    echo2 "Aligning input replicate $i to the genome $GenomeFa"
    if [[ ! -s $Bam ]]; then 
        PeBowtie2Alignment \
            $FqClippedOut1 \
            $FqClippedOut2 \
            $BowtieIndex \
            $Bam \
            $Threads \
            $Log
    else
        echo2 "Alignment to generate $Bam has been done previously" warning 
    fi 
    InputBams+=( $Bam )

    # IP
    Fq1=${IpFastq1[$i]}
    Fq1Base=$(basename $Fq1)
    Fq1Prefix=${Fq1Base%.f[aq]*}
    FqClippedOut1=$FqOutDir/${Fq1Prefix}.clipped.fq

    Fq2=${IpFastq2[$i]}
    Fq2Base=$(basename $Fq2)
    Fq2Prefix=${Fq2Base%.f[aq]*}
    FqClippedOut2=$FqOutDir/${Fq2Prefix}.clipped.fq

    # pre-process fq file
    echo2 "Clipping UMI on ChIP-Seq IP $Fq1"
    if [[ ! -s $FqClippedOut1 ]]; then
        clip_umi -i "$Fq1" -l $UMI_LEN1 -p $UMI_PADDING1 -o $FqClippedOut1 \
        || echo2 "Failed to run clip_umi on $Fq1"
    else
        echo2 "$FqClippedOut1 has been generated previously" warning 
    fi 

    echo2 "Clipping UMI on ChIP-Seq IP $Fq2"
    if [[ ! -s $FqClippedOut2 ]]; then
        clip_umi -i "$Fq2" -l $UMI_LEN2 -p $UMI_PADDING2 -o $FqClippedOut2 \
        || echo2 "Failed to run clip_umi on $Fq2"
    else
        echo2 "$FqClippedOut2 has been generated previously" warning 
    fi 

    Type=IP
    Bam=$BamOutDir/${Type}.${i}.bam
    Log=$LogDir/bowtie2.${Type}.${i}.log
    echo2 "Aligning IP replicate $i to the genome $GenomeFa" 
    if [[ ! -s $Bam ]]; then 
        PeBowtie2Alignment \
            $FqClippedOut1 \
            $FqClippedOut2 \
            $BowtieIndex \
            $Bam \
            $Threads \
            $Log
    else 
        echo2 "Alignment to generate $Bam has been done previously" warning 
    fi 
    IpBams+=( $Bam )
done