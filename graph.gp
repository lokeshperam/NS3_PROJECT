set terminal png
set output 'graph.png'
set xlabel 'Latency'
set ylabel 'Throughput (KBps)'
set title 'Latency vs Throughput '
set grid
plot 'data.txt' with linespoints
