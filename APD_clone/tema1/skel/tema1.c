#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "genetic_algorithm.h"

int main(int argc, char *argv[]) {
	struct timeval start, end;
	double time_spent;
	gettimeofday(&start, NULL);
	// array with all the objects that can be placed in the sack
	sack_object *objects = NULL;

	// number of objects
	int object_count = 0;

	// maximum weight that can be carried in the sack
	int sack_capacity = 0;

	// number of generations
	int generations_count = 0;

	if (!read_input(&objects, &object_count, &sack_capacity, &generations_count, argc, argv)) {
		return 0;
	}

	run_genetic_algorithm(objects, object_count, generations_count, sack_capacity);

	free(objects);
	gettimeofday(&end, NULL);
    time_spent = ((double) ((double) (end.tv_usec - start.tv_usec) / 1000000 +
                            (double) (end.tv_sec - start.tv_sec)));
    printf("Time taken for execution: %lf seconds\n", time_spent);
	return 0;
}
