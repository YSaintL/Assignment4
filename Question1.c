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

int n = 5;
int m = 4;
int numResources = 4;
int doneCustomers[5] = {-1, -1, -1, -1, -1};
int allocation[5][4];
int need[5][4];
int maxNeed[5][4];
int available[4];

struct CustomerRequest {
    int resources[4];
};

struct CommandRequest {
    char type[10];
    int customerAndResources[5]; // index at 0 repreesnts the customer index and the following 4 values represent the resources
};

struct argsStruct {
    int *allocation;
    int *need;
    int *maxNeed;
    int *available;
    int customerNum;
};
//cleaned up a little, made soem comments and commented out matrix prints
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



//int safeSequence[n]; 
int safeSequence[5] = {};

//memset(safeSequence, 0, sizeof(safeSequence[0]) * n); // used to process sequence order


int readFile(char* fileName, struct CustomerRequest customerArr[]);
void findNeed(int maxNeed[n][m], int allocation[n][m], int need[n][m]);
bool safetyAlg(int available[m], int allocation[n][m], int maxNeed[n][m], int need[n][m]);
void request(int available[m], int allocation[n][m], int maxNeed[n][m], int need[n][m], int request[m + 1]);
void Realease(int available[m], int allocation[n][m], int maxNeed[n][m], int need[n][m], int realease[m+1]);
void commandHandler(int allocation[n][m], int need[n][m], int availableResources[m], int maxNeed[n][m]);
void constructMaxNeedMatrix(struct CustomerRequest customerArr[n], int maxNeed[n][m]);
void print2DArray(int arr[n][m], char name[500]);
void print1DArray(int arr[], char name[500], int length);
void status(int available[m], int allocation[n][m], int maxNeed[n][m], int need[n][m]);
int run(int available[m], int allocation[n][m], int maxNeed[n][m], int need[n][m]);
void *threadRun(void *arguments);
void calculateSafeSequence(int available[m], int allocation[n][m], int maxNeed[n][m], int need[n][m]);
int searchArr(int arr[], int num);

