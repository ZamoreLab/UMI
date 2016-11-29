function PeBowtie2Alignment {
    local L=$1
    local R=$2
    local I=$3
    local B=$4
    local T=$5
    local Log=$6
    bowtie2 \
        -1 $L \
        -2 $R \
        -x $I \
        -p $T \
        --very-sensitive \
        --no-discordant \
        --no-mixed \
        2> $Log \
    | samtools view -uS - -o $B \
    || echo2 "Failed to perform bowtie2 alignment for $L and $R on $B" error
}
export -f PeBowtie2Alignment

function bowtie2_se_log_parse {
    [[ $# -ne 2 ]] && exit 255
    local bt2="${1}"
    local output="${2}"
    [[ ! -s "$bt2" ]] && exit 1
    local total=$(head -1 "$bt2" | awk '{print $1}') 
    local align0=$(grep 'aligned 0 times' "$bt2" | awk '{print $1}')
    local align1=$(grep 'aligned exactly 1 time' "$bt2" | awk '{print $1}')
    local alignN=$(grep 'aligned >1 times' "$bt2" | awk '{print $1}')
    echo -e "${total}\t${align0}\t${align1}\t${alignN}" \
    | awk '{ \
        printf \
            "Unaligned\t%d\t%.3f\n" \
            "Aligned_once\t%d\t%.3f\n" \
            "Aligned_more_than_once\t%d\t%.3f\n", \
            $2, $2/($1 != 0? $1 : 1),\
            $3, $3/($1 != 0? $1 : 1),\
            $4, $4/($1 != 0? $1 : 1) \
        }' \
    >> "$output"
}
export -f bowtie2_se_log_parse

function bowtie2_pe_log_parse {
    [[ $# -ne 2 ]] && exit 255
    local bt2="${1}"
    local output="${2}"
    [[ ! -s "$bt2" ]] && exit 1
    local total=$(head -1 "$bt2" | awk '{print $1}') 
    local align0=$(grep 'aligned concordantly 0 times' "$bt2" | awk '{print $1}')
    local align1=$(grep 'aligned concordantly exactly 1 time' "$bt2" | awk '{print $1}')
    local alignN=$(grep 'aligned concordantly >1 times' "$bt2" | awk '{print $1}')
    echo -e "${total}\t${align0}\t${align1}\t${alignN}" \
    | awk '{ \
        printf \
            "Unaligned\t%d\t%.3f\n" \
            "Aligned_once\t%d\t%.3f\n" \
            "Aligned_more_than_once\t%d\t%.3f\n", \
            $2, $2/($1 != 0? $1 : 1),\
            $3, $3/($1 != 0? $1 : 1),\
            $4, $4/($1 != 0? $1 : 1) \
        }' \
    >> "$output"
}
export -f bowtie2_pe_log_parse