#ifndef FILE_FP_QOI_H
#define FILE_FP_QOI_H

//Header Files (QUESO)
#include "queso/GslMatrix.h"
#include "queso/DistArray.h"

using namespace std;

struct qoiRoutine_DataType {
	//Time, Cell Counts & Variance
	const vector<double>* time_vector;
	const vector<double>* cell_vector;
	const vector<double>* vari_vector;
};

///Quantity of Interest (QoI) Routine
void qoi_routine(const QUESO::GslVector& param_values, const QUESO::GslVector* param_direction, const void* qoi_data_ptr, QUESO::GslVector& qoi_values, QUESO::DistArray<QUESO::GslVector*>* grad_vectors, QUESO::DistArray<QUESO::GslMatrix*>* hessian_matrices, QUESO::DistArray<QUESO::GslVector*>* hessian_effects);

#endif
