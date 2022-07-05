#ifndef FILE_IP_LIKELIHOOD_H
#define FILE_IP_LIKELIHOOD_H

//Header Files (QUESO)
#include "queso/GslMatrix.h"

using namespace std;

struct likelihoodRoutine_DataType {
	//Time, Cell Counts & Variance
	const vector<double>* time_vector;
	const vector<double>* cell_vector;
	const vector<double>* vari_vector;
};

///Gaussian Likelihood Routine
double likelihood_routine(const QUESO::GslVector& param_values, const QUESO::GslVector* param_direction, const void* likelihood_data_ptr, QUESO::GslVector* grad_vector, QUESO::GslMatrix* hessian_matrix, QUESO::GslVector* hessian_effect);

#endif
