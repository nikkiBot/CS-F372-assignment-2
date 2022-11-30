// group25_s1_22/
// >>>> file: group25_assignment1.c
/* Do not make any changes in the skeleton. Your submission will be invalidated if the skeleton is changed */
 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
 
// Thread Limit
int count_thread = 0;
 
// can be made local if required
int N;
 
// Possible movements
// other
 
int dx[6][8] = {{1, 1, 2, 2, -1, -1, -2, -2}, {-2, -2, -1, -1, 1, 1, 2, 2}, {2, 2, 1, 1, -1, -1, -2, -2}, {-1, 1, -2, 2, -2, 2, -1, 1}, {1, -1, 2, -2, 2, -2, 1, -1}, {2, 1, 2, 1, -2, -1, -2, -1}};
// other
 
int dy[6][8] = {{2, -2, 1, -1, 2, -2, 1, -1}, {1, -1, 2, -2, 2, -2, 1, -1}, {-1, 1, -2, 2, -2, 2, -1, 1}, {2, 2, 1, 1, -1, -1, -2, -2}, {-2, -2, -1, -1, 1, 1, 2, 2}, {1, 2, -1, -2, 1, 2, -1, -2}};
// other
 
 
// Next Set Variable
int nextSet = -1;
 
// Check Variable
int isFound = 0;
 
// Helper struct, can utilize any other custom structs if needed 
typedef struct{
	int x,y;
} pair;
 
// Argument Struct
typedef struct{
    int x, y;
    int board[50][50];
    pair path[2500];
    int level;
} struct_arg;
 
// Priority Struct
typedef struct{
    int x, y;
    int access;
} access_index;
 
// Final answer
struct_arg res;
 
// Not necessary to use this function as long as same printing pattern is followed 
void print_path(pair path[], int n){
	for(int i=0;i<n;i++){
		printf("%d,%d|",path[i].x,path[i].y);
	}
}
 
// Sort the next given points
void sort(access_index points[8]) {
    for(int i=0; i<7; i++) {
        access_index temp;
 
        for(int j=0; j<8-i-1; j++) {
            if(points[j].access > points[j+1].access) {
                temp = points[j];
                points[j] = points[j+1];
                points[j+1] = temp;
            }
        }
    }
}
 
// Check the Accessibility of the Point
access_index checkAccess(int x, int y, int board[50][50], int level) {
    int count = 0;
    int nextX = -1;
    int nextY = -1;
 
    srand(time(0));
 
    int startIndex = rand() % 8;
 
    for(int i=startIndex; i-startIndex<8; i++) {
        if(nextSet == -1) {
            nextX = x + dx[0][i-startIndex];
            nextY = y + dy[0][i-startIndex];
        }
        else {
            nextX = x + dx[nextSet][i%8];
            nextY = y + dy[nextSet][i%8];
        }
 
        if(nextX >= 0 && nextX < N && nextY >= 0 && nextY < N && board[nextX][nextY] == 0) {
            count++;
        }
    }
 
    access_index pt;
 
    pt.x = x;
    pt.y = y;
    pt.access = count;
 
    if((level+2 != (N*N)) && (count == 0)) {
        pt.access = 9;
    }
 
    return pt;
}
 
// Iterative Warnsdorff
void* findPath(void *argument) {
    if(argument == NULL) {
        pthread_exit(NULL);
        return;
    }
 
    if(isFound == 1) {
        pthread_exit(NULL);
        return;
    }
 
    struct_arg *arg_ptr = (struct_arg*) argument;
    struct_arg arg = *arg_ptr; 
 
    for(int i=arg.level; i<=N*N; i++) {
        arg.board[arg.x][arg.y] = 1;
        arg.path[i].x = arg.x;
        arg.path[i].y = arg.y;
 
        access_index points[8];
 
        srand(time(0));
 
        int startIndex = rand() % 8;
 
        for(int j=startIndex; j-startIndex<8; j++) {
            int nextX = arg.x + dx[nextSet][j%8];
            int nextY = arg.y + dy[nextSet][j%8];
 
            if(nextX >= 0 && nextX < N && nextY >= 0 && nextY < N && arg.board[nextX][nextY] == 0) {
                points[j-startIndex] = checkAccess(nextX, nextY, arg.board, arg.level);
            }
            else {
                points[j-startIndex].x = nextX;
                points[j-startIndex].y = nextY;
                points[j-startIndex].access = 9;
            }
        }
 
        access_index point;
        point.x = 51;
        point.y = 51;
        point.access = 9;
 
        for(int j=0; j<8; j++) {
            if(points[j].access < point.access) {
                point = points[j];
            }
        }

        if(point.access != 9) {
            arg.x = point.x;
            arg.y = point.y;
            arg.level++;
        }
        else {
            break;
        }
    }
 
    if(arg.level+1 == (N*N)) {
        res = arg;
        isFound = 1;
    }
}
 
