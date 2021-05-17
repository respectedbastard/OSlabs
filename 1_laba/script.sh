#!bin/bash
mkdir 9091
cd 9091
mkdir Semenov
date > Egor.txt
date --date="next monday" > filedate.txt
cat Egor.txt filedate.txt > result.txt
cat result.txt