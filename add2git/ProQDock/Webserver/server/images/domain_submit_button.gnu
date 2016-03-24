set terminal postscript eps color solid "Helvetica" 20
set output 'domain_submit_button.eps'
set size 0.45,0.10
unset xtics
unset ytics
unset border
set yrange [0.25:1];
set xrange [-1:1.0];
set label "SUBMIT UNALIGNED REGION" at 0,0.5 center

set key off
plot 0,0 notitle
