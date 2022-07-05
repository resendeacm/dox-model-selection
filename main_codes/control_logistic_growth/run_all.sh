#!/bin/bash
clear
rm ./Output/* mlike.out mpvs.out *~ *# &> /dev/null

# MATLAB Path
MATLAB=/usr/local/MATLAB/R2017a/bin/matlab

# Calibration (Bayesian Approach)
make run

# Levels & Evidence
DISPLAY=`ls ./Output/display* |tr -s ' ' '\n' | awk NR==1`
LEVELS=`grep currExponent ${DISPLAY} | wc -l`
EVIDENCE=`grep "evidence =" ${DISPLAY} | tr -s ',' '\n' | grep "evidence =" | cut -d = -f2 | awk NR==1 | awk '{printf "%.2e\n", $1 + 0}'`
echo "Evidence ("${LEVELS} " LEVELS): " ${EVIDENCE}

# Maximum Likelihood & Most Probable Values
${MATLAB} -nodesktop -nojvm -nosplash -r "maxlike(${LEVELS}); exit" &> /dev/null
LOGLIKE=`cat mlike.out`

# Standard Deviation: Discrete Random Variable
gcc std_rand.c -o exec_std -lm && ./exec_std 3000 3

# Solution, Histogram & Likelihood
echo " "
echo "[MATLAB GRAPHICS]"
cd ./Output/
./../solution.sh ${MATLAB} ./../dataset.dat ./../dataplot.dat
./../likeplot.sh ${MATLAB} ${LEVELS}

# Remove Compilation Files
rm models_ode_sifp *.depend *.o job_output.txt exec_std *~ *# &> /dev/null

exit