int main(int argc, char *argv[]) {
    // Store ints from argv into an int array
    if (argc < 5) {
        printf("Please enter the number of available resources for 4 resources in the form of 4 integers.\n");
        return 0;
    }

    int availableResources[argc-1];
    for(int i = 0; i < argc-1; i++) {
        availableResources[i] = atoi(argv[i+1]);
        available[i] = atoi(argv[i+1]); 
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
                printf(" ");
            }
            printf("%d", customerArr[i].resources[j]);
        }
        printf("\n");
    }

    // int allocation[n][m];
    // int need[n][m];
    // int maxNeed[n][m];
    memset(allocation, 0, sizeof(allocation[0][0]) * m * n);
    memset(need, 0, sizeof(need[0][0]) * m * n);
    memset(maxNeed, 0, sizeof(maxNeed[0][0]) * m * n);

    constructMaxNeedMatrix(customerArr, maxNeed);

    commandHandler(allocation, need, available,maxNeed);

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
    char input[200];
    int numInput = 0;
    int rqCounter = 0;             // Remove this after( this is for testing purposes)
    // Remove this after( this is for testing purposes)
    int rq1[5][5] = {
        {0, 1, 0, 0, 1},
        {1, 1, 1, 1, 1},
        {2, 2, 2, 2, 2},
        {3, 1, 1, 1, 1},
        {4, 1, 0, 0, 0}
    };
    // int rq2[5] = {1, 1, 1, 1, 1};
    // int rq3[5] = {2, 2, 2, 2, 2};
    // int rq4[5] = {3, 1, 1, 1, 1};
    // int rq5[5] = {4, 1, 0, 0, 0};

    while (strcmp(commandInputRequest.type,"-1") != 0) {
        printf("\nEnter Command: (or enter -1 to stop running) ");
        scanf("%s", commandInputRequest.type);
        if (strcmp(commandInputRequest.type, "-1") == 0 ) { // If it is -1, break the loop
            break;
        }

        // scanf("%s", input);
        // strcpy(commandInputRequest.type, input);

        if (strcmp(commandInputRequest.type,"RQ") == 0 || strcmp(commandInputRequest.type, "RL") == 0) {

            // Remove this after( this is for testing purposes)
            memcpy(commandInputRequest.customerAndResources, rq1[rqCounter], sizeof(commandInputRequest.customerAndResources));
            rqCounter++;

            // for(int i = 0; i < numResources + 1; i++){
            //     // commandInputRequest.customerAndResources[i] = 0;
            //     scanf("%s", input);
            //     // scanf("%d", numInput);

            //     // scanf("%s", input);
            //     commandInputRequest.customerAndResources[i] = atoi(input);
            //     // commandInputRequest.customerAndResources[i] = numInput;
                
            // }
            printf("Handling RQ or RL command\n");

            if (strcmp(commandInputRequest.type,"RQ") == 0) {
                printf("Handling RQ command\n");
                findNeed(maxNeed, allocation, need);
                //sends to the request func
               // request(availableResources, allocation, maxNeed, need, commandInputRequest.customerAndResources);
                request(available, allocation, maxNeed, need, commandInputRequest.customerAndResources); //
                
            } else {
                printf("Handling RL command\n");
               // Realease(availableResources, allocation, maxNeed, need, commandInputRequest.customerAndResources);
                Realease(available, allocation, maxNeed, need, commandInputRequest.customerAndResources); //
            }        

        } else if (strcmp(commandInputRequest.type,"Status") == 0 ) {
            printf("Handling status command\n");
           // status(availableResources, allocation, maxNeed, need);
            status(available, allocation, maxNeed, need); //

        } else if (strcmp(commandInputRequest.type,"Run") == 0 ) {
            printf("Handling Run command\n");

            calculateSafeSequence(availableResources, allocation, maxNeed, need); //
            print1DArray(doneCustomers, "Done Customer", 5);
           // run(availableResources, allocation, maxNeed, need);
            run(available, allocation, maxNeed, need);//
        } else {
            printf("Please enter a valid command (RQ, RL, Status, or Run).\n");
        }

        
    }
   
}

void print2DArray(int arr[n][m], char name[500]) {
    if (strcmp(name, "") != 0) { // if name is empty, don't print this line
        printf("\nPrinting the %s variable:\n", name);
    }
    
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}

void print1DArray(int arr[], char name[500], int length) {
    if (strcmp(name, "") != 0) { // if name is empty, don't print this line
        printf("\nPrinting the %s variable:\n", name);
    }

    for(int i = 0; i < length; i++) {
        printf("%d ", arr[i]);
    }
}

void status(int available[m], int allocation[n][m], int maxNeed[n][m], int need[n][m]) {
    printf("Available Resources:\n");
    print1DArray(available, "", m);
    printf("\n");

    printf("\nMaximum Resources:\n");
    print2DArray(maxNeed, "");
    printf("\n");

    printf("Allocated Resources:\n");
    print2DArray(allocation, "");
    printf("\n");

    printf("Need Resources:\n");
    print2DArray(need, "");
    printf("\n");
}

//finding the need matrix
void findNeed(int maxNeed[n][m], int allocation[n][m], int need[n][m]){

    // print2DArray(maxNeed, "maxNeed");
    // print2DArray(allocation, "allocation");
    // print2DArray(need, "need");

    int x,y;

    for(x=0; x < n; x++){
        
        for(y=0; y < m; y++){

            need[x][y] = maxNeed[x][y] - allocation[x][y];
            //printf("need")
        }
    }

    // print2DArray(maxNeed, "maxNeed");
    // print2DArray(allocation, "allocation");
    // print2DArray(need, "need");

   // printf("the Need Matrix has been created\n");
}

