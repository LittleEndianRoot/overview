#! /bin/bash

# Title:
# Description:
# Author:
# ID:


# Two proccesses called proc_A and proc_B communicate via a single named pipe1
# proc_A starts first: sends a message on pipe1 5 times. Then it sends the "Last message"
# to indicated the communication from proc_A is completed.
# proc_B starts in a separate terminal: when proc_B receives the message "Last message" on pipe1
# It then creates a pipe named pipe2 and sends its PID over pipe2.
# Then it prints "proc_B looping" every second.
# The proc_A will then read pipe2 and write the message that was received to the screen stdout
# proc_A will send a "HUP" signal to proc_B using the PID that was received over pipe2. 
# It will then remove pipe1 and exit.
# The proc_B process will use a trap function to intercept the HUP signal and it will also lower it's priority by 5.
# The proc_B will remove pipe2 and then exit.


echo "proc_A starting"

#trap 'trap_function' SIGHUP

if [ ! -p pipe1 ]; then
	mkfifo pipe1
else 
	echo "Pipe1 already exists"
fi

# send message 5 times
for((x=0; x<5; x++));do
	echo "Hello from $0" > pipe1
	sleep 1
done

echo "Last message!" > pipe1
sleep 1
rm pipe1

while [ "$input" != pipe2 ] do
	read input < pipe2
	echo "Proc_B.sh PID: $input"
	sleep 1
done

echo "proc_A has completed"

wait
exit 0


