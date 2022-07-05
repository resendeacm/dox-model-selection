#!/bin/bash
rm matfigure.m *~ *# &> /dev/null

# Arguments
ARGS=("$@")

# Experiment Times
TIMES=( $(awk '{print $1}' ${ARGS[1]}) )

# Matlab Plot
rm matfigure.m &> /dev/null
LOAD=sfpOutput_qoi_seq
VECTOR=fp_mc_QoiSeq_unified

# Calibration Data
CALNAME=solution.dat

# Print Solution
echo "clear all;" | tee -a matfigure.m &> /dev/null
echo "${LOAD};" | tee -a matfigure.m &> /dev/null
echo "file = fopen('${CALNAME}','w');" | tee -a matfigure.m &> /dev/null

for l in $(seq 1 ${#TIMES[@]})
do
	let ll=l-1
	echo "fprintf(file, '%e\t%e\t%e\n', ${TIMES[${ll}]}, mean(${VECTOR}(:,${l})), std(${VECTOR}(:,${l})));" | tee -a matfigure.m &> /dev/null
done

echo "fclose(file);" | tee -a matfigure.m &> /dev/null
echo "exit;" | tee -a matfigure.m &> /dev/null

# Run Script
${ARGS[0]} -nodesktop -nosplash -r "matfigure"

# Gnuplot
gnuplot << EOF
	set term postscript eps enhanced color font "Helvetica, 24"
	set key out horiz right top font "Helvetica, 20" spacing 2.0 width -2.5 samplen 3.5
	set output "calibration.eps"
	set style fill transparent solid 0.2
	set colorsequence classic
	set print "-"
	
	set xlabel "Time (Hours Post-Treatment)"
	set ylabel "Cell Counts"

	# Range
	set xrange[-120:600]
	set yrange[0.0:4.0]

	set format y "%.1f"
	set label "x 10^{4}" at -120.0,4.2
	set xtics 0,100,600

	set pointsize 0.9
	plot "${ARGS[2]}" using 1:(\$2/1.0e4):(\$3/1.0e4) with yerrorbars lc 7 lt 7 t "Experiment",\
		 "./${CALNAME}" using 1:((\$2 + \$3)/1.0e4):((\$2 - \$3)/1.0e4) with filledcu fill lc 3 notitle,\
		 "./${CALNAME}" using 1:(\$2/1.0e4) with lines lc 3 lw 2 t "Calibration"
EOF

exit
