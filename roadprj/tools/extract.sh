#!/bin/sh

for file in ./*.tar.gz
do
    tar -xf $file
    rm -f $file
done
