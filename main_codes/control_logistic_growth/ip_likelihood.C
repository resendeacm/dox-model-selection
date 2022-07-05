//System Libraries
#include <cmath>

//Header Files (QUESO)
#include "queso/StatisticalInverseProblem.h"

//Header Files (Source)
#include "main_model.h"
#include "ip_likelihood.h"

///Gaussian Likelihood Routine
double likelihood_routine(const QUESO::GslVector& param_values, const QUESO::GslVector* param_direction, const void* likelihood_data_ptr, QUESO::GslVector* grad_vector, QUESO::GslMatrix* hessian_matrix, QUESO::GslVector* hessian_effect) {
	//Model File (#Parameters, Priors)
	QUESO::GetPot input_data("input_data.in");

	//Calibration Parameters
	unsigned int npar = input_data("num_parameters", 0.0);
	UQ_FATAL_TEST_MACRO(param_values.sizeGlobal() != npar, QUESO::UQ_UNAVAILABLE_RANK, "likelihoodRoutine()", "wrong size of vector param_values");

	//Parameters
	vector<double> parameters(npar,0), model_output;
	for (unsigned int i = 0; i < parameters.size(); i++)
		parameters[i] = param_values[i];

	//Time Vector
	const vector<double> time_vector = *((likelihoodRoutine_DataType*) likelihood_data_ptr)->time_vector;
	
	//Forward Problem (Output)
	main_code(model_output, parameters, time_vector);

	//Gaussian Likelihood
	double result = 0.0;
	const QUESO::BaseEnvironment& env = param_values.env();

	if (env.subRank() == 0) {
		//Cell Counts & Variance
		const vector<double> cell_vector = *((likelihoodRoutine_DataType*) likelihood_data_ptr)->cell_vector;
		const vector<double> vari_vector = *((likelihoodRoutine_DataType*) likelihood_data_ptr)->vari_vector;
		
		for (unsigned int i = 0; i < model_output.size(); ++i) {
			//Comment this line if 'variance' is a model hyperparameter (a parameter that also needs to be calibrated)
			//Uncomment this line if 'variance' is given in the experimentalist's file
			double variance = pow(vari_vector[i], 2);
			double std_devs = vari_vector[i];
			
			//Comment this line if 'variance' is given in the experimentalist's file
			//Uncomment this line if 'variance' is a model hyperparameter (a parameter that also needs to be calibrated)
			//Important note: remember that if 'variance' needs calibration, it has to be the last parameter on the parameters list
			//double variance = pow(parameters[parameters.size() - 1], 2);
			//double std_devs = parameters[parameters.size() - 1];
			
			//Gaussian Likelihood
			double ratio = (model_output[i] - cell_vector[i]);
			result += (ratio*ratio/(0.5*variance) + log(std_devs) - 0.5*log(2.*M_PI));
		}
	}

	return -1.0*result;
}
