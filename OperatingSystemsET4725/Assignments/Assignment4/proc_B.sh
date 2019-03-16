#! /bin/bash

echo "proc_B starting"

while [ "$input" != "Last Message!" ] do
	read input < pipe1
	echo $input
	sleep 1
done

echo "Something"

if [ ! -p pipe2 ]; then
	mkfifo pipe2
else
	echo "Pipe2 already exists"
fi

echo "$$" > pipe2

echo "proc_B has completed"

wait
exit 0


