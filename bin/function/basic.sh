
# print info with different color based on the second argument
function echo2 {
if [[ $# -eq 1 ]]; then
    echo -e $FONT_COLOR_GREEN"[$(date "+$ISO_8601")] ${1}${FONT_COLOR_RESET}"
else
    case $2 in
        warning) 
            echo -e $FONT_COLOR_MAGENTA"[$(date "+$ISO_8601")] Warning: $1${FONT_COLOR_RESET}";;
        error)   
            echo -e $FONT_STYLE_BOLD$FONT_COLOR_RED"[$(date "+$ISO_8601")] Error: $1${FONT_COLOR_RESET}" && exit 1;;
        *)       
            echo -e $FONT_COLOR_GREEN"[$(date "+$ISO_8601")] $1${FONT_COLOR_RESET}";;
    esac
fi
}
export -f echo2

# assert that if a binrary is in the PATH, exit the program if not
function assertBinExists {
    if ! which $1 &>/dev/null; then echo2 "Required program \"$1\" is not available" error; fi
}
export -f assertBinExists

# assert that a file exists, exit the program if not
function assertFileExists {
    if [[ ! -f "${1}" ]]; then echo2 "Required file \"${1}\" doesn't exist" error; fi
}
export -f assertFileExists

# assert that a directory exists, exit the program if not
function assertDirExists {
    if [[ ! -d "${1}" ]]; then echo2 "Required directory \"${1}\" doesn't exist" error; fi
}
export -f assertDirExists

# assert that a directory if writable, exit the program if not
function assertDirWritable {
    if [[ ! -w "$1" ]]; then echo2 "Directory \"$1\" is not writable" error; fi
}
export -f assertDirWritable

# return 0 if the file exists, return 1 otherwise
function fileExists {
    if [[ ! -f "$1" ]]; then return 1; else return 0; fi
}
export -f fileExists

# return 0 if the directory exists, return 1 otherwise
function dirExists {
    if [[ ! -d "$1" ]]; then return 1; else return 0; fi
}
export -f dirExists

# return 0 if all the components of the bowtie index exist, return 1 otherwise
function bowtieCheck {
    for sfx in "1.ebwt" "2.ebwt" "3.ebwt" "4.ebwt" "rev.1.ebwt" "rev.2.ebwt"; do
        if [[ ! -f ${1}.$sfx ]]; then return 1; fi
    done
    return 0
}
export -f bowtieCheck

# return 0 if all the components of the bowtie2 index exists, return 1 otherwise
function bowtie2Check {
    for sfx in "1.bt2" "2.bt2" "3.bt2" "4.bt2" "rev.1.bt2" "rev.2.bt2"; do
        if [[ ! -f "$1".$sfx ]]; then return 1; fi
    done
    return 0
}
export -f bowtie2Check
