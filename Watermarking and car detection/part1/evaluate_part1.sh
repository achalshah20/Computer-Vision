#!/bin/bash

echo "Image name: Kamehouse.png and N: 15"

./watermark 1.3 input/KameHouse.png output/watermarked_KameHouse_15.png add 15

echo "Kamehouse.png,corr,watermark_present" > results/kamehouse.csv
for i in {1..100}
do
    o="$(./watermark 1.3 input/KameHouse.png output/watermarked_KameHouse_15.png check $i)"
    comp=0.20
    echo "num: $i & corr: $o"

	if (( $(echo "$o > $comp" |bc -l) )); then
		echo " "
		echo "Found corr greater than 0.20"
		echo " "
        result="$(echo 1)"
    else
    	result="$(echo 0)"

    fi
    echo "$i,$o,$result" >> results/kamehouse.csv

done


echo "Image name: Clean.png and N:20"

echo "clean.png,corr,watermark_present" > results/clean.csv

./watermark 1.3 input/clean.png output/watermarked_clean_20.png add 20

for i in {1..100}
do
    o="$(./watermark 1.3 input/clean.png output/watermarked_clean_20.png check $i)"
    comp=0.20
    echo "num: $i & corr: $o"
	if (( $(echo "$o > $comp" |bc -l) )); then
		echo " "
		echo "Found corr greater than 0.20"
		echo " "
		result="$(echo 1)"
    else
    	result="$(echo 0)"

    fi
    echo "$i,$o,$result" >> results/clean.csv

done
echo "Image name: nature.png and N:37"


echo "nature.png,corr,watermark_present" > results/nature.csv

./watermark 1.3 input/nature.png output/watermarked_nature_37.png add 37

for i in {1..100}
do
    o="$(./watermark 1.3 input/nature.png output/watermarked_nature_37.png check $i)"
    comp=0.20
    echo "num: $i & corr: $o"
	if (( $(echo "$o > $comp" |bc -l) )); then
       	echo " "
		echo "Found corr greater than 0.20"
		echo " "
        result="$(echo 1)"
    else
    	result="$(echo 0)"

    fi
    echo "$i,$o,$result" >> results/nature.csv
done

echo "mountain.png,corr,watermark_present" > results/mountain.csv


echo "Image name: mountain.png and N:60"

./watermark 1.3 input/mountain.png output/watermarked_mountain_60.png add 60

for i in {1..100}
do
    o="$(./watermark 1.3 input/mountain.png output/watermarked_mountain_60.png check $i)"
    comp=0.20
    echo "num: $i & corr: $o"
	if (( $(echo "$o > $comp" |bc -l) )); then
       	echo " "
		echo "Found corr greater than 0.20"
		echo " "        
        result="$(echo 1)"
    else
    	result="$(echo 0)"

    fi
    echo "$i,$o,$result" >> results/mountain.csv
done