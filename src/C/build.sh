#!/bin/bash

set -e # exit on error

rootData="../../data"

ikarus="./ikarus"
csv="$rootData/DigitalTypefaces.csv"
json="$rootData/DigitalTypefaces_C.json"
tempCSV="$rootData/temp.csv"
tempJSON="$rootData/temp.json"


echo "Build Ikarus"
clang \
    -std=c90 \
    -Wno-implicit-function-declaration \
    --include-directory ./lib \
    --include-directory ./ \
    main.c \
    ./lib/BuchM.c \
    ./lib/SplinM.c \
    ./lib/KottaM.c \
    ./lib/AusglM.c \
    ./lib/VektoM.c \
    ./lib/SaurM.c \
    ./lib/DraikM.c \
    Output.c \
    -v -o $ikarus

echo "DigitalTypefaces – Convert CSV"
mlr --nidx --fs "|" --ofs ',' cut -f 2,3,4 then clean-whitespace $csv > $tempCSV

echo "DigitalTypefaces – Running Ikarus"
$ikarus $tempCSV > $tempJSON
jq . $tempJSON > $json
prettier --write $json


# ----------------- next font -----------------

csv="$rootData/IkarusNimbusSansRegular.csv"
json="$rootData/IkarusNimbusSansRegular_C.json"


# convert to comma separated and skip comments
grep -v '^#' $csv | mlr --icsv --ifs ' ' --ocsv cat 1> $tempCSV

# remove first column
mlr --nidx --fs "," cut -x -f 1 $tempCSV | sponge $tempCSV

# replace Start, Corner, Curve, Tangent with 12, 13, 14, 15
sed -i '' \
    -e 's/^Start,/12,/' \
    -e 's/^Corner,/13,/' \
    -e 's/^Curve,/14,/' \
    -e 's/^Tangent,/15,/' $tempCSV


echo "NimbusSansRegular – Running Ikarus"
$ikarus $tempCSV > $tempJSON
jq . $tempJSON > $json
prettier --write $json


rm -rf $tempCSV
rm -rf $tempJSON