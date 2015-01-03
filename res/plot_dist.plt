set xlabel "Vzdalenost makespan od optima [s]"
set ylabel "Doba reseni SAT problemu [s]"
#set logscale x
set terminal png
set output "dst.png"
set autoscale
set grid
plot "eval_res/j120_dst" using 1:2 with points title ""
