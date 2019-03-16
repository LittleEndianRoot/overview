#! /bin/bash

# Title:	proc_B.sh
# Description:	This script will demonstrate sending data between proc_B.sh and proc_B.sh
#		and in proc_B.sh we recieve some data from proc_A.sh via pipe1
#		then we send the PID of this process to proc_A.sh and loop until we recieve
#		a SIGHUP signal. Using the trap function we can hangup out program
# Author:	Graham Claffey
# ID:		18296661

echo -e "\e[1m--------------| proc_B.sh |--------------\e[0m"
echo -e "\e[1mproc_B starting!\e[0m"
echo -e "\e[1mPID of $0 is: $$\e[0m"
echo -e "\e[1m-----------------------------------------\e[0m"

# setting up the trap_function to trigger when we recieve the hangup signal
trap 'trap_function' SIGHUP

trap_function()
{
	# Print some information and increase the niceness of this process
	# so we can hangup then exit the process
	echo -e "\e[95m#############################################\e[0m"
	echo -e "\e[31mTrap routine initiated!\e[0m"
	echo -e "\e[31mSignal caught!!!\e[0m\n\n"
	
	echo -e "\e[31m$0: Increasing my niceness by 5\e[0m"
	renice 5 $$
	echo -e "\e[31m$0 here, PID $$! I am about to remove pipe2!\e[0m"
	rm pipe2
	echo -e "\e[31mTrap routine finished!\e[0m"
	echo -e "\e[31m$0: Exiting now...\e[0m"
	echo -e "\e[95m#############################################\e[0m"
	exit 0
}

# looping until we recieve the "Last Message!" string from pipe1
while [ "$input" != "Last Message!" ]
do
	if [ ! -p pipe1 ];
	then
		mkfifo pipe1
		sleep 1
		read input < pipe1
	else
		read input < pipe1
	fi
	echo $input
	sleep 1
done

if [ ! -p pipe2 ];
then
	echo -e "\e[32mCreating pipe2\e[0m"
	mkfifo pipe2
else
	echo -e "\e[7mPipe2 already exists\e[0m"
fi

# put this processes PID into the PID variable
PID=$$
sleep 1

# send the PID variable to pipe2
echo "$PID" > pipe2
# if you don't want to make a variable to send the PID
# echo "$$" > pipe2
# this works the same way
sleep 1

# loop forever printing every second
while true
do
	echo -e "\e[31mproc_B looping every second\n\e[0m"
	sleep 1
done

wait
