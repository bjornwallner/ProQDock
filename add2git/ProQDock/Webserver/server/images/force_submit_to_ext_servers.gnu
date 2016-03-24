set terminal postscript eps color solid "Helvetica" 24
set output 'force_submit_to_ext_servers.eps'
set size 0.48,0.07
unset xtics
unset ytics
unset border
set yrange [0.25:1];
set xrange [-1:1.0];
set label "Force submit to external servers" at 0,0.5 center

set key off
plot 0,0 notitle
