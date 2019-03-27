#! /bin/bash





# The array of varied files
my_array=(dd if=/dev/zero of=myFile_1 bs=1024 count=18 status=none 
dd if=/dev/zero of=myFile_2 bs=512 count=30 status=none 
dd if=/dev/zero of=myFile_2 bs=256 count=700 status=none 
dd if=/dev/zero of=myFile_2 bs=128 count=20 status=none 
dd if=/dev/zero of=myFile_2 bs=64 count=34 status=none) 
