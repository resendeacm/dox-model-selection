//System Libraries
#include <cmath>

//Header Files (QUESO)
#include "queso/StatisticalInverseProblem.h"

//Header Files (Source)
#include "main_model.h"

///Main Code (Forward Problem)
void main_code(vector<double>& output, vector<double> parameters, vector<double> times) {	
	//Time Step, Last Step & Iterator
	double time_step = times[2] - times[1];
	double last_step = times[times.size() - 1];
	unsigned int tstep = 0;
	
	//Quantity of Interest (QoI) Size
	unsigned int time_size = times.size();
	
	//ODE System
	unsigned int eqnum = 1;
	vector<double> result(eqnum,0.0);

	//Initial Condition
	unsigned int num_timesteps = (last_step*100.0 - times[0]*100.0)/(time_step*100.0);
	result[0] = parameters[0];
	
	//Initial Condition: Cell Count Result
	output.push_back(result[0]);
	
	//Save Results of Specific Iterations (Excludes Initial Condition)
	vector<unsigned int> save_iter(time_size - 1,0);
	for(unsigned int i = 0; i < time_size - 1; i++)
		save_iter[i] = (times[i + 1]*100.0 - times[0]*100.0)/(time_step*100.0);
	
	//ODE Solver
	do {
		//Prepare Timestep
		tstep++;
		double tum_time = times[0] + tstep*time_step;
		
		//Runge-Kutta (4th Order)
		runge_kutta(result, time_step, parameters, tum_time);

		//Post-Processing
		int print_output = 0;
		for(unsigned int i = 0; i < save_iter.size(); i++)
			if(save_iter[i] == tstep)
				print_output = 1;
		
		//Tumor Response (Cell Counts)
		if(print_output)
			output.push_back(result[0]);
	
	} while(tstep < num_timesteps);
}

///ODE Approximation Method: Runge-Kutta (4th Order)
void runge_kutta(vector<double>& result, double time_step, vector<double> parameters, double time) {
	const unsigned int result_size = result.size();
	vector<double> K(4*result_size,0.0);
	vector<double> qoi(result_size,0.0);
	vector<double> aux(result_size,0.0);

	//Initialization
	for(unsigned int i = 0; i < result_size; i++)
		qoi[i] = result[i];
	
	//K1
	fmodel(qoi, aux, parameters, time);
	for(unsigned int i = 0; i < result_size; i++) {
		K[i*4 + 0] = time_step*aux[i];
		qoi[i] = result[i] + K[i*4 + 0]/2.0;
	}	

	//K2
	fmodel(qoi, aux, parameters, time);
	for(unsigned int i = 0; i < result_size; i++) {
		K[i*4 + 1] = time_step*aux[i];
		qoi[i] = result[i] + K[i*4 + 1]/2.0;
	}
	
	//K3
	fmodel(qoi, aux, parameters, time);
	for(unsigned int i = 0; i < result_size; i++) {
		K[i*4 + 2] = time_step*aux[i];
		qoi[i] = result[i] + K[i*4 + 2];
	}
	
	//K4
	fmodel(qoi, aux, parameters, time);
	for(unsigned int i = 0; i < result_size; i++)
		K[i*4 + 3] = time_step*aux[i];

	//RK4: (1.0/6.0)*(K1 + 2.0*(K2 + K3) + K4)
	for(unsigned int i = 0; i < result_size; i++)
		result[i] = result[i] + (1.0/6.0)*(K[i*4 + 0] + 2*(K[i*4 + 1] + K[i*4 + 2]) + K[i*4 + 3]);
}

///ODE'S Right Hand Sides
void fmodel(vector<double> qoi, std::vector<double>& auxiliar, vector<double> parameters, double time) {
	double kp = parameters[1];    //Control Calibration: Logistic Growth
	double theta = parameters[2]; //Carrying Capacity

	auxiliar[0] = kp*qoi[0]*(1.0 - qoi[0]/theta);
}
