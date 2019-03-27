# Title:	copy_rate.sh
# Description:	A program to create some files of abratrary size and show the elapsed copy
#               times of these files.
# Author:	Graham Claffey
# ID:		18296661

# Create an array of file names
# Make the file of various sizes with the dd command 10 kbytes to 50 Mbytes
# Create a function that is passed a file name as an argument
# Copy files to another file (_temp) using the cp command
# find the size of the files that were created with wc 
# measure the elapsed time to copy these files of variying size with the date command
# calculate the transfer rate of each file that was copied


# The array of file names
my_array=("myFile_1" "myFile_2" "myFile_3" "myFile_4" "myFile_5")

# Creating our files of varirying size
dd if=/dev/zero of=myFile_1 bs=1024 count=18 status=none
dd if=/dev/zero of=myFile_2 bs=512 count=30 status=none
dd if=/dev/zero of=myFile_3 bs=256 count=700 status=none
dd if=/dev/zero of=myFile_4 bs=128 count=20 status=none
dd if=/dev/zero of=myFile_5 bs=64 count=34 status=none

copyRate_function ()
{
	
	# Calulating the data
	startTime=$(( $(date +%s%N/1000000) ))
	
	cp $1 _temp
	
	endTime=$(( $(date +%s%N/1000000) ))
	
	elapsedTime=$((endTime-startTime))
	
	size=$(wc -c < $1)
	
	transferRate=$((size/elapsedTime))

	# Format of the data gathered	
	echo -en "$1\t"
	echo -en "$size\t KiB\t"
	awk "BEGIN {printf\"    %.2f ms\t\",$elapsedTime}"
	awk "BEGIN {printf\"%.2f Kbytes/s\n\",$transferRate}"
}

# Column headings
echo -en "\n\n\e[1m\e[4m\e[94mFile Name\e[0m\t"
echo -en "\e[1m\e[4m\e[94mFile Size\e[0m\t"
echo -en "\e[1m\e[4m\e[94mElapsed Time\e[0m\t"
echo -en "\e[1m\e[4m\e[94mTransfer Rate\e[0m\n"

# execute the data that has been gathered in the copyRate_function
i=0
while [ $i -lt 5 ]
do
	copyRate_function "${my_array[i]}"
        let i=i+1
done


# Remove the files with the use of the array
echo -e "\n\e[91mRemoving temporary files !!!\e[0m"

j=0
while [ $j -lt 5 ]
do
	rm "${my_array[j]}"
	let j=j+1
done

rm _temp

echo "copy_rate.sh exiting now ..."

exit 0
