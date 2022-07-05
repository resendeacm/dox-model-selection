//Header Files (QUESO)
#include "queso/StatisticalInverseProblem.h"

//Header Files (Source)
#include "fp_qoi.h"
#include "main_model.h"

///Quantity of Interest (QoI) Routine
void qoi_routine(const QUESO::GslVector& param_values, const QUESO::GslVector* param_direction, const void* qoi_data_ptr, QUESO::GslVector& qoi_values, QUESO::DistArray<QUESO::GslVector*>* grad_vectors, QUESO::DistArray<QUESO::GslMatrix*>* hessian_matrices, QUESO::DistArray<QUESO::GslVector*>* hessian_effects) {
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
	const vector<double> time_vector = *((qoiRoutine_DataType*) qoi_data_ptr)->time_vector;

	//Forward Problem (Output)
	main_code(model_output, parameters, time_vector);

	//Results
	const QUESO::BaseEnvironment& env = param_values.env();

	//Quantity of Interest (QoI) Results
	if (env.subRank() == 0) {
		for (unsigned int i = 0; i < model_output.size(); i++)
			qoi_values[i] = model_output[i];
	}

}
