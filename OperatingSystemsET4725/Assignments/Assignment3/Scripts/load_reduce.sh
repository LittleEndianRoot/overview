#! /bin/bash

# Title:         load_reduce
# Description:	 A program that is used to display and terminate the highest running process
#		 in terms of CPU usage. In our example this process will be the busy_wait.sh
#		 script that just increments a variable forever with no delay.
#		 So the busy wait script will always end up being the highest process for us
# 		 due to the fact that it will hog 1 of the cores on our CPU
# Author:	 Graham Claffey
# ID:		 18296661

# Run the busy_wait script in the backgroud with all execution privs
# Display the PID number for the busiest process, in terms of CPU%
# Assign the PID number to a variable
# Kill the busiest process and display a message saying the naem of which process was killed


# busy_wait execution
chmod a+x busy_wait.sh
./busy_wait.sh &
sleep 1

# Display PID of busiest process
highestProcessPID=$(ps aux | awk '{print $3, $2}' | sort -h | tail -1 | awk '{print $2}')

# Display how much CPU% the busiest process uses
highestProcessUsage=$(ps aux | awk '{print $3}' | sort -h | tail -1)

# Display highest process name
highestProcessName=$(ps -aux | awk '{print $3, $11}' | sort -h | tail -1 | awk '{print $2}')

# kill and display the details of the process with the highest CPU usage
echo -e "`kill -15 $highestProcessPID` \nKilling the processs $highestProcessName"
echo -e "\nWith a PID of $highestProcessPID."
echo -e "\nIt was using $highestProcessUsage% of your CPU resources."

