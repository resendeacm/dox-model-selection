#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float** malloc_matrix(int rows, int cols);
void free_matrix(float **matrix, int rows, int cols);

int main(int argc, char** argv) {
	//Samples & Parameters
	unsigned int samples = atoi(argv[1]);
	unsigned int parameters = atoi(argv[2]);
	
	//Allocation (Posterior Matrix & Most Probable Values Vector)
	float **pmatrix = malloc_matrix(samples, parameters);
	float *mpvalues = (float*) malloc(parameters * sizeof(float));
	
	//Files
	FILE *pfile = fopen("posterior.log", "r");
	FILE *mfile = fopen("mpvs.out", "r");
	FILE *afile = fopen("mpvs.out", "a+");
	
	//File Reading
	for(unsigned int i = 0; i < samples; i++) {
		for(unsigned int j = 0; j < parameters; j++) {
			if(j < parameters - 1)
				fscanf(pfile, "%e\t", &pmatrix[i][j]);
			else
				fscanf(pfile, "%e\n", &pmatrix[i][j]);
		}
	}
	
	//Most Probable Values
	for(unsigned int j = 0; j < parameters; j++)
		fscanf(mfile, "%e\t", &mpvalues[j]);
	
	//Standard Deviation: Discrete Random Variable
	for(unsigned int j = 0; j < parameters; j++) {
		//New Calculation
		float result = 0;
		
		//STD
		for(unsigned int i = 0; i < samples; i++)
			result += pow(pmatrix[i][j] - mpvalues[j], 2);
		
		//Append Newline
		if(j == 0)
			fprintf(afile, "\n%e\t", sqrt(result/samples));
		else
			fprintf(afile, "%e\t", sqrt(result/samples));
	}
	
	fclose(pfile);
	fclose(mfile);
	fclose(afile);
	free(mpvalues);
	free_matrix(pmatrix, samples, parameters);
	return 0;
}

//Matrix Allocation
float** malloc_matrix(int rows, int cols) {
	//Rows
	float **matrix = (float**) malloc(rows * sizeof(float*));
	
	//Columns
	for (unsigned int i = 0; i < rows; i++)
		matrix[i] = (float*) malloc(cols * sizeof(float));

	return (matrix);
}

//Matrix Deallocation
void free_matrix(float **matrix, int rows, int cols) {
	for (unsigned int i = 0; i < rows; i++)
		free(matrix[i]);

	free(matrix);
}
