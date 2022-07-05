#!/bin/bash
rm matfigure.m *~ *# &> /dev/null

# Arguments
ARGS=("$@")

# Calibration Parameters (List)
PAR=(n0 kp theta)

# Raw File
FILE=cal_rawChain_ml

# Vector & Likelihood
VECTOR=`echo cycle_cal_ip_ml_${ARGS[1]}_rawChain_unified`
VELIKE=`echo cycle_cal_ip_ml_${ARGS[1]}_rawLogLikelihood_unified`
NBINS=100

# MATLAB Plot
echo "clear all;" | tee -a matfigure.m &> /dev/null
echo "${FILE};" | tee -a matfigure.m &> /dev/null

# Histogram & Likelihood
for p in $(seq 1 ${#PAR[@]})
do
	let pp=p-1
	echo "fprintf(1, '[Plotting Histogram & Likelihood - RAW ${p}]\n');" | tee -a matfigure.m &> /dev/null
	
	# Histogram
	NAME=${PAR[${pp}]}_histraw
	echo "NBINS=${NBINS};" | tee -a matfigure.m &> /dev/null
	echo "figure" | tee -a matfigure.m &> /dev/null
	echo "hist(${VECTOR}(:,${p}), NBINS);" | tee -a matfigure.m &> /dev/null
	echo "h = findobj(gca, 'Type', 'patch');" | tee -a matfigure.m &> /dev/null
	echo "set(h, 'FaceColor', 'b', 'EdgeColor', 'w');" | tee -a matfigure.m &> /dev/null
	echo "xlabel('${PAR[${pp}]}', 'fontname', 'Times', 'fontsize', 16);" | tee -a matfigure.m &> /dev/null
	echo "ylabel('Frequency', 'fontname', 'Times', 'fontsize', 16);" | tee -a matfigure.m &> /dev/null
	echo "grid on;" | tee -a matfigure.m &> /dev/null
	echo "set(gca, 'FontSize', 16);" | tee -a matfigure.m &> /dev/null
	echo "print -dpng ${NAME}.png" | tee -a matfigure.m &> /dev/null
	echo "clf;" | tee -a matfigure.m &> /dev/null

	# Likelihood
	NAME=${PAR[${pp}]}_likeraw
	echo "scatter(${VECTOR}(:,${p}), exp(${VELIKE}(:,1)), 10, 'filled', 'b');" | tee -a matfigure.m &> /dev/null
	echo "xlabel('${PAR[${pp}]}', 'fontname', 'Times', 'fontsize', 16);" | tee -a matfigure.m &> /dev/null
	echo "ylabel('Likelihood', 'fontname', 'Times', 'fontsize', 16);" | tee -a matfigure.m &> /dev/null
	echo "grid on;" | tee -a matfigure.m &> /dev/null
	echo "set(gca, 'FontSize', 16);" | tee -a matfigure.m &> /dev/null
	echo "print -dpng ${NAME}.png" | tee -a matfigure.m &> /dev/null
	echo "clf;" | tee -a matfigure.m &> /dev/null
done

echo "exit;" | tee -a matfigure.m &> /dev/null

# Run Script
${ARGS[0]} -nodesktop -nosplash -r "matfigure"
