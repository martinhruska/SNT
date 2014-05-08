#! /bin/bash

for i in `ls $1` 
do
    cat $1/$i | grep Optimal | cut -f 2 -d' ' > eval/$1/$i"_opt"
done
