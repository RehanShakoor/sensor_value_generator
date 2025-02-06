/*
Name     : Rehan Shakoor
Document : Sensor Value Generator
Date     : 06/02/2025
*/

//include required headers
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>

//define macro for buffer size
#define BUFFER_SIZE 100
//define macro for buffer print limit
#define PRINT_LIMIT 50 

//define global variable to store generated data 
uint8_t buffer[BUFFER_SIZE];  
//define global variable to store count of bytes stored
uint8_t stored_bytes = 0;  

//define and initialise mutex for shared resource
pthread_mutex_t buffer_lock = PTHREAD_MUTEX_INITIALIZER;

//function declaration for SIGALRM signal handler
void SIGALRM_handler(int signum); 
//function declaration to setup timer
void timer_setup(void); 

int main() 
{
	//seed for rand() function
    srand(time(NULL));
    //call function to setup the timer
    timer_setup();

	//run main thread in a while() loop
	while(1) 
	{
		//wakeup in main after every 10 sec
        sleep(10);
		//lock the shared resource
        pthread_mutex_lock(&buffer_lock);

		//check if stored bytes are more than print limit 
        if(stored_bytes >= PRINT_LIMIT) 
		{
			//print info and required bytes
            printf("\nMAIN   : Printing latest %d bytes\n", PRINT_LIMIT);
            
			//print latest bytes
			uint8_t print_count = 0;
			for (uint8_t i = stored_bytes - PRINT_LIMIT; i <= (stored_bytes - 1); i++) 
			{
				//print only 10 bytes in a row
                printf("%02X ", buffer[i]);
				print_count++;
				if(!(print_count % 10))
				{
					printf("\n");
				}
            }

            //delete printed bytes
            stored_bytes -= PRINT_LIMIT;
			//print current buffer size
			printf("MAIN   : Total bytes stored in buffer : %hhd\n\n", stored_bytes);
        } 

		//unlock the shared resource
        pthread_mutex_unlock(&buffer_lock);
    }

    return 0;
}

//function definition for SIGALRM signal handler
void SIGALRM_handler(int signum) 
{
	//lock the shared resource
    pthread_mutex_lock(&buffer_lock);
    //generate count of bytes (from 0 to 5)
    uint8_t bytes_count = rand() % 6;
    printf("SENSOR : Byte/s of data generated     : %hhd\n", bytes_count);

	//iterate bytes_count times
    for (uint8_t i = 1; i <= bytes_count; i++) 
	{	
		//generate random number of 1 byte and store it in buffer
        buffer[stored_bytes++] = rand() % 256;
    }

	//print current buffer size
	printf("SENSOR : Total bytes stored in buffer : %hhd\n", stored_bytes);
	//unlock the shared resource
    pthread_mutex_unlock(&buffer_lock);
}

//function definition to setup timer
void timer_setup(void) 
{
	//setting up signal handler for SIGALRM
    struct sigaction sa;
	//assign cutom function to SIGALRM 
    sa.sa_handler = SIGALRM_handler;
	//not blocking other signals
    sigemptyset(&sa.sa_mask);
	//setting default behaviour
    sa.sa_flags = 0;
	//register custom funtion for SIGALRM
    sigaction(SIGALRM, &sa, NULL);

	//setting up timer to generate SIGALRM signal
    struct itimerval timer;
    //set first timer expiration value (1 sec)
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
	//set timer interval for timer expiration (1 sec)
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;
    //activate the timer
    setitimer(ITIMER_REAL, &timer, NULL);
}
