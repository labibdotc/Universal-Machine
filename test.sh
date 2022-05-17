#! /bin/sh

echo "testing..."

# UMLocation=$(ls $1)
# testFiles=$(ls $2)
# echo $testFiles
# echo $UMLocation


# grep ".*\.um$"

# find . -name "*.um"
testFiles=()
#suss line of code!! copied from internet
testFiles=(`find . -name "*.um" | sort -r | head -1000`)

#echo "${testFiles[*]}"

for testFile in "${testFiles[@]}" ; do 
    um $testFile > example.txt
    ./um $testFile > ours.txt
    diff ours.txt example.txt
done