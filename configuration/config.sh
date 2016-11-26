# this file store all user specific enviromental variables
# ideally, a new user in a new enviroment only need to modify this file to use this pipeline correctly

# load python2 virtualenv
if [[ $(uname -s) == "Darwin" ]]; then
    source $HOME/py2/bin/activate
fi
