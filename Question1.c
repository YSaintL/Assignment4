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
#include <sys/stat.h>
//#include <time.h>
//#include <sys/types.h>
#include <stdbool.h>


int numResources = 4;

struct CustomerRequest {
    int resources[4];
};
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

//int safetyAlg(int available[m], int allocation[n][m], int maxNeed[n][m], int need[n][m]);


// /*int newAvailable(int available[m], int allocation[n][m]){

//     int x,y;

// }

// int initAvailable(int available[m]){

//     int x,y,z;

//     printf("please enter available");

//     for (x = 0; x < m; x++)
//     {
//         scanf("%d", &z);
//         available[x] = z;
//     }
//     return available[m];
// }

int readFile(char* fileName, struct CustomerRequest customerArr[]);

int main(int argc, char *argv[]) {
    // Store ints from argv into an int array
    int numResources[argc-1];
    for(int i = 0; i < argc-1; i++) {
        numResources[i] = atoi(argv[i+1]); 
    }

    struct CustomerRequest customerArr[5] = {};
    char fileName[14] = "sample4_in.txt";
    char* fileNamePtr = fileName;

    readFile(fileNamePtr, customerArr);
    

    int hello = 5;

}

//finding the need matrix
int findNeed(int maxNeed[n][m], int allocation[n][m], int need[n][m]){

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


//checking to see if the sequence is safe
int safetyAlg(int available[m], int allocation[n][m], int maxNeed[n][m], int need[n][m]){

    need[n][m] = findNeed(maxNeed, allocation, need);

    int x,y,count,flag, processFlag;

    int work[m];

    bool isSafe = false;

    int finish[n];

    for(x = 0; x < n; x++){
        finish[x] = 0;
    }

    for(x = 0; x < m; x++){
        work[x] = available[x];
    }

    count = 0;
    while(count < n){
        
        for(x = 0; x < n; x++){

            if(finish[x] == 0){

                flag = 0;

                for(y = 0; y < n; y++){

                    if(need[x][y] > work[y]){
                        flag = 1;
                    }
                }

                if((finish[x] == 0) && (flag == 0)){

                    for(y = 0; y < n; y++){
                        work[y] += allocation[x][y];
                    }
                    
                    finish[x] = 1;
                    processFlag++;
                    count++;

                }
            }
        }

        if(processFlag == n){
            isSafe = true;
        }

        return isSafe;
    }
}


int readFile(char* fileName, struct CustomerRequest customerArr[])//use this method in a suitable way to read file
{
	FILE *in = fopen(fileName, "r");
	if(!in)
	{
		printf("Child A: Error in opening input file...exiting with error code -1\n");
		return -1;
	}
	printf("%s\n", fileName);

	struct stat st;
	fstat(fileno(in), &st);
	char* fileContent = (char*)malloc(((int)st.st_size+1)* sizeof(char));
	fileContent[0]='\0';	
	while(!feof(in))
	{
		char line[100];
		if(fgets(line,100,in)!=NULL)
		{
			strncat(fileContent,line,strlen(line));
		}
	}
	fclose(in);

	char* command = NULL;
	int customerCount = 0;
	char* fileCopy = (char*)malloc((strlen(fileContent)+1)*sizeof(char));
	strcpy(fileCopy,fileContent);
	command = strtok(fileCopy,"\r\n");
	while(command!=NULL)
	{
		customerCount++;
		command = strtok(NULL,"\r\n");
	}
	// *threads = (Thread*) malloc(sizeof(Thread)*customerCount);

	char* lines[customerCount];
	command = NULL;
	int i=0;
	command = strtok(fileContent,"\r\n");
	while(command!=NULL)
	{
		lines[i] = malloc(sizeof(command)*sizeof(char));
		strcpy(lines[i],command);
		i++;
		command = strtok(NULL,"\r\n");
	}

	for(int k=0; k<customerCount; k++)
	{
		char* token = NULL;
		int j = 0;
		// int a;
		token =  strtok(lines[k],",");
		//making array to stash tokens
		char *arr[20] = {NULL};
		while(token!=NULL)
		{
			arr[j++] = token;
			token = strtok(NULL, ",");
//this loop tokenizes each line of input file
//write your code here to populate instances of Thread to build a collection
		}

		// For testing: printing the strings inside of array
		// for(a = 0; a < j; a++)
		// {
			//Thread currentThread th;
			
			// printf("%s\n", arr[a]);
			
		// }
		
        struct CustomerRequest currentCustomerRequest;
        // for (int i = 0; i < 4; i++) {
        //     arr
        // }
        // strncpy(currentCustomerRequest.resources, arr, sizeof(int) * numResources);

        // while (*arr != (void *)0) {
        //     printf("%s", &arr);
        //     arr++;
        // }

        for (int i = 0; i < 4; i++) {
            // printf("Begin: ");
            // printf("%s \n", arr[i]);
            currentCustomerRequest.resources[i] = atoi(arr[i]);
        }
        printf("\n");

		// memcpy(currentCustomerRequest.tid, arr[0], 8); // 8 is the size of the arr[0] (a char pointer)
	
       	// currentCustomerRequest.start_T = atoi(arr[1]);

    	// currentCustomerRequest.lifetime = atoi(arr[2]);

		// printf("%d\n", currentCustomerRequest.lifetime);

        customerArr[k] = currentCustomerRequest;

        // strcpy(customerArr, array1);
	}
	return customerCount;
}