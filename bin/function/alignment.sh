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
        --no-mixed \
        2> $Log \
    | samtools view -uS - -o $B \
    || echo2 "Failed to perform bowtie2 alignment for $L and $R on $B" error
}
export -f PeBowtie2Alignment