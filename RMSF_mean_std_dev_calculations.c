#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

//Code for computing the mean and standard deviation of the RMSF of a protein-protein interface over a MD simulation

//=============Parameters you might want to change===============

#define RES   (24)

//end to parameters you might want to change=====================


//==============Directories you might want to change=============
char workingdir[]="/scratch/rloureiro/COMPLEXES_MD/5nt2_NS1-TRIM25/run-01/";
char writingdir[]="/scratch/rloureiro/COMPLEXES_MD/5nt2_NS1-TRIM25/run-01/";
char rootdir[]="/scratch/rloureiro";
char ss[]="5nt2_NS1-TRIM25/run-01";
//==============================================================

double RMSF_sum;
double RMSF_mean;
double RMSF[RES];
double square_diff_RMSF[RES];
double sum_square_diff_RMSF;
double variance_RMSF;
double std_dev_RMSF;
double std_error_mean_RMSF;

void READ_RMSF_values(void);

int main(void)
{
	
	FILE *output;
	
	int i,j;

	chdir(workingdir);
	
	READ_RMSF_values();

        RMSF_sum = 0.0;

        RMSF_mean = 0.0;

        for(i = 0; i < RES; i++)
        {
                RMSF_sum = RMSF_sum + RMSF[i];
        }

	RMSF_mean = RMSF_sum/(double)RES;
		
	sum_square_diff_RMSF = 0.0;
	
	for(i = 0; i < RES; i++)
	{
		square_diff_RMSF[i] =(RMSF[i] - RMSF_mean) * (RMSF[i] - RMSF_mean);
		sum_square_diff_RMSF = sum_square_diff_RMSF + square_diff_RMSF[i];
	}
	
	variance_RMSF = sum_square_diff_RMSF/(double)RES;
	std_dev_RMSF = sqrt(variance_RMSF);
	std_error_mean_RMSF = std_dev_RMSF/sqrt((double)RES);
	
	chdir(writingdir);
	
	output = fopen("rmsf_per_residue_TRIM25_interface_mean_std_dev.dat", "w");
	
	fprintf(output,"%lf      %lf      %lf\n", RMSF_mean, std_dev_RMSF, std_error_mean_RMSF);
		
	return 0;
}


void READ_RMSF_values(void)
{
	FILE *input_RMSF_values;
	
	int j, r;
	double t, rmsf;
	char s[64];
	
	sprintf(s, "rmsf_per_residue_TRIM25_interface.dat");
		
	input_RMSF_values = fopen(s, "r");
		
	if(input_RMSF_values == NULL)	
	{
		printf("Fail to open dat file: %s\nQuit.\n",s);
		exit(0);
	}
	
	j = 0;
	while(!feof(input_RMSF_values))
	{
		if (fscanf(input_RMSF_values, "%lf   %lf", &t, &rmsf) > 0)
		{
			if (1)
			{
				RMSF[j] = rmsf;
				j++;
			}
		}
	}
			
	fclose(input_RMSF_values);
}
