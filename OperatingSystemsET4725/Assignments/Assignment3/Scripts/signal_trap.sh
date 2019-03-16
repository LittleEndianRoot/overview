signal_trap.sh
#! /bin/bash

# Title:	 signal_trap
# Description:	 A program that displays the most intensive CPU process which in our case will
#		 be busy_wait.sh. auto launch the busy_waith.sh script and wait for an
#		 interrupt signal, when we get the signal output some details of the process
#		 to the screen and then use SIGKILL(9) to kill the process.
# Author:	 Graham Claffey	
# ID:            18296661


# Run the busy_wait script
# trap_function() modification 
# - Display the CPU utilisation for busy_wait 
# - kill the busy_wait program and exit when we recieve a signal interrupt 

# trap_functoin is waiting for an interrupt signal(Ctrl+c)
trap 'trap_function' SIGINT

# trap_function to trigger when we press Ctrl+c
trap_function()
{
	# Create local variables inside this function
	local highestProcessPID=$(ps -aux | awk '{print $3, $2}' | sort -h | tail -1 | awk '{print $2}')
	local highestProcessUsage=$(ps -p $highestProcessPID --format %cpu | tail -1)	

	# echo the detials of the process to the screen and KILL the process (SIGKILL) also -9
	echo -e "\n\e[1m\e[4mPID for busy_wait was:\e[0m $highestProcessPID"
	echo -e "\e[1m\e[4mAt termination the CPU utilisation was:\e[0m $highestProcessUsage%"
	kill -SIGKILL "$highestProcessPID"
	wait
	exit
}

i=0 # our counter variable used to count the seconds

./busy_wait.sh & # run the busy_wait.sh script in the backgroud (& << background signifier)
# This loop will run and print the current real time information of the highest CPU % process
# the information is printed every second, the loop will end when it recieves a SIGINT which tiggers the SIGKILL
while true
do
	clear
	let i++
	procPID=$(ps -aux | awk '{print $3, $2}' | sort -h | tail -1 | awk '{print $2}')
	procUsage=$(ps -p $procPID --format %cpu | tail -1)

	echo -e "\n\e[1m========Process run time: $i seconds============\e[0m"
	echo -e "\e[1m\e[4mThe PID for most CPU intensive is:\e[0m $procPID"
	echo -e "\e[1m\e[4mCurrent CPU utilisation is:\e[0m $procUsage%"
	sleep 1
done
wait
exit
