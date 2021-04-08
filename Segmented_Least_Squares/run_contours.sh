#!/bin/bash
make Segmented_Least_Squares
echo "Compiled Successfully"
if [ $# -eq 0 ]
then
    ./Segmented_Least_Squares
else
    ./Segmented_Least_Squares < $1
fi
python3 DAA_Vis2.py
