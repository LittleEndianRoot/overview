#! /bin/bash

my_array=("myFile_1" "myFile_2" "myFile_3" "myFile_4" "myFile_5")

dd if=/dev/zero of=myFile_1 bs=1024 count=18 status=none
dd if=/dev/zero of=myFile_2 bs=512 count=30 status=none
dd if=/dev/zero of=myFile_3 bs=256 count=700 status=none
dd if=/dev/zero of=myFile_4 bs=128 count=20 status=none
dd if=/dev/zero of=myFile_5 bs=64 count=34 status=none

test_function ()
{
	
	startTime=$(( $(date +%s%N/1000000) ))
	
	cp $1 _temp
	
	endTime=$(( $(date +%s%N/1000000) ))
	
	elapsedTime=$((endTime-startTime))
	
	size=$(wc -c < $1)
	
	transferRate=$((size/elapsedTime))

	echo -en "$1\t"
	echo -en "$size\t KiB\t"
	awk "BEGIN {printf\"    %.2f ms\t\",$elapsedTime}"
	awk "BEGIN {printf\"%.2f Kbytes/s\n\",$transferRate}"
}


echo -en "\n\n\e[1m\e[4m\e[94mFile Name\e[0m\t"
echo -en "\e[1m\e[4m\e[94mFile Size\e[0m\t"
echo -en "\e[1m\e[4m\e[94mElapsed Time\e[0m\t"
echo -en "\e[1m\e[4m\e[94mTransfer Rate\e[0m\n"

i=0
while [ $i -lt 5 ]
do
	test_function "${my_array[i]}"
        let i=i+1
done

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
