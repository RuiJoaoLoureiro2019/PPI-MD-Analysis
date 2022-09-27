#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

//Code for computing the mean interfacial area and the corresponding standard error of a protein-protein complex over a MD simulation

//=============Parameters you might want to change===============

#define CONF   (1001)

//end to parameters you might want to change=====================


//==============Directories you might want to change=============
char workingdir[]="/scratch/rloureiro/COMPLEXES_MD/5nt2_NS1-TRIM25/run-01/";
char writingdir[]="/scratch/rloureiro/COMPLEXES_MD/5nt2_NS1-TRIM25/run-01/";
char rootdir[]="/scratch/rloureiro";
char ss[]="5nt2_NS1-TRIM25/run-01";
//==============================================================

double SASA_sum;
double SASA_mean;
double SASA[CONF];
double square_diff_SASA[CONF];
double sum_square_diff_SASA;
double variance_SASA;
double std_dev_SASA;
double std_error_mean_SASA;

void READ_SASA_values(void);

int main(void)
{

        FILE *output;

        int i,j;

        chdir(workingdir);

        READ_SASA_values();

        SASA_sum = 0.0;

        SASA_mean = 0.0;

        for(i = 0; i < CONF; i++)
        {
                SASA_sum = SASA_sum + SASA[i];
        }

        SASA_mean = SASA_sum/(double)CONF;

        sum_square_diff_SASA = 0.0;

        for(i = 0; i < CONF; i++)
        {
                square_diff_SASA[i] =(SASA[i] - SASA_mean) * (SASA[i] - SASA_mean);
                sum_square_diff_SASA = sum_square_diff_SASA + square_diff_SASA[i];
        }

        variance_SASA = sum_square_diff_SASA/(double)CONF;
        std_dev_SASA = sqrt(variance_SASA);
        std_error_mean_SASA = std_dev_SASA/sqrt((double)CONF);

        chdir(writingdir);

        output = fopen("buried_SASA_NS1-TRIM25_mean_std_dev.dat", "w");

        fprintf(output,"%lf      %lf      %lf\n", SASA_mean, std_dev_SASA, std_error_mean_SASA);

        return 0;
}


void READ_SASA_values(void)
{
        FILE *input_SASA_values;

        int j, t;
        double sasa;
        char s[64];

        sprintf(s, "buried_SASA_NS1-TRIM25.dat");

        input_SASA_values = fopen(s, "r");

        if(input_SASA_values == NULL)
        {
                printf("Fail to open dat file: %s\nQuit.\n",s);
                exit(0);
        }

        j = 0;
        while(!feof(input_SASA_values))
        {
                if (fscanf(input_SASA_values, "%lf", &sasa) > 0)
                {
                        if (1)
                        {
                                SASA[j] = sasa;
                                j++;
                        }
                }
        }

        fclose(input_SASA_values);
}
