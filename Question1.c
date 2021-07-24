/*
 ----------------------------------
 Author:  Daniel Faseyi and Alex Do
 ID:      190850640 and 180531080
 Email:   Fase0640@mylaurier.ca and doxx1080@mylaurier.ca
 ----------------------------------
 */
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

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

struct CommandRequest {
    char type[10];
    int customerAndResources[5]; // index at 0 repreesnts the customer index and the following 4 values represent the resources
};

/* RQ - take a request
      - check if the request will result in a safe state
      - if safe = update the allocation of matrix to allocate resources
      - if not safe = reject allocation

   RL - if the value of the user inputted release of the specific thread (if T1 = input)
       then subtract the input from that thread.
      - if the inputted value does not equal the thread then reject it

   RUN - execute the safe sequence: <t1, t0, t3, ... tn>
       - output the allocation matrix, maxneed matrix, need matrix and available vector 
*/


int n = 5;

int m = 4;


int readFile(char* fileName, struct CustomerRequest customerArr[]);
void findNeed(int maxNeed[n][m], int allocation[n][m], int need[n][m]);
int safetyAlg(int available[m], int allocation[n][m], int maxNeed[n][m], int need[n][m]);
void Request(int available[m], int allocation[n][m], int maxNeed[n][m], int need[n][m], int request[m], int m, int n);
void Realease(int available[m], int allocation[n][m], int maxNeed[n][m], int need[n][m], int realease[m], int m, int n);
void commandHandler(int allocation[n][m], int need[n][m], int availableResources[m], int maxNeed[n][m]);
void constructMaxNeedMatrix(struct CustomerRequest customerArr[n], int maxNeed[n][m]);
void print2DArray(int arr[n][m], char name[500]);
void print1DArray(int arr[], char name[500]);

int main(int argc, char *argv[]) {
    // Store ints from argv into an int array
    if (argc < 5) {
        printf("Please enter the number of available resources for 4 resources in the form of 4 integers.\n");
        return 0;
    }

    int availableResources[argc-1];
    for(int i = 0; i < argc-1; i++) {
        availableResources[i] = atoi(argv[i+1]); 
    }
    if (availableResources > 0) {
        printf("Number of Customers: %lu\n", (sizeof(availableResources) / sizeof(int)));\
        printf("Currently Available resources: ");
        for(int i = 0; i < (sizeof(availableResources) / sizeof(int)); i++){
            printf("%d ", availableResources[i]);
        }
        printf("\n");
    }


    struct CustomerRequest customerArr[5] = {};
    char fileName[14] = "sample4_in.txt";
    char* fileNamePtr = fileName;

    int customerCount = readFile(fileNamePtr, customerArr);

    printf("Maximum resources from file\n");

    for (int i = 0; i < customerCount; i++) {
        for (int j = 0; j < (sizeof(availableResources) / sizeof(int)); j++) {
            if (j > 0) {
                printf(",");
            }
            printf("%d", customerArr[i].resources[j]);
        }
        printf("\n");
    }

    int allocation[n][m];
    int need[n][m];
    int maxNeed[n][m];
    memset(allocation, 0, sizeof(allocation[0][0]) * m * n);
    memset(need, 0, sizeof(need[0][0]) * m * n);
    memset(maxNeed, 0, sizeof(maxNeed[0][0]) * m * n);

    constructMaxNeedMatrix(customerArr, maxNeed);

    commandHandler(allocation, need, availableResources, maxNeed);

    int hello = 5;

}

void constructMaxNeedMatrix(struct CustomerRequest customerArr[n], int maxNeed[n][m]) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            maxNeed[i][j] = customerArr[i].resources[j];
        }
    }
}

void commandHandler(int allocation[n][m], int need[n][m], int availableResources[m], int maxNeed[n][m]) {
    struct CommandRequest commandInputRequest;
    // commandInputRequest.type = '';
    // int commandInputRequest[5] = {};
    // char input[200] = '';

    // while (strcmp(commandInputRequest.type,"-1") != 0) {
        printf("Enter Command: (or enter -1 to stop running)");
        scanf("s", commandInputRequest.type);
        // scanf("%s", input);
        // strcpy(commandInputRequest.type, input);

        if (strcmp(commandInputRequest.type,"RQ") != 0 || strcmp(commandInputRequest.type, "RL")) {
            for(int i = 0; i < numResources + 1; i++){
                scanf("%d", &commandInputRequest.customerAndResources[i]);
                
            }
            printf("Handling RQ or RL command\n");

            if (strcmp(commandInputRequest.type,"RQ") != 0) {
                printf("Handling RQ command\n");
                findNeed(maxNeed, allocation, need);
            }


            

            printf("hello33333");
            

        } else if (strcmp(commandInputRequest.type,"*") != 0 ) {
            printf("Handling * command\n");
        } else if (strcmp(commandInputRequest.type,"Run") != 0 ) {
            printf("Handling Run command\n");
        } else {
            printf("Please enter a valid command.");
        }

        
    // }
   
}

