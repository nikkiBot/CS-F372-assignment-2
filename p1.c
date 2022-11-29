#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>


int main(int argc, char *argv[])
{
    if (argc != 7) {
		printf("Usage: ./groupX_assignment2.out i j k in1.txt in2.txt out.txt -> enter correctly");
		exit(-1);
	}

    FILE *in1 = fopen(argv[4], "r");
    FILE *in2 = fopen(argv[5], "r");
    FILE *out = fopen(argv[6], "w+");

    if(in1 == NULL || in2 == NULL || out == NULL){
        perror("fopen");
        exit(EXIT_FAILURE);
    }   
    // in1 -> i x j matrix , in2 -> j x k matrix
    int i = atoi(argv[1]);
    int j = atoi(argv[2]);
    int k = atoi(argv[3]);

    int matrix1[i][j] ;
    int matrix2[j][k] ;

    for(int rows = 0 ; rows < i ; rows++)
        for(int cols = 0 ; cols < j ; cols++)
            fscanf(in1,"%d",&matrix1[rows][cols]);

    for(int rows = 0 ; rows < j ; rows++)
        for(int cols = 0 ; cols < k ; cols++)
            fscanf(in2,"%d",&matrix2[rows][cols]);

    
}