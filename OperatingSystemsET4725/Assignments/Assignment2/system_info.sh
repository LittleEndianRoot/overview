#! /bin/bash

#Title:          system_info
#Description:    This is a script used to display various information on the current system,
#                and displays the system information to the user, in an easy to read and neatly formatted fashion.
#Format info:	 The formatting that is used is using the Bash escape sequences. If you wish to know more about
#		 these escape sequnces vist: https://misc.flogisoft.com/bash/tip_colors_and_formatting
#Commands:       echo, cat, ls, grep, awk, sed(redundant-ish), sort, head, wc, uname, id, hostname, df, whoami, find
#Tools:      	 Text editor: Vim, Terminal Emulator: Terminator
#Author:         Graham Claffey                                                                                           
#ID:             18296661


#----------------USER Details---------------------------------------------
echo -ne "\n\e[1mThe current user is:\e[0m "

#print current user
echo -e "\e[94m`whoami`\e[0m"

echo -ne "\n\e[1mThe current user ID:\e[0m "

#print user id
echo -e "\e[35m`id -u`\e[0m"

echo -ne "\n\e[1mThe host name of this machine:\e[0m "

#print the name of the host machine
echo -e "\e[32m`hostname`\e[0m"

echo -ne "\n\e[1mThe machine type is:\e[0m "

#display machine type
echo -e "\e[94m`echo $MACHTYPE`\e[0m"

#-----------------------SYSTEM INFO---------------------------------------

echo -e "\n\e[1m\e[4mThe host operating system is:\e[0m"

#print the host OS
hostnamectl | grep --color "Operating System"

echo -ne "\n\e[1m\e[4mTotal number of files in the home directory:\e[0m "

#print home directory files
echo -e "\e[42m\e[30m`find $HOME -type f 2>&1 | wc -l`\e[0m"      # result: 97704    5 files difference 
# ls ~ lRa -1 2>&1 | grep '^-' | wc -l                              result: 97699    

# find $HOME -type f 2>&1 | grep -v "Permission denied" | wc -l     result: 97700
# ls ~ lRa 2>&1 | grep '^-' | grep -v "Permission denied" | wc -l   result: 97700

# Seems to me like in this situation the grep -v is just ommiting the Permision denied files
# which leaves a difference of 5 files in my $HOME, ~
# I will test this more soon...

# CONCLUSION: ls -lRa -1 2>&1 | egrep -c '^l|^-'                    # result: 977704
# Seems like I was missing the files with the symbolic link (l) identifier. 
# With the help of egrep I got the same result as the find utility.   

echo -ne "\n\e[1m\e[4mTotal number of directories in the home directory:\e[0m "

#print home directory, directory files
echo -e "\e[43m\e[30m`ls -lRa ~ 2>&1 | egrep -c '^d'`\e[0m"

echo -ne "\n\e[1m\e[4mThe shell environment is:\e[0m "

echo -e "\e[33m`set | grep "SHELL=" | sed -e 's|/||g' -e 's/SHELL=//g' -e 's/bin//g'`\e[0m"

#--------------------DISK INFO--------------------------------------------

echo -ne "\n\e[1m\e[4mTotal number of free blocks:\e[0m "

#show free blocks
echo -e "\e[46m\e[30m`df | awk '{print $6, $4}' | sort -k1 | head -1 | awk '{print $2}'`\e[0m"

echo -ne "\n\e[1m\e[4mUsed space is:\e[0m "

#Show used space
#(sed is redundant for my purposes here, but if I want to use the Use% value as a variable, I can do so without worry)
echo -e "\e[41m\e[30m`df | awk '{print $6, $5}' | sort | head -1 | awk '{print $2}' | sed 's/%//g'`%\e[0m" 

#---------------------------CPU INFO--------------------------------------

echo -e "\n\e[1m\e[4m\e[44m\e[30mThe CPU model name is:\e[0m "

#print cpu model
cat /proc/cpuinfo | head -10 | grep -i "model name"

echo -e "\n\e[1m\e[4mNumber of CPU Cores:\e[0m "

cat /proc/cpuinfo | head -15 | grep --color "cpu cores"

echo -e "\n\e[1m\e[4m\e[46m\e[30mThe CPU speed is:\e[0m "

#make a list of the cpu core speeds
cat /proc/cpuinfo | grep -i --color "cpu MHz"

echo -e "\n\e[1m\e[4m\e[42m\e[30mThe CPU cache size is:\e[0m "

#display the cpu cache size
cat /proc/cpuinfo | head -15 | grep --color "cache size"

echo -e "\n\e[1m\e[4m\e[44m\e[30mThe CPU vendor ID is:\e[0m "

#print the vendor id
cat /proc/cpuinfo | head -10 | grep --color "vendor_id"

#----------------------------MEMORY INFO----------------------------------

echo -e "\n\e[1m\e[4mThe total memory size is:\e[0m "

#print total system memory
cat /proc/meminfo | grep -i --color "MemTotal"

echo -e "\n\e[1m\e[4mThe size of free memory is:\e[0m "

#print total free memory
cat /proc/meminfo | grep -i --color "MemFree"

echo -e "\n\e[1m\e[4mThe available memory is:\e[0m"

#print available memory
cat /proc/meminfo | grep --color "MemAvailable"
