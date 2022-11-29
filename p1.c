#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdbool.h>

struct fileReader
{
    int i ;
    int j ;
    int rowToRead ;
    char filename[] ;
};

/**
 * @brief This function is used to read the file and store the data in the array
 * 
 * @param arg 
 * @return NULL
 */
void *readMyFile(void *arg)
{
    struct fileReader *fr = (struct fileReader *)arg ;
    FILE *fp = fopen(fr->filename, "r") ;
    char *line = NULL ;
    size_t len = 0 ;
    ssize_t read ;
    int i = 0 ;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (i == fr->rowToRead)
        {
            printf("%s", line) ;
            break ;
        }
        i++ ;
    }
    fclose(fp) ;
    if (line)
        free(line) ;
    return NULL ;
}

int rowStore[] ;    //global array to store the row and then put the values on 2d array

/**
 * @brief This function is used to read a single row from the file input
 * 
 * @param argument struct
 * @return NULL 
 */
void* readFile(void *arg)
{
    struct fileReader *reader = (struct fileReader *)arg;
    FILE *in1 = fopen(&(reader.filename),"r");
    for(int row = 0; row < reader->i ; row++ )
    {
        fscanf(in1,"%d",rowStore[row]) ;
    }
}


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

    struct fileReader matrix1_arg ;
    matrix1_arg.i = i ;
    matrix1_arg.j = j ;
    matrix1_arg.filename[7] = "in1.txt" ;
    matrix1_arg.rowToRead = 0;

    struct fileReader matrix2_arg ;
    matrix2_arg.i = j ;
    matrix2_arg.j = k ;
    matrix2_arg.filename[7] = "in2.txt" ;
    matrix2_arg.rowToRead = 0;


    // for(int rows = 0 ; rows < i ; rows++)
    //     for(int cols = 0 ; cols < j ; cols++)
    //         fscanf(in1,"%d",&matrix1[rows][cols]);

    // for(int rows = 0 ; rows < j ; rows++)
    //     for(int cols = 0 ; cols < k ; cols++)
    //         fscanf(in2,"%d",&matrix2[rows][cols]);

    
}