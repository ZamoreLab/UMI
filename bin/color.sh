declare -xr FONT_COLOR_RED="\033[31;40m";
declare -xr FONT_COLOR_GREEN="\033[32;40m";
declare -xr FONT_COLOR_YELLOW="\033[33;40m";
declare -xr FONT_COLOR_BLUE="\033[34;40m";
declare -xr FONT_COLOR_MAGENTA="\033[35;40m";
declare -xr FONT_COLOR_CYAN="\033[36;40m";
declare -xr FONT_COLOR_RESET="\033[0m";

declare -xr FONT_STYLE_BOLD='\033[1m';
declare -xr FONT_STYLE_UNDERLINE='\033[4m';
declare -xr FONT_STYLE_BLINK='\033[5m';
declare -xr FONT_STYLE_RESET='\033[0m';

declare -xr COMMENT=$(echo -ne $FONT_COLOR_BLUE);
declare -xr REQUIRED=$(echo -ne $FONT_COLOR_RED);
declare -xr OPTIONAL=$(echo -ne $FONT_COLOR_GREEN);
declare -xr ADVANCED=$(echo -ne $FONT_COLOR_MAGENTA);
declare -xr FORDEBUG=$(echo -ne $FONT_COLOR_CYAN);
