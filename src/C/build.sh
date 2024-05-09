#!/bin/bash

set -e # exit on error

rootData="../../data"

ikarus="./ikarus"


echo "Build Ikarus"

# clean up
rm -rf $ikarus

# Build
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

# ----------------- conversion with test-font -----------------

echo "Start conversion of test font"

tempCSV="$rootData/temp.csv"
tempJSON="$rootData/temp.json"
csv="$rootData/IkarusNimbusSansRegular.csv"
json="$rootData/IkarusNimbusSansRegular_C.json"

# remove old converted file
rm -rf $json

# copy csv to temp
cp $csv $tempCSV

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