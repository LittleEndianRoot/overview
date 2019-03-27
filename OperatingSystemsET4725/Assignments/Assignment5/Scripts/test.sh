#! /bin/bash

# measure how long a nominal 1 second sleep takes in nanoseconds

t1=$(( $(date +%s%N) )) # read time in nanoseconds

sleep 1                 # sleep for a nominal 1 second

t2=$(( $(date +%s%N) )) # read the time again in nanoseconds

elapsedTime=$((t2-t1)) # compare time differences

echo "The one second sleep took an actual $elapsedTime nanoseconds"

exit


