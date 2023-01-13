#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define WIDTH 7
#define HEIGHT 7
#define SMALLWIDTH 4
#define SMALLHEIGHT 4
#define NTHREADS 49

int thread_args[NTHREADS];
pthread_t thread [NTHREADS];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int bigMatrix[HEIGHT][WIDTH] = {
    {17,11,17,1,14,17,16},
    {19,20,3,17,4,14,16},
    {3,20,9,19,15,7,4},
    {19,4,19,14,1,20,10},
    {13,20,16,16,6,1,9},
    {2,20,20,15,1,9,13},
    {6,10,14,8,2,8,15}
};
 
int smallMatrix[4][4] = {
    {1,0,0,0},
    {0,1,0,0},
    {0,0,1,0},
    {0,0,0,1}
};

int resMatrix[HEIGHT][WIDTH] = {
    {-1,-1,-1,-1,-1,-1,-1},
    {-1,-1,-1,-1,-1,-1,-1},
    {-1,-1,-1,-1,-1,-1,-1},
    {-1,-1,-1,-1,-1,-1,-1},
    {-1,-1,-1,-1,-1,-1,-1},
    {-1,-1,-1,-1,-1,-1,-1},
    {-1,-1,-1,-1,-1,-1,-1}
};

void printMatrix() {
    printf("____________________________________\n\n");
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            printf("%d ",resMatrix[i][j]);
        }
        printf("\n");
    }
    printf("____________________________________\n");
}

// cast void * to int
// lock matrix
// do calculation
// unlock matrix

void *multiplyMatrix(void * arg) {
	int* index = (int *) arg;
    
    pthread_mutex_lock(&mutex);
	int row = *index / HEIGHT;
	int col = *index % WIDTH;
    
    resMatrix[row][col] = 0; // initial value of matrix is -1
    
	int k = 0; // reaching small matrix's row 
    for(int i = row; i < row + SMALLHEIGHT; i++) {
		int l = 0; // reaching small matrix's col
        for(int j = col; j < col + SMALLWIDTH; j++) {
            int num = 0;
	        
            if (i < HEIGHT && j < WIDTH) {
                num = bigMatrix[i][j];
            }
			
			resMatrix[row][col] += smallMatrix[k][l] * num;
            l++;
        }
		k++;
    }
	
    printMatrix();
    pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

int main(void) {

    for (int i = 0; i < NTHREADS; i++) {
        thread_args[i] = i;
        pthread_create(&thread[i], NULL, multiplyMatrix, &thread_args[i]);
    }

    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(thread[i], NULL);
    }

    return 0;
}