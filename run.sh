#!/bin/bash

set -e -o pipefail

declare -xr PACKAGE_NAME="Zamore UMI Chip-Seq"

declare CalledProgramName=$0

# link coreutils programs in Mac before using them
if [[ $(uname -s) == 'Darwin' ]]; then
    if ! which greadlink &>/dev/null; then echo "Please install coreutils with brew" && exit 1; fi

    declare CalledProgramFullPath=$(greadlink -f $CalledProgramName)
    for program in readlink nproc awk; do
        if [[ ! -f $(dirname $CalledProgramFullPath)/bin/${program} ]]; then 
            declare macversion=g${program} # coreutils commands has 'g' prefix
            if ! which $macversion &>/dev/null; then 
                echo2 "Missing $macversion in your MacOS, please make sure you have followed our instructions to install coreutils with brew" error
            fi
            ln -s $(which $macversion) $(dirname "$CalledProgramFullPath")/bin/${program}
        fi
    done
    
else # Linux 
    declare CalledProgramFullPath=$(readlink -f $CalledProgramName)
fi

# varaibles
declare -rx PipelineDir=$(dirname "$CalledProgramFullPath")
declare -rx ThisBin="${PipelineDir}"/bin

# append to PATH; Note: don't make PATH readonly because user might wanna change it in config.sh
declare -x PATH="${ThisBin}":${PATH}
# load constant variables and
. "$ThisBin"/const.sh 
# load enviromental variables
# in order for this to be run by a docker image, the variables needs to be passed in at run time
. "${PipelineDir}"/configuration/config.sh

# load functions and other settings
. "$ThisBin"/color.sh
. "$ThisBin"/function.sh



# global usage
function usage {
    local _yellow=$(echo -ne ${FONT_COLOR_YELLOW})
    local _green=$(echo -ne ${FONT_COLOR_GREEN})
    local _cyan=$(echo -ne ${FONT_COLOR_CYAN})
    local _magenta=$(echo -ne ${FONT_COLOR_MAGENTA})
    
    cat << EOF

${EOFBOLD}${PACKAGE_NAME}${EOFRESET}

Pipeline for ChIP-Seq analysis used in the Zamore Lab.

Version: ${EOFUNDERLINE}$PROG_VERSION${EOFRESET}

It currently include the following modules:

${_cyan}[chip-seq]
    Run Chip-Seq analysis pipeline

${_green}[_]
    place holder

${_yellow}[_]
    place holder

${_magenta}${EOFRESET}    

EOF
}
# trigger usage
if [[ $# -lt 1 ]]; then usage && exit 1; fi

# invoke specific module
declare SubProgram=$(echo ${1} | tr '[A-Z]' '[a-z]')
case $SubProgram in
    chip|c)
        shift && bash "$ThisBin"/modules/chipseq.sh "$@";;
    version|v) 
        echo $PROG_VERSION;;
    *)
        echo2 "unrecognized option: ${SubProgram}" error;;
esac