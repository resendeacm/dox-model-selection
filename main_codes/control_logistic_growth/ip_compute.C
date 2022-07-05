//System Libraries
#include <sstream>
#include <string>

//Header Files (QUESO)
#include "queso/GenericVectorFunction.h"
#include "queso/GenericScalarFunction.h"
#include "queso/StatisticalInverseProblem.h"
#include "queso/StatisticalForwardProblem.h"

//Header Files (Source)
#include "fp_qoi.h"
#include "ip_compute.h"
#include "ip_likelihood.h"

///Calibration Process (Bayesian Approach)
void compute_ip(const QUESO::FullEnvironment& env, int argc, char** argv) {
	cout << endl << "#--------------------------------------------------#";
	cout << endl << "|                                                  |";
	cout << endl << "|                 CALIBRATION STEP                 |";
	cout << endl << "|                                                  |";
	cout << endl << "#--------------------------------------------------#" << endl << endl;
	
	//----------------------------------------------------------------------------------------------------------
	// SIP STEP 1 OUT OF 6: Instantiate the parameter space;
	//----------------------------------------------------------------------------------------------------------
	QUESO::GetPot input_data("input_data.in");
	
	unsigned int npar = input_data("num_parameters", 0.0);
	QUESO::VectorSpace<QUESO::GslVector,QUESO::GslMatrix> param_space(env, "param_", npar, NULL);

	//----------------------------------------------------------------------------------------------------------
	// SIP STEP 2 OUT OF 6: Instantiate the parameter domain;
	//----------------------------------------------------------------------------------------------------------
	QUESO::GslVector pmin_values(param_space.zeroVector());
	QUESO::GslVector pmax_values(param_space.zeroVector());
	
	//Parameters
	pmin_values[0] = input_data("n0_min", 0.0);
	pmax_values[0] = input_data("n0_max", 0.0);
	pmin_values[1] = input_data("kp_min", 0.0);
	pmax_values[1] = input_data("kp_max", 0.0);
	pmin_values[2] = input_data("theta_min", 0.0);
	pmax_values[2] = input_data("theta_max", 0.0);
	
	QUESO::BoxSubset<QUESO::GslVector,QUESO::GslMatrix> param_domain("param_", param_space, pmin_values, pmax_values);

	//----------------------------------------------------------------------------------------------------------
	// SIP STEP 3 OUT OF 6: Instantiate the likelihood function object to be used by QUESO;
	//----------------------------------------------------------------------------------------------------------
	
	//Experimentalist File (Real Data)
	//Original Time, Cell Counts, Variance
	string datafile = "dataset.dat";
	double read_time, read_cell, read_var;
	
	//Data Reading
	string line;
	ifstream infile(datafile);
	
	//Data Vectors
	vector<double> time_vector, cell_vector, vari_vector;

	if (infile) {
		while(getline(infile,line)) {
			istringstream(line) >> read_time >> read_cell >> read_var;
			time_vector.push_back(read_time);
			cell_vector.push_back(read_cell);
			vari_vector.push_back(read_var);
		}
		infile.close();
	}
	else
		cout << "ERROR OPENING THE FILE '" << datafile << "'!" << endl;
	
	//Quantity of Interest (QoI) Size
	unsigned int qoi_size = time_vector.size();

	//Likelihood Structure
	likelihoodRoutine_DataType likelihoodRoutine_Data;
	likelihoodRoutine_Data.time_vector = &time_vector;
	likelihoodRoutine_Data.cell_vector = &cell_vector;
	likelihoodRoutine_Data.vari_vector = &vari_vector;
  
	//Routine Computes [ln(function)]
	QUESO::GenericScalarFunction<QUESO::GslVector,QUESO::GslMatrix> likelihood_function_obj("cal_like_", param_domain, likelihood_routine, (void *) &likelihoodRoutine_Data, true);
  
	//----------------------------------------------------------------------------------------------------------
	// SIP STEP 4 OUT OF 6: Define the prior RV.
	//----------------------------------------------------------------------------------------------------------
	QUESO::UniformVectorRV<QUESO::GslVector,QUESO::GslMatrix> prior_rv("cal_prior_", param_domain);

	//----------------------------------------------------------------------------------------------------------
	// SIP STEP 5 OUT OF 6: Instantiate the inverse problem.
	//----------------------------------------------------------------------------------------------------------
	
	//Extra prefix before the default "rv_" prefix
	QUESO::GenericVectorRV<QUESO::GslVector,QUESO::GslMatrix> post_rv("cal_post_", param_space);

	//No extra prefix before the default "ip_" prefix
	QUESO::StatisticalInverseProblem<QUESO::GslVector,QUESO::GslMatrix> ip("cycle_cal_", NULL, prior_rv, likelihood_function_obj, post_rv);

	//----------------------------------------------------------------------------------------------------------
	// SIP STEP 6 OUT OF 6: Solve the inverse problem (set the 'pdf' and the 'realizer' of the posterior RV).
	//----------------------------------------------------------------------------------------------------------
	ip.solveWithBayesMLSampling();


	cout << endl << "#---------------------------------------------------#";
	cout << endl << "|                                                   |";
	cout << endl << "|            STATISTICAL FORWARD PROBLEM            |";
	cout << endl << "|                                                   |";
	cout << endl << "#---------------------------------------------------#" << endl << endl;
	
	//----------------------------------------------------------------------------------------------------------
	// SFP STEP 1 OUT OF 6: Instantiate the parameter QoI space;
	//----------------------------------------------------------------------------------------------------------
	
	QUESO::VectorSpace<> qoi_space(env, "qoi_", qoi_size, NULL);
	
	//----------------------------------------------------------------------------------------------------------
	// SFP STEP 2 OUT OF 6: Instantiate the parameter domain;
	//----------------------------------------------------------------------------------------------------------

	//Not necessary because SFP's input RV is equal SIP's output RV (post_rv)
	
	//----------------------------------------------------------------------------------------------------------
	// SFP STEP 3 OUT OF 6: Instantiate the QoI object to be used by QUESO;
	//----------------------------------------------------------------------------------------------------------
	
	qoiRoutine_DataType qoiRoutine_Data;
	qoiRoutine_Data.time_vector = &time_vector;
	qoiRoutine_Data.cell_vector = &cell_vector;
	qoiRoutine_Data.vari_vector = &vari_vector;

	QUESO::GenericVectorFunction<QUESO::GslVector,QUESO::GslMatrix,QUESO::GslVector,QUESO::GslMatrix> qoi_function_obj("qoi_", param_domain, qoi_space, qoi_routine, (void *) &qoiRoutine_Data);
	
	//----------------------------------------------------------------------------------------------------------
	// SFP STEP 4 OUT OF 6: Define the input RV;
	//----------------------------------------------------------------------------------------------------------
	
	//Not necessary because SFP's input RV is equal SIP's output RV (post_rv)

	//----------------------------------------------------------------------------------------------------------
	// SFP STEP 5 OUT OF 6: Instantiate the forward problem;
	//----------------------------------------------------------------------------------------------------------
	QUESO::GenericVectorRV<> qoi_rv("qoi_", qoi_space);
	QUESO::StatisticalForwardProblem<> fp("", NULL, post_rv, qoi_function_obj, qoi_rv);

	//----------------------------------------------------------------------------------------------------------
	// SFP STEP 6 OUT OF 6: Solve the forward problem.
	//----------------------------------------------------------------------------------------------------------
	fp.solveWithMonteCarlo(NULL);
}
