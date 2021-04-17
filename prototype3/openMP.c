// To run: Use gcc opemMP.c -fopenmp

#include <omp.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	// Runtime function to request a certain number of threads
	omp_set_num_threads(4);

	#pragma omp parallel
	{
		// Runtime function returning a thread ID 
		int ID = omp_get_thread_num();
		printf("\n Hello from Thread %d", ID);
	}
	return 0;

}