#!/bin/bash
make Contour
if [ $# -eq 0 ]
then
    ./Contour
else
    ./Contour < TestFile/$1
fi
python3 DAA_Vis.py
