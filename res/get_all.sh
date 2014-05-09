#! /bin/bash

function make_test {
    ./get_done.sh $1
    ./get_optimal.sh $1
    ./get_makespan.sh $1
    ./compute_results.py $1
}

make_test j30
make_test j60
