#! /bin/bash

for i in `ls $1` 
do
    cat $1/$i | grep Makespan | cut -f 2,4,5 -d' ' > eval/$1/$i"_mks"
done