void request(int available[m], int allocation[n][m], int maxNeed[n][m], int need[n][m], int request[m + 1]){

    findNeed(maxNeed, allocation, need);

    //before
    // CTRL+/
    // printf("\n LINE 299: [BEFORE] allocation\n");
    // print2DArray(maxNeed, "\n maxNeed\n");
    // print2DArray(allocation, "\n allocation\n");
    // print2DArray(need, "\n need\n");
    // print1DArray(available, "\n available", m);
    // print1DArray(request, "\n request\n", n);

    int x;

    //for all resources in the vector
    for(x=0; x < m; x++){
        //if requesti <= needi
        if(request[x + 1] <= need[request[0]][x]){  
            //if requesti <= availblei
            if(request[x + 1] <= available[x]){
                //available = available - request
                available[x] = available[x] - request[x+1];
                //allocationi = allocationi + requesti
                allocation[request[0]][x] = allocation[request[0]][x] + request[x+1];
                //needi = needi - requesti
                need[request[0]][x] = need[request[0]][x] - request[x+1];
            }
            else{
                printf("waiting 1 ");
            }
        }
        else{
            printf("waiting 2 ");
        }
    }

    //after
    // printf("\n LINE 331: [AFTER] allocation\n");
    // print2DArray(maxNeed, "\nmaxNeed\n");
    // print2DArray(allocation, "\nallocation\n");
    // print2DArray(need, "\nneed\n");
    // print1DArray(available, "\navailable", m);
    // print1DArray(request, "\nrequest\n", n);
    // printf("\n  [BEFORE SAFE ALG]\n");


    //safetyAlg(int available[m], int allocation[n][m], int maxNeed[n][m], int need[n][m]);
    bool isSafe = safetyAlg(available,allocation,maxNeed,need);

    //after safetyAlg
    
    // CTRL+/
    // printf("\n  [AFTER SAFE ALG]\n");
    // print2DArray(maxNeed, "\nmaxNeed\n");
    // print2DArray(allocation, "\nallocation\n");
    // print2DArray(need, "\nneed\n");
    // print1DArray(available, "\navailable", m);
    // print1DArray(request, "\nrequest\n", n);

    if(isSafe){

        printf("\n<The Resources are [SAFE]>\n");
        //available[y] -= allocation[x][y];

    }
    else{
        //de-allocation
        printf("\n<LINE 359 The Resources are [NOT SAFE]>\n");

        for(x=0; x < m; x++){
                //available = available + request
            available[x] = available[x] + request[x+1];
                //allocationi = allocationi + requesti
            allocation[request[0]][x] = allocation[request[0]][x] - request[x+1];
                //needi = needi - requesti
            need[request[0]][x] = need[request[0]][x] + request[x+1];
        } 

        //after deallocation
        printf("\n After [DE-ALLOCATION]\n");
        printf("\n"); //space to be clean
        print2DArray(maxNeed, "\nmaxNeed\n");
        print2DArray(allocation, "\nallocation\n");
        print2DArray(need, "\nneed\n");
        print1DArray(available, "\navailable\n", n);
        print1DArray(request, "\nrequest\n", m);
        printf("\n");
        printf("\n The safe sequence is: ");
        for(x=0;x<n;x++){
            printf("P%d\t", safeSequence[x]+1);
        }
    }
}

void Realease(int available[m], int allocation[n][m], int maxNeed[n][m], int need[n][m], int realease[m + 1]){


    //before
    // CTRL+/
    // printf("\n Before [DE-ALLOCATION]");
    // printf("\n"); // space to look clean
    // print2DArray(maxNeed, "\n maxNeed");
    // print2DArray(allocation, "\n allocation");
    // print2DArray(need, "\n need");
    // print1DArray(available, "\n available", m);
    // print1DArray(realease, "\n release", n);


    int x;
    bool canDeallocate = true;

    for(x=0; x < m; x++){
        if(realease[x + 1] > allocation[realease[0]][x]){
            printf("Could not be de-allocated\n");
            canDeallocate = false;
        }
    }

    if (canDeallocate) {
        for(x=0; x < m; x++){ //if true
            if(realease[x + 1] <= allocation[realease[0]][x]){
                allocation[realease[0]][x] -= realease[x+1];
                available[x] += realease[x+1];
            }

            else{ // if false
                printf("could not be de-allocated\n");
            }
        }
    }
    

    //after
    // CTRL+/
    // printf("\n After [DE-ALLOCATION]");
    // print2DArray(maxNeed, "\nmaxNeed");
    // print2DArray(allocation, "\nallocation");
    // print2DArray(need, "\nneed");
    // print1DArray(available, "\navailable", m);
    // print1DArray(realease, "\nrelease", n);
}

