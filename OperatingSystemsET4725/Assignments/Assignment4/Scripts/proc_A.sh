#! /bin/bash

# Title:	proc_A.sh
# Description:  This script will demonstrate sending data between proc_A.sh and proc_B.sh
#		and in proc_A.sh we use the PID that we recieve from proc_B to send a SIGHUP
#		signal.
# Author:	Graham Claffey
# ID:		18296661


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


echo -e "\e[1m-------------| proc_A.sh |----------------\e[0m"
echo -e "\e[1mproc_A.sh Starting!\e[0m"
echo -e "\e[1mPID of $0 is: $$\e[0m"
echo -e "\e[1m------------------------------------------\e[0m"

# if pipe1 does not exist 
if [ ! -p pipe1 ];
then
	# create pipe1
	echo -e "\e[32mCreating pipe1\e[0m"
	mkfifo pipe1
else 
	# echo this message if pipe1 exists
	echo -e "\e[7mPipe1 already exists\e[0m"
fi


echo "$0: Attempting to send a message through pipe1!"
# send message 5 times
for((x=0; x<5; x++));
do
	# echo this message into pipe1
	echo "Hello from $0" > pipe1
	sleep 1
done

# echo the final message into pipe1
echo "Last Message!" > pipe1
sleep 1

echo -e "\n$0: Waiting to receive a message through pipe2"

# if pipe2 has not been created
if [ ! -p pipe2 ];
then
	# make Fist In First Out pipe
	mkfifo pipe2
	sleep 1
	# then read the datat from pipe2
	read pid < pipe2
else
	# else just read the data
	read pid < pipe2
fi

# This is the data message(proc_B.sh pid) we recieved from pipe2
echo -e "\e[36m================================\e[0m"
echo -e "\e[32mpipe2 received message: \e[34m$pid\e[0m"
echo -e "\e[36m================================\e[0m"
sleep 1

echo -e "$0: Sending HUP signal to \e[34m$pid\e[0m!"
sleep 3
# send a SIGHUP signal with the PID of proc_B
kill -SIGHUP "$pid"

# remove pipe1
rm pipe1
echo "Removing pipe1 and exiting now!"

# wait for the child process to finish 
wait

exit 0
