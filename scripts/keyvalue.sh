#!/bin/bash
curl -s https://coderbyte.com/api/challenges/logs/web-logs-raw -O > /dev/null
reqid=""
fwdstr=""
cat web-logs-raw | \
while read line || [ -n "$line" ];
do
        #echo ">>>$line"
        if echo "$line" | grep -q "coderbyte heroku/router";
        then
                for change in $line; do
                    set -- `echo $change | tr '=' ' '`
                    key=$1
                    value=$2
                    if [ "$key" == "request_id" ]; then
                            reqid=$value
                    elif [ "$key" == "fwd" ]; then
                            if [ "$value" == "\"MASKED\"" ]; then
                               fwdstr="[M]"
                            else
                               fwdstr=""
                            fi
                    fi
                done
                echo "$reqid $fwdstr"
        fi
done
