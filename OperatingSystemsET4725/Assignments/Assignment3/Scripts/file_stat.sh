#! /bin/bash

# Title:         file_stat
# Description:   This is a script used to display the current diskspace used and available blocks from the home directory. 
# 		 When the availbable blocks go below a certian treshold a warning message is displayed. 
#		 I also put in a double warning for usage % at 90%.
# Author:        Graham Claffey
# ID:            18296661


# Display available blocks and percentage fo used space
# If available blocks < 5000000
# echo warning to the terminal "Disk Space is running low, n% is used"
# else echo the used that the disk space is sufficient 

# creating some global variables
# AVAILABLE BLOCKS
availableBlocks=$(df ~ | tail -1 | awk '{print $4}')
# DISK USAGE
usagePercent=$(df ~ | tail -1 | awk '{print $5}' | sed 's/%//')
# REMAINING SPACE
remainingPercent=$((100-$usagePercent))

# if else block to check the if the availbable blocks is below 500k
if(($availableBlocks < 5000000));then
 	echo -e "\n\e[41mWARNING:\e[0m Disk space is running low, \e[1m\e4m$usagePercent%\e[0m is used"       
else
	echo -e "\n\e[42mMESSAGE:\e[0m There is a suffiecnt amount of diskspace, at \e[1m\e[4m$remainingPercent%\e[0m left"
fi

# if else block to check if the disk usage is above 90%
if(($usagePercent > 90));then
	echo -e "\n\e[41mWARNING:\e[0m currently only \e[1m\e[4m$reainingPercent%\e[0m of diskspace is remaining!!"
else
	echo -e "\n\e[42mMESSAGE:\e[0m You are currently at \e[1m\e[4m$usagePercent%\e[0m with \e[1m\e[4m$availableBlocks\e[0m 1K-blocks remaining"
fi
