/*
 ----------------------------------
 Author:  Daniel Faseyi and Alex Do
 ID:      190850640 and 180531080
 Email:   Fase0640@mylaurier.ca and doxx1080@mylaurier.ca
 ----------------------------------
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
//#include <sys/stat.h>
//#include <time.h>
//#include <sys/types.h>
//#include <stdbool.h>

/* RQ - take a request
      - check if the request will result in a safe state
      - if safe = update the allocation of matrix to allocate resources
      - if not safe = reject allocation

   RL - if the value of the user inputted release of the specific thread (if T1 = unput)
       then subtract the input from that thread.
      - if the inputted value does not equal the thread then reject it

   RUN - execute the safe sequence: <t1, t0, t3, ... tn>
       - output the allocation matrix, maxneed matrix, need matrix and available vector 
*/


int n = 5;

int m = 4;



//find the need matrix
int findNeed(int maxNeed[n][m], int allocation[n][m]){

    int x,y;

    for(x=0; x < n; x++){
        
        for(y=0; y < m; y++){

            need[n][m] = maxNeed[n][m] - allocation[n][m];
            //printf("need")
        }
    }
    printf("the Need Matrix has been created");
    return need[n][m];
}

int newAvailable(int available[m], int allocation[n][m]){

    int x,y;

}

int initAvailable(int available[m]){

    int x,y,z;

    printf("please enter available");

    for (x = 0; x < m; x++)
    {
        scanf("%d", &z);
        available[x] = z;
    }
    return available[m];
}

int main(int argc, char *argv[]) {

    int 
}
