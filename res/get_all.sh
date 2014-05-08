#! /bin/bash
D1=j30
./get_done.sh $D1
./get_optimal.sh $D1
./get_makespan.sh $D1
./compute_results.py $D1
