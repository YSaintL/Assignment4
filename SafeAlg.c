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

int safetyAlg(){

    int work[m];

    int finish[n];

    



}

