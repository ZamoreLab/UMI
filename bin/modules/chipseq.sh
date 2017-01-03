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
declare -xi UMI_PADDING1=$DEFAULT_UMI_PADDING1

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
declare -x BigWigOurDir
declare -x LogDir

# required binaries, make sure they exist
declare -a RequiredPrgrams=( \
    readlink sed awk faSize \
    python macs2 \
    samtools bowtie2 \
)
for program in "${RequiredPrgrams[@]}"; do assertBinExists $program; done

# usage
usage () {
cat << EOF

This pipeline takes paired-end Illumina UMI libraries as input and perform Chip-Seq analysis.

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
    -N      Length of UMI on \1 and \2, default: $UMI_LEN1
    -P      Length of UMI padding on \1 and \2, default: $UMI_PADDING1

${FORDEBUG} [ Debug ]
    -D      Debug mode, default: off

${RESET}

EOF
}

# argument parse
while getopts "hl:r:L:R:g:b:o:c:N:P:D" OPTION; do
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
        N)
            UMI_LEN1=$OPTARG
        ;;
        P)
            UMI_PADDING1=$OPTARG
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
    UMI_LEN1 UMI_PADDING1 \
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
BigWigOurDir=$OutputDir/bigWigs && mkdir -p "$BigWigOurDir" || echo2 "Failed to create directory $BigWigOurDir" error 
LogDir=$OutputDir/log && mkdir -p "$LogDir" || echo2 "Failed to create directory $LogDir" error 

# aligning
declare Fq1
declare Fq2
declare Fq1Base
declare Fq2Base
declare Type
declare Bam
declare DedupBam
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
    if [[ ! -s $FqClippedOut1 ]]; then
        echo2 "Clipping UMI on ChIP-Seq Input $Fq1"
        clip_umi -i "$Fq1" -l $UMI_LEN1 -p $UMI_PADDING1 -o $FqClippedOut1 \
        || echo2 "Failed to run clip_umi on $Fq1"
    else
        echo2 "$FqClippedOut1 has been generated previously" warning 
    fi 

    if [[ ! -s $FqClippedOut2 ]]; then
        echo2 "Clipping UMI on ChIP-Seq Input $Fq2"
        clip_umi -i "$Fq2" -l $UMI_LEN1 -p $UMI_PADDING1 -o $FqClippedOut2 \
        || echo2 "Failed to run clip_umi on $Fq2"
    else
        echo2 "$FqClippedOut2 has been generated previously" warning 
    fi 

    Type=input
    Bam=$BamOutDir/${Type}.${i}.bam
    DedupBam=$BamOutDir/${Type}.${i}.dedup.bam
    Log=$LogDir/bowtie2.${Type}.${i}.log
    if [[ ! -s $Bam ]]; then 
        echo2 "Aligning input replicate $i to the genome $GenomeFa"
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

    if [[ ! -s $DedupBam ]]; then 
        echo2 "Removing duplicated UMI from $Bam"
        bam_dedup -i $Bam -o $DedupBam -l $UMI_LEN1 \
            &> $LogDir/bam_dedup.${Type}.${i}.log \
        && rm $Bam $FqClippedOut1 $FqClippedOut2 \
        || echo2 "Failed to run bam_dedup on $Bam" error 
    else 
        echo2 "Duplicating $Bam has been done previously" warning
    fi 
    InputBams+=( $DedupBam )

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
    if [[ ! -s $FqClippedOut1 ]]; then
        echo2 "Clipping UMI on ChIP-Seq IP $Fq1"
        clip_umi -i "$Fq1" -l $UMI_LEN1 -p $UMI_PADDING1 -o $FqClippedOut1 \
        || echo2 "Failed to run clip_umi on $Fq1"
    else
        echo2 "$FqClippedOut1 has been generated previously" warning 
    fi 

    if [[ ! -s $FqClippedOut2 ]]; then
        echo2 "Clipping UMI on ChIP-Seq IP $Fq2"
        clip_umi -i "$Fq2" -l $UMI_LEN1 -p $UMI_PADDING1 -o $FqClippedOut2 \
        || echo2 "Failed to run clip_umi on $Fq2"
    else
        echo2 "$FqClippedOut2 has been generated previously" warning 
    fi 

    Type=IP
    Bam=$BamOutDir/${Type}.${i}.bam
    DedupBam=$BamOutDir/${Type}.${i}.dedup.bam

    Log=$LogDir/bowtie2.${Type}.${i}.log
    if [[ ! -s $Bam ]]; then 
        echo2 "Aligning IP replicate $i to the genome $GenomeFa" 
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

    if [[ ! -s $DedupBam ]]; then 
        echo2 "Removing duplicated UMI from $Bam"
        bam_dedup -i $Bam -o $DedupBam -l $UMI_LEN1 \
            &> $LogDir/bam_dedup.${Type}.${i}.log \
        && rm $Bam $FqClippedOut1 $FqClippedOut2 \
        || echo2 "Failed to run bam_dedup on $Bam" error 
    else 
        echo2 "Duplicating $Bam has been done previously" warning
    fi 
    IpBams+=( $DedupBam )
done

# call peaks with macs2
if [[ ! -f $LogDir/macs2.callpeak.done ]]; then 
    echo2 "Calling peaks with MACS2"
    macs2 callpeak \
        -t ${IpBams[@]} \
        -c ${InputBams[@]} \
        -g $( faSize -detailed -tab ${GenomeFa} | awk '{a+=$2}END{print a}' ) \
        --outdir ${WorkingDir}/macs_out \
        -n ${RunName} \
        -B \
        --SPMR \
        --keep-dup all \
        &> $LogDir/macs2.callpeak.log \
    && touch $LogDir/macs2.callpeak.done \
    || echo2 "Failed to run macs2"
fi

for method in FE logLR; do 
    if [[ ! -f $LogDir/macs2.bdgcmp.${method}.done ]]; then 
        echo2 "Calculating ${method} and generate bigWig file"
        macs2 bdgcmp \
            -t ${WorkingDir}/macs_out/${RunName}_treat_pileup.bdg \
            -c ${WorkingDir}/macs_out/${RunName}_control_lambda.bdg \
            -m ${method} \
            -o ${WorkingDir}/macs_out/${RunName}.${method}.bedgraph \
            2> $LogDir/macs2.bdgcmp.${method}.log \
        && sort -k1,1 -k2,2n ${WorkingDir}/macs_out/${RunName}.${method}.bedgraph > ${WorkingDir}/macs_out/${RunName}.${method}.bedgraph.sorted \
        && bedGraphToBigWig \
            ${WorkingDir}/macs_out/${RunName}.${method}.bedgraph.sorted \
            <( faSize -detailed -tab ${GenomeFa} ) \
            ${BigWigOurDir}/${RunName}.${method}.bw \
        && rm ${WorkingDir}/macs_out/${RunName}.${method}.bedgraph.sorted \
        && touch $LogDir/macs2.bdgcmp.${method}.done \
        || echo2 "Failed to convert ${WorkingDir}/macs_out/${RunName}_${target}" error
    fi
done 