void print2DArray(int arr[n][m], char name[500]) {
    printf("\nPrinting the %s variable:\n", name);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}

void print1DArray(int arr[], char name[500]) {
    printf("\nPrinting the %s variable:\n", name);
    for(int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
}

//finding the need matrix
void findNeed(int maxNeed[n][m], int allocation[n][m], int need[n][m]){

    print2DArray(maxNeed, "maxNeed");
    print2DArray(allocation, "allocation");
    print2DArray(need, "need");

    int x,y;

    for(x=0; x < n; x++){
        
        for(y=0; y < m; y++){

            need[x][y] = maxNeed[x][y] - allocation[x][y];
            //printf("need")
        }
    }

    print2DArray(maxNeed, "maxNeed");
    print2DArray(allocation, "allocation");
    print2DArray(need, "need");

    printf("the Need Matrix has been created\n");
}

void Request(int available[m], int allocation[n][m], int maxNeed[n][m], int need[n][m], int request[m], int m, int n){

    findNeed(maxNeed, allocation, need);

    int x;
    //for all resources in the vector
    for(x=0; x < m; x++){
        //if requesti <= needi
        if(request[x + 1] <= need[request[0]][x]){  
            //if requesti <= availblei
            if(request[x + 1] <= available[x]){
                //available = available - request
                available[x] = available[x] - request[x];
                //allocationi = allocationi + requesti
                allocation[request[0]][x] = allocation[request[0]][x] + request[x];
                //needi = needi - requesti
                need[request[0]][x] = need[request[0]][x] - request[x];
            }
            else{
                printf("waiting");
            }
        }
        else{
            printf("waiting");
        }
    }


    //safetyAlg(int available[m], int allocation[n][m], int maxNeed[n][m], int need[n][m]);
    safetyAlg(available,allocation,maxNeed,need);

    if(true){

        printf("the Resources are safe!");

    }
    else{

        printf("the Resources are NOT safe!");

        available[x] = available[x] + request[x];
                //allocationi = allocationi + requesti
        allocation[request[0]][x] = allocation[request[0]][x] - request[x];
                //needi = needi - requesti
        need[request[0]][x] = need[request[0]][x] + request[x];
    }
}

void Realease(int available[m], int allocation[n][m], int maxNeed[n][m], int need[n][m], int realease[m], int m, int n){

    int x;

    for(x=0; x < m; x++){

        if(realease[x + 1] == allocation[realease[0]][x]){
            
            allocation[realease[0]][x] -= allocation[realease[0]][x];
        }

        else{
            printf("could not be de-allocated");
        }
    }
}

int Run(int available[m], int allocation[n][m], int maxNeed[n][m], int need[n][m]){

    //
    return 0;
}


//checking to see if the sequence is safe
int safetyAlg(int available[m], int allocation[n][m], int maxNeed[n][m], int need[n][m]){

    findNeed(maxNeed, allocation, need);

    int x,y,count,flag, processFlag;

    int work[m];

    // this is the safe sequence character array that adds th
    /*char safeSeqChar[n];

    // this is the safe sequence counter that will increminate for every exacuted process
    int  safeSeqCount;*/

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
                        printf("the safe sequence is [%d]", y);
                        
                    }
                    
                    finish[x] = 1;
                    processFlag++;
                    count++;

                }
            }
        }
    }



    if(processFlag == n){
        isSafe = true;
    }

    return isSafe;
}


int readFile(char* fileName, struct CustomerRequest customerArr[])//use this method in a suitable way to read file
{
	FILE *in = fopen(fileName, "r");
	if(!in)
	{
		printf("Child A: Error in opening input file...exiting with error code -1\n");
		return -1;
	}
	// printf("%s\n", fileName);

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
        // printf("\n");

		// memcpy(currentCustomerRequest.tid, arr[0], 8); // 8 is the size of the arr[0] (a char pointer)
	
       	// currentCustomerRequest.start_T = atoi(arr[1]);

    	// currentCustomerRequest.lifetime = atoi(arr[2]);

		// printf("%d\n", currentCustomerRequest.lifetime);

        customerArr[k] = currentCustomerRequest;

        // strcpy(customerArr, array1);
	}
	return customerCount;
}