#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

//Code for computing the mean RMSD and the corresponding standard error of a protein-protein complex over a MD simulation

//=============Parameters you might want to change===============

#define CONF   (1001)

//end to parameters you might want to change=====================


//==============Directories you might want to change=============
char workingdir[]="/scratch/rloureiro/COMPLEXES_MD/5nt2_NS1-TRIM25/run-01/";
char writingdir[]="/scratch/rloureiro/COMPLEXES_MD/5nt2_NS1-TRIM25/run-01/";
char rootdir[]="/scratch/rloureiro";
char ss[]="5nt2_NS1-TRIM25/run-01";
//==============================================================

double RMSD_sum;
double RMSD_mean;
double RMSD[CONF];
double square_diff_RMSD[CONF];
double sum_square_diff_RMSD;
double variance_RMSD;
double std_dev_RMSD;
double std_error_mean_RMSD;

void READ_RMSD_values(void);

int main(void)
{
	
	FILE *output;
	
	int i,j;

	chdir(workingdir);
	
	READ_RMSD_values();

        RMSD_sum = 0.0;

        RMSD_mean = 0.0;

        for(i = 0; i < CONF; i++)
        {
                RMSD_sum = RMSD_sum + RMSD[i];
        }

	RMSD_mean = RMSD_sum/(double)CONF;
		
	sum_square_diff_RMSD = 0.0;
	
	for(i = 0; i < CONF; i++)
	{
		square_diff_RMSD[i] =(RMSD[i] - RMSD_mean) * (RMSD[i] - RMSD_mean);
		sum_square_diff_RMSD = sum_square_diff_RMSD + square_diff_RMSD[i];
	}
	
	variance_RMSD = sum_square_diff_RMSD/(double)CONF;
	std_dev_RMSD = sqrt(variance_RMSD);
	std_error_mean_RMSD = std_dev_RMSD/sqrt((double)CONF);
	
	chdir(writingdir);
	
	output = fopen("Ca_rmsd_NS1-ED-TRIM25_complex_mean_std_dev.dat", "w");
	
	fprintf(output,"%lf      %lf      %lf\n", RMSD_mean, std_dev_RMSD, std_error_mean_RMSD);
		
	return 0;
}



void READ_RMSD_values(void)
{
	FILE *input_RMSD_values;
	
	int j, r;
	double t, rmsd;
	char s[64];
	
	sprintf(s, "Ca_rmsd_NS1-ED-TRIM25_complex.dat");
		
	input_RMSD_values = fopen(s, "r");
		
	if(input_RMSD_values == NULL)	
	{
		printf("Fail to open dat file: %s\nQuit.\n",s);
		exit(0);
	}
	
	j = 0;
	while(!feof(input_RMSD_values))
	{
		if (fscanf(input_RMSD_values, "%lf   %lf", &t, &rmsd) > 0)
		{
			if (1)
			{
				RMSD[j] = rmsd;
				j++;
			}
		}
	}
			
	fclose(input_RMSD_values);
}
