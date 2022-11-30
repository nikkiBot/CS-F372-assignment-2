#include <../header/p1.h>


struct fileReader
{
    int i ;
    int j ;
    int rowToRead ;
    char filename[] ;
};

int rowStore[] ;    //global array to store the row and then put the values on 2d array

/**
 * @brief This function is used to read the file and store the data in the array
 * 
 * @param arg 
 * @return NULLptr
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
            // printf("%s", line) ;
            rowStore[i] = atoi(line) ;
            break ;
        }
        i++ ;
    }
    fclose(fp) ;
    if (line)
        free(line) ;
    return NULL ;
}

/**
 * @brief This function takes in2.txt and then writes the transpose of the matrix in the text file
 * 
 * @param fp, the file pointer to be passed 
 */
void preProcess(FILE *fp){
    
    return ;
}
int main(int argc, char *argv[])
{
    if (argc != 7) {
		printf("Usage: ./groupX_assignment2.out i j k in1.txt in2.txt out.txt -> enter correctly");
		exit(-1);
	}

    FILE *in1 = fopen(argv[4], "r");
    FILE *in2 = fopen(argv[5], "r+"); //write access for preprocessing
    FILE *out = fopen(argv[6], "w+");

    preProcess(in2);
    
    clock_t t ;
    t = clock();

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
    int result[i][k] ;

    struct fileReader matrix1_arg[i] ;
    struct fileReader matrix2_arg[j] ;

    pthread_t tid_mat1[i] ;
    pthread_t tid_mat2[j] ;

    //Creating threads to read rows of in1.txt
    for(int row = 0; row < i ; row++)
    {
        matrix1_arg[row].i = i ;
        matrix1_arg[row].j = j ;
        matrix1_arg[row].rowToRead = row ;
        strcpy(matrix1_arg[row].filename,argv[4]) ;
        pthread_create(&tid_mat1[row],NULL, readMyFile,(void *)&matrix1_arg[row]) ;
    }
    for(int row = 0 ; row < i ; row++)
    {
        pthread_join(tid_mat1[row], NULL) ;
    }

    //Creating threads to read columns of in2.txt
    for(int row = 0; row < i ; row++)
    {
        matrix2_arg[row].i = j ;
        matrix2_arg[row].j = k ;
        matrix2_arg[row].rowToRead = row ;
        strcpy(matrix2_arg[row].filename,argv[5]) ;
        pthread_create(&tid_mat2[row],NULL, readMyFile,(void *)&matrix2_arg[row]) ;
    }
    for(int row = 0 ; row < i ; row++)
    {
        pthread_join(tid_mat2[row], NULL) ;
    }

    t = clock() - t ;
    double timeTaken = ((double)t)/ CLOCKS_PER_SEC ;
    printf("%f", timeTaken) ;
    exit(0);
}