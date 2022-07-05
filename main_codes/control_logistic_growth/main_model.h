#ifndef FILE_MAIN_MODEL_H
#define FILE_MAIN_MODEL_H

//System Libraries
#include <vector>

using namespace std;

///Main Code (Forward Problem)
void main_code(vector<double>& output, vector<double> parameters, vector<double> times);

///ODE Approximation Method: Runge-Kutta (4th Order)
void runge_kutta(vector<double>& QoI, double time_step, vector<double> parameters, double time);

///ODE'S Right Hand Sides
void fmodel(vector<double> qoi, vector<double>& auxiliar, vector<double> parameters, double time);

#endif