// Recursive Backtracking + Warnsdorff 
void* findPathBacktrack(void* argument) {
    if(argument == NULL) {
        return;
    }
 
    if(isFound == 1) {
        return;
    }
 
    struct_arg *arg_ptr = (struct_arg*) argument;
    struct_arg arg = *arg_ptr; 
 
    arg.path[arg.level].x = arg.x;
    arg.path[arg.level].y = arg.y;
    arg.board[arg.x][arg.y] = 1;
 
    if(arg.level+1 == (N*N)) {
        res = arg;
 
        isFound = 1;
        return;
    }
 
    int x = arg.x;
    int y = arg.y;
 
    arg.level = arg.level+1;
 
    access_index points[8];
 
    int count = 0;
 
    for(int i=0; i<8; i++) {
        int nextX = x + dx[0][i];
        int nextY = y + dy[0][i];
 
        if(nextX >= 0 && nextX < N && nextY >= 0 && nextY < N && arg.board[nextX][nextY] == 0) {
            points[i] = checkAccess(nextX, nextY, arg.board, arg.level-1);
            count++;
        }
        else {
            points[i].x = 51;
            points[i].y = 51;
            points[i].access = 9;
        }
    }
 
    sort(points);
 
    if(isFound == 0) {
        for(int i=0; i<8; i++) {
            if(points[i].access != 9) {
                arg.x = points[i].x;
                arg.y = points[i].y;
 
                count--;
 
                if(count == 0 || count_thread > 100000) 
                    findPathBacktrack(&arg);
                else {
                    count_thread++;
                    pthread_t tid;
 
                    pthread_create(&tid, NULL, findPathBacktrack, &arg);
                    pthread_join(tid, NULL);
                }
            }
        }
    }
 
    if(isFound == 0) {
        arg.x = x;
        arg.y = y;
        arg.board[x][y] = 0;
 
        arg.level = arg.level - 1;
        arg.path[arg.level].x = -1;
        arg.path[arg.level].y = -1;
    }
 
    return;
}
 
int main(int argc, char *argv[]) {
	
	if (argc != 4) {
		printf("Usage: ./Knight.out grid_size StartX StartY");
		exit(-1);
	}
	
	N = atoi(argv[1]);
	int StartX=atoi(argv[2]);
	int StartY=atoi(argv[3]);
 
	/* Do your thing here */
    
    if((N%2 == 1 && (StartX + StartY)%2 == 1 )|| (N < 5 && N != 1)) {
        printf("No Possible Tour");
    }
    else {
        struct_arg arg;
 
        arg.x = StartX;
        arg.y = StartY;
 
        arg.level = 0;
 
        for(int i=0; i<50; i++) {
            for(int j=0; j<50; j++) {
                arg.board[i][j] = 0;
            }
        }
 
        for(int i=0; i<2500; i++) {
            arg.path[i].x = -1;
            arg.path[i].y = -1;
            res.path[i].x = -1;
            res.path[i].y = -1;
        }
 
        if(N > 5) {
            pthread_t tid;
 
            while(isFound == 0) {
 
                nextSet = nextSet % 6;
 
                count_thread++;
 
                pthread_create(&tid, NULL, &findPath, &arg);
                pthread_join(tid, NULL);
 
                if(count_thread > 5713) {
                    break;
                }
 
                nextSet++;                    
            }
        }
        else {
            pthread_t tid;
 
            pthread_create(&tid, NULL, &findPathBacktrack, &arg);
            pthread_join(tid, NULL);
        }
 
        if(isFound == 0) {
            printf("No Possible Tour");
        }
        else {
            print_path(res.path, N*N);
        }
    }
 
	return 0;
}