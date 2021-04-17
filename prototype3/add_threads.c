// Author: Dr. Shital Joshi
// Email: shital.joshi@okstate.edu
// Description: POSIX pthread to demonstrate addition

// Run: Use gcc add_threads.c -lpthread

#include <stdio.h>
#include <pthread.h>

#define MAX 9000000000 // 9 billion times

struct arg_struct {
    long long int lowerLimit;
    long long int upperLimit;
};

void *add(void *arguments)
{
	struct arg_struct *args = arguments;
	
	long long int i, result = 0;
	for (i = args -> lowerLimit; i <= args -> upperLimit; i++){
		result = result + i;
	}
	
  	return (void *) result;
}

int main()
{
	clock_t t;
	t = clock();
	double time;
	
    pthread_t thread1, thread2;
    struct arg_struct args1;
    args1.lowerLimit = 1;
    args1.upperLimit = MAX/2;

    if (pthread_create(&thread1, NULL, &add, (void *)&args1) != 0) {	// thread1 create
        printf("Thread1 failed\n");
        return -1;
    }

    struct arg_struct args2;
    args2.lowerLimit = MAX/2+1;
    args2.upperLimit = MAX;
    if (pthread_create(&thread2, NULL, &add, (void *)&args2) != 0) {	// thread1 create
        printf("Thread2 failed\n");
        return -1;
    }
	
	void *result1, *result2;
	long long int finalResult;
	
	//wait until the thread completes, assign return value to str
	pthread_join(thread1, &result1);	/* Wait until thread1 is finished */
	pthread_join(thread2, &result2);	/* Wait until thread2 is finished */

	finalResult = (long long int)result1 + (long long int)result2;
	
	t = clock() - t;
	time = (double)t/CLOCKS_PER_SEC;
		
	printf("\nTime taken: %f sec\n", time);

	printf("\nFinal result is: %lld\n", finalResult);
	return 0;
}
