#! /bin/bash

echo "------------------|array no quotes|--------------------"

my_array1=(black brown red)

echo ${my_array1[@]}

echo "-------------------|array with quotes|-------------------"

my_array2=("black" "brown" "red" "sea blue")

echo ${my_array2[@]}

echo "-------------------|index in array|-------------------"

my_array3=([0]="black" [1]="brown" [2]="red" [6]="sea blue")

echo ${my_array3[@]}

echo "-------------------|array with for loop|-------------------"

my_array4=("black" "brown" "red" "sea blue")

for colour in "${my_array4[@]}";
do
	echo "$colour"
done

echo "--------------------|array with printf|-----------------"

my_array5=("black" "brown" "red" "sea blue")

printf "%s\n" "${my_array5[@]}"

echo "--------------------|simple single string|----------------"

my_array6=("black" "brown" "red" "sea blue")

printf "%s\n" "${my_array5[*]}"

echo "--------------------|number of elements|------------------------"

my_array6=("black" "brown" "red" "sea blue")

echo "The number of elements is: ${#my_array6[@]}"

echo "-------------------|length of element|---------------------"

my_array7=("black" "brown" "red" "sea blue")

echo "The length of element 3 is ${#my_array7[3]} characters"

echo "--------------------|associative array single element|------------------------"

declare -A teachers1

teachers1=(["Science"]="I. Newton" ["Maths"]="A. Einstein" ["English"]="W. Shakespeare")

echo ${teachers1[Maths]}

echo "-------------------|associative array all elements|-------------------------"

declare -A teachers2

teachers2=(["Science"]="I. Newton" ["Maths"]="A. Einstein" ["English"]="W. Shakespeare")

echo ${teachers2[@]}

echo "--------------------|associative array keys and values|---------------------------"

declare -A teachers3

teachers3=(["Science"]="I. Newton" ["Maths"]="A. Einstein" ["English"]="W. Shakespeare")

for subj in "${!teachers3[@]}";
do
	echo "For the subject $subj, the teacher is: ${teachers3[@]}."
done


echo "----------------------|itterate over keys and values|--------------------------------"

declare -A teachers4

teachers4=(["Science"]="I. Newton" ["Maths"]="A. Einstein" ["English"]="W. Shakespeare")

for i in "${!teachers4[@]}";
do
	echo "key: $i"
	echo "value: ${teachers4[$i]}"
done
exit 0
