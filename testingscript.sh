#! /bin/sh

echo "testing..."

testFiles=$(ls $2)
echo testFiles
um multiply.um > example.txt
./um multiply.um > ours.txt
diff ours.txt example.txt
