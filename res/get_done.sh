#! /bin/bash
TEMP=eval/$1/temp
cat $1/* | grep Done > $TEMP
cut -f 2 -d ' ' eval/$1/temp > eval/$1/"done"
rm $TEMP