int run(int available[m], int allocation[n][m], int maxNeed[n][m], int need[n][m]){
    // int customerNum;

    // Create args to pass in the thread run function
    struct argsStruct args;
    args.allocation = &allocation;
    args.need = &need;
    args.maxNeed = &maxNeed;
    args.available = &available;
    
    pthread_t t1;
    pthread_t t2;
    pthread_t t3;
    pthread_t t4;
    pthread_t t5;

    for(int i = 0; i < n; i++) {
        // customerNum = safeSequence[i];
        args.customerNum = doneCustomers[i];
        if(i == 0) {
            pthread_create(&t1, NULL, &threadRun, (void *)&args);
            pthread_join(t1, NULL);
        } else if(i == 1) {
            pthread_create(&t2, NULL, &threadRun, (void *)&args);
            pthread_join(t2, NULL);
        } else if(i == 2) {
            pthread_create(&t3, NULL, &threadRun, (void *)&args);
            pthread_join(t3, NULL);
        } else if(i == 3) {
            pthread_create(&t4, NULL, &threadRun, (void *)&args);
            pthread_join(t4, NULL);
        } else if(i == 4) {
            pthread_create(&t5, NULL, &threadRun, (void *)&args);
            pthread_join(t5, NULL);
        }
    }
    //
    return 0;
}

void *threadRun(void *arguments) {
    int sum = 0;
    struct argsStruct *args = arguments;

    printf("--> Customer / Thread %d\n", args->customerNum);
    printf("    Allocated resources: ");
    //print1DArray(args->allocation[args->customerNum], "", m);
    print1DArray(allocation[args->customerNum], "", m);

    printf("     Needed: ");
    //print1DArray(args->need[args->customerNum], "", m);
    print1DArray(need[args->customerNum], "", m);

    printf("     Available: ");
    //print1DArray(args->available, "", m);
    print1DArray(available, "", m);

    printf("     Thread has started\n");
    printf("     Thread has finished\n");
    printf("     Thread is releasing resources\n");
    // Need to put allocated amount into an array and then call release to release the allocation for the customer/thread to deallocate resource

    int  x;
    int releaseArr[m + 1];
    releaseArr[0] = args->customerNum;

    for(x=0; x < m; x++){
        //releaseArr[x+1] = args->allocation[args->customerNum][x];
        releaseArr[x+1] = allocation[args->customerNum][x];
    } 

    //Realease(args->available, args->allocation, args->maxNeed, args->need, releaseArr);
    Realease(available, allocation, maxNeed, need, releaseArr);

    // You want to deallocate the resources since we will be done

    // releaseArr = {1, 1, 1, 1, 1}
    // RL 1 1 1 1 1 --> release()

    printf("     New Availible: \n");
    //print1DArray(args->available, "", m);
    print1DArray(available, "", m);
    
    return NULL;
}


