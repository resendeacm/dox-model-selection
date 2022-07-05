#ifndef FILE_IP_COMPUTE_H
#define FILE_IP_COMPUTE_H

//Header Files (QUESO)
#include "queso/Environment.h"

using namespace std;

///Calibration Process (Bayesian Approach)
void compute_ip(const QUESO::FullEnvironment& env, int argc, char** argv);

#endif
