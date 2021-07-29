# Project Title: 
Bankers Algorithm in C

# Motivation
We wanted to create a program that implements the banker's algorithm while using multi-threads.
Customers would be able to request and release resources from a "bank" with the program keeping track of the resources. Only when our safety algorithm is fulfilled, then the "banker" will allow a request to go through. If the request does not satisfy the safety algorithm, then the "banker" denies this request.

Given 5 customers and 4 types of resources, our "banker" uses a few data structures to keep track.
available - 1D array of 4 ints: number of each resource available
maxNeed - 2D array of 5 rows by 4 columns: the maximum demand of each customer. There are 5 customers and 4 resource types
allocation - 2D array of 5 rows by 4 columns: the amount of resources allocated to each customer. There are 5 customers and 4 resource types
need - 2D array of 5 rows by 4 columns: the remaining needed resources for each customer. There are 5 customers and 4 resource types

# Installation
To use this program, you need a Linux environment.

# Screenshots

# Individual Contribution
Alex Do was responsible for: general program creation, debugging this program, handling inputs, revising initial logic of program.

Daniel Faseyi was responsible for: general program creation, debugging this program, coming up with initial logic for this program.

The two developers collaboated on this project and much of the work was intertwined.

# Features
The program takes an initial 4 resources that are put into the available array when it is run.

It allows you to put in requests to the banker by calling the RQ command. These requests are allocated. Once you have put 5 requests (one for each customer), then the Run command can be called to calculate the safe sequence, then run the threads in the right order. After each thread goes, the resources will be deallocated and released, and added back to the available array.

The RL command can also be called to release resources after they have been requested, this will remove them from the allocation array, and add them back into the available array.

The status command allows you to view the current values in the need, maxNeed, available, and allocation arrays.

# Test cases
In our test, once we called the "make runq1",
We tested using these commands:

RQ 0 1 0 0 1
RQ 1 1 1 1 1
RQ 2 2 2 2 2
RQ 3 1 1 1 1
RQ 4 1 0 0 0
Status
Run
Status

After using these commands, the program worked as intended.
To test the release command, we used these commands:

RQ 0 1 0 0 1
Status
RL 0 1 0 0 1
Status

The program worked as intended.

# Use examples
To run this program, open a terminal/command line in this folder. Type "make" into the terminal to compile the code using the Makefile. 
Afterwards, call "make runq1" to get the program running with default available resource values of 10 5 7 8. To use custom values, instead of calling "make runq1" call "./Question1 10 5 7 8". Your own 4 integers can be used as desired.

# About Developers
Alex Do and Daniel Faseyi are student developers currently majoring in Computer Science. They collaborated to create this program together.

# Credits
Big thank you to Dr. Sukhjit Singh Sehra for helping us in the the debugging process and guiding us in learning these concepts and creating this program.

# License to use your code
If using this code, please provide us credit.