//checking to see if the sequence is safe
bool safetyAlg(int available[m], int allocation[n][m], int maxNeed[n][m], int need[n][m]){
   
    int counter = n; // the amount of customers/processes
    //printf("\n(339)counter is: %d\n", counter); 
    int safeFlag = 0; // 0 if safe, 1 if not safe
    int x,y,i,flag; /*x,y,i just variables for the for loops below... flag is used to break lines 397 if there
                      is a need > avaialble*/

    int finish[n]; //array of the processes
    bool isSafe;
    int seqCounter = -1; //used to process sequence order
    int seqCount = 0; //used to process sequence order

    //findNeed(maxNeed, allocation, need); // finding the need matrix

    for(x = 0; x < n; x++){
        finish[x] = 0; //setting all processes to false( or 1)
    }

    while(counter != 0){ // while there are still processes running

        safeFlag = 0;

        for(x=0; x < n; x++){ //used to enter 2d variables

            if(finish[x] == 0){ // if there exists processes init with false/1

                flag = 0;

                for(y=0; y < m; y++){
                   // printf("\nthe available index here is: %d\n", available[y]);

                    if(need[x][y] <= available[y]){ //check if need is greater than available then break

                       // printf("\n (366) need is %d and available is %d", need[x][y], available[y]);
                        flag = 1;
                        break;
                    }
                   // printf("\nthe available index here is: %d\n", available[y]);
                }
                if(flag == 1){ // if available[y] > need[x][y]
                   // printf("\n (370) Process %d is executing\n", x + 1); //x was i

                  //  printf("\n (372 )finish[x] is at %d\n", finish[x]);
                    finish[x] = 1;

                    counter--;
                    //printf("\n (375) The counter is now at %d\n", counter);

                    //recording the sequence
                    seqCounter = x;
                    safeSequence[seqCount] = seqCounter;
                    seqCount++;
                    ////////////////////////
                    
                    safeFlag = 1; // the proces breaks here (435)

                    // for(y=0; y < m; y++){

                    //     available[y] += allocation[x][y]; //adds the allocation[x][y] to available vector
                    //     printf("\n (383) The available vector is now: [%d]", available[y]);
                    // }
                    break;
                }
            }
            //checking safety sequence
           // printf("\n SS process %d", n);

        }

        if(!safeFlag){
            printf("\nLINE 591 [SAFE ALG] this sequence is not in safe state");
            isSafe = false;
            break;
        }
        else{

            // CTRL+/
            // printf("\nLINE 597 [SAFE STATE SATISFIED]!\n");


           // printf("\n The current available vector is:\n");

            // for(i=0;i<m;i++){
            //     printf("\t%d\n", available[i]); //make breakpoint here
            // }
            // printf("\n");
            isSafe = true;
        }
    }
    //return safe?
    return isSafe;
}

void calculateSafeSequence(int available[m], int allocation[n][m], int maxNeed[n][m], int need[n][m]) {
    bool canRun;
    int sequenceIndex = 0;

    int tempAvailable[m];
    int tempAllocation[n][m];
    int tempNeed[n][m];
    int tempMaxNeed[n][m];

    memcpy(tempAvailable, available, sizeof (int) * n * m);
    memcpy(tempAllocation, allocation, sizeof (int) * n * m);
    memcpy(tempNeed, need, sizeof (int) * n * m);
    memcpy(tempMaxNeed, maxNeed, sizeof (int) * n * m);

    int releaseArr[m+1];


    while (sequenceIndex < n) {
        for(int i = 0; i < n; i++) { //
            status(tempAvailable, tempAllocation, tempMaxNeed, tempNeed);

            canRun = true;

            if (searchArr(doneCustomers, i) == 1) {
                continue;
            }

            for(int j = 0; j < m; j++) {
                if (tempNeed[i][j] > tempAvailable[j]) {
                    canRun = false;
                    break;
                }
            }

            if (canRun) {
                // safeSequence[sequenceIndex] = i;
                doneCustomers[sequenceIndex] = i;
                sequenceIndex++;

                releaseArr[0] = i; //

                for(int k = 0; k < m; k++) { //
                    releaseArr[k+1] = tempAllocation[i][k];
                }
                Realease(tempAvailable, tempAllocation, tempMaxNeed, tempNeed, releaseArr);
                status(tempAvailable, tempAllocation, tempMaxNeed, tempNeed);
                int hello = 5;
                continue;

                // 0 1 1 1 1

            }
        }
    }
    
    int a = 5;
}

int searchArr(int arr[], int num) {
    for(int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
    {
        if(arr[i] == num) // checks if it is equal
            return 1;
    }
    return 0;
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