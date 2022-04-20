#!/bin/bash

# $1 - count of clients
# $2 - IP address
# $3 - port
# $4 and etc - message (not required)

# where is our script contains;
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

i=4
ARG_4=""

while [[ -n ${!i} ]]; do
    ARG_4="$ARG_4 ${!i}"
    ((++i))
done

# place where executable file
CLIENT_APP="${SCRIPT_DIR}/../build/client/client-app"
# place where command with args contains for start app
TEMP_SCRIPT="${SCRIPT_DIR}/tmp.sh"
if [[ $ARG_4 != "" ]]; then
    echo "Signle message"
    echo "ARG_4 $ARG_4"
    echo "\"${CLIENT_APP}\" $2 $3 \"$ARG_4\"" > "${TEMP_SCRIPT}"    #single message mode
else
    echo "interactive"
    echo "\"${CLIENT_APP}\" $2 $3" > "${TEMP_SCRIPT}"   # interactive mode
fi
# give rules for execute;
chmod 755 "${TEMP_SCRIPT}"

# open several terminals
for ((i=0; i < $1; ++i )); do
    open -a Terminal "${TEMP_SCRIPT}"
done