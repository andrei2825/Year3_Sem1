#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "genetic_algorithm_par.h"
#define min(a,b) a < b ? a: b
//structure that will hold the data passed to each thread
typedef struct {
	int id;
	int num_threads;
	int object_count;
	individual *current_generation;
	const sack_object *objects;
	int generations_count;
} arguments1;

// merge the sections that remain after the merge sort 
void merge_sections(individual *arr, int number, int pos, int num_per_turn, int len) {
    for(int i = 0; i < number; i = i + 2) {
        int left = i * (num_per_turn * pos);
        int right = ((i + 2) * num_per_turn * pos) - 1;
        int mid = left + (num_per_turn * pos) - 1;
        if (right >= len) {
            right = len - 1;
        }
        merge(arr, left, mid, right);
    }
    if (number / 2 >= 1) {
        merge_sections(arr, number / 2, pos * 2, num_per_turn, len);
    }
}
// execute merge sort for a section of the array on a thread
void *paralel_merge_sort(void* arg)
{
    arguments1 data = *(arguments1*)arg;
	int num_per_thread = data.object_count / data.num_threads;
	int offset = data.object_count % data.num_threads;
    int left = data.id * (num_per_thread);
    int right = (data.id + 1) * (num_per_thread) - 1;
    if (data.id == data.num_threads - 1) {
        right += offset;
    }
	int mid = left + (right - left) / 2;
    if (left < right) {
        merge_sort(data.current_generation, left, right);
		merge_sort(data.current_generation, left+1, right);
		merge(data.current_generation, left, mid, right);
    }
    pthread_exit(NULL);
}


// sequential merge sort, used by each thread
void merge_sort(individual *current_generation, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(current_generation, left, mid);
        merge_sort(current_generation, mid + 1, right);
        merge(current_generation, left, mid, right);
    }
}



// sequential merge function, used by each thread
void merge(individual *arr, int left, int mid, int right) {
    int i = 0;
    int j = 0;
    int k = 0;
    int len_left = mid - left + 1;
    int len_right = right - mid;
    individual arr_left[len_left];
    individual arr_right[len_right];
    for (int i = 0; i < len_left; i ++) {
        arr_left[i] = arr[left + i];
    }
    
    for (int j = 0; j < len_right; j ++) {
        arr_right[j] = arr[mid + 1 + j];
    }
    
    i = 0;
    j = 0;

    while (i < len_left && j < len_right) {
        if ((arr_right[j].fitness - arr_left[i].fitness) <= 0) {
            arr[left + k] = arr_left[i];
            i ++;
        } else {
            arr[left + k] = arr_right[j];
            j ++;
        }
        k ++;
    }
    
    while (i < len_left) {
        arr[left + k] = arr_left[i];
        k ++;
        i ++;
    }
    while (j < len_right) {
        arr[left + k] = arr_right[j];
        k ++;
        j ++;
    }
}


int read_input(sack_object **objects, int *object_count, int *sack_capacity, int *generations_count, int *num_threads, int argc, char *argv[])
{
	FILE *fp;

	if (argc < 4) {
		fprintf(stderr, "Usage:\n\t./tema1 in_file generations_count\n");
		return 0;
	}

	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		return 0;
	}

	if (fscanf(fp, "%d %d", object_count, sack_capacity) < 2) {
		fclose(fp);
		return 0;
	}

	if (*object_count % 10) {
		fclose(fp);
		return 0;
	}

	sack_object *tmp_objects = (sack_object *) calloc(*object_count, sizeof(sack_object));

	for (int i = 0; i < *object_count; ++i) {
		if (fscanf(fp, "%d %d", &tmp_objects[i].profit, &tmp_objects[i].weight) < 2) {
			free(objects);
			fclose(fp);
			return 0;
		}
	}

	fclose(fp);

	*generations_count = (int) strtol(argv[2], NULL, 10);
	
	if (*generations_count == 0) {
		free(tmp_objects);

		return 0;
	}

	*num_threads = (int) strtol(argv[3], NULL, 10);

	if (*num_threads == 0) {
		free(tmp_objects);
		
		return 0;
	}

	*objects = tmp_objects;

	return 1;
}

void print_objects(const sack_object *objects, int object_count)
{
	for (int i = 0; i < object_count; ++i) {
		printf("%d %d\n", objects[i].weight, objects[i].profit);
	}
}

void print_generation(const individual *generation, int limit)
{
	for (int i = 0; i < limit; ++i) {
		for (int j = 0; j < generation[i].chromosome_length; ++j) {
			printf("%d ", generation[i].chromosomes[j]);
		}

		printf("\n%d - %d\n", i, generation[i].fitness);
	}
}

void print_best_fitness(const individual *generation)
{
	printf("%d\n", generation[0].fitness);
}

void compute_fitness_function(const sack_object *objects, individual *generation, int object_count, int sack_capacity)
{
	int weight;
	int profit;

	for (int i = 0; i < object_count; ++i) {
		weight = 0;
		profit = 0;

		for (int j = 0; j < generation[i].chromosome_length; ++j) {
			if (generation[i].chromosomes[j]) {
				weight += objects[j].weight;
				profit += objects[j].profit;
			}
		}

		generation[i].fitness = (weight <= sack_capacity) ? profit : 0;
	}
}

void mutate_bit_string_1(const individual *ind, int generation_index)
{
	int i, mutation_size;
	int step = 1 + generation_index % (ind->chromosome_length - 2);

	if (ind->index % 2 == 0) {
		// for even-indexed individuals, mutate the first 40% chromosomes by a given step
		mutation_size = ind->chromosome_length * 4 / 10;
		for (i = 0; i < mutation_size; i += step) {
			ind->chromosomes[i] = 1 - ind->chromosomes[i];
		}
	} else {
		// for even-indexed individuals, mutate the last 80% chromosomes by a given step
		mutation_size = ind->chromosome_length * 8 / 10;
		for (i = ind->chromosome_length - mutation_size; i < ind->chromosome_length; i += step) {
			ind->chromosomes[i] = 1 - ind->chromosomes[i];
		}
	}
}

void mutate_bit_string_2(const individual *ind, int generation_index)
{
	int step = 1 + generation_index % (ind->chromosome_length - 2);

	// mutate all chromosomes by a given step
	for (int i = 0; i < ind->chromosome_length; i += step) {
		ind->chromosomes[i] = 1 - ind->chromosomes[i];
	}
}

void crossover(individual *parent1, individual *child1, int generation_index)
{
	individual *parent2 = parent1 + 1;
	individual *child2 = child1 + 1;
	int count = 1 + generation_index % parent1->chromosome_length;

	memcpy(child1->chromosomes, parent1->chromosomes, count * sizeof(int));
	memcpy(child1->chromosomes + count, parent2->chromosomes + count, (parent1->chromosome_length - count) * sizeof(int));

	memcpy(child2->chromosomes, parent2->chromosomes, count * sizeof(int));
	memcpy(child2->chromosomes + count, parent1->chromosomes + count, (parent1->chromosome_length - count) * sizeof(int));
}

void copy_individual(const individual *from, const individual *to)
{
	memcpy(to->chromosomes, from->chromosomes, from->chromosome_length * sizeof(int));
}

void free_generation(individual *generation)
{
	int i;

	for (i = 0; i < generation->chromosome_length; ++i) {
		free(generation[i].chromosomes);
		generation[i].chromosomes = NULL;
		generation[i].fitness = 0;
	}
}

void run_genetic_algorithm(const sack_object *objects, int object_count, int generations_count, int sack_capacity, int num_threads)
{
	pthread_t threads[num_threads];
    long id;
	arguments1 a1[num_threads];;
	pthread_barrier_t barier;
	int count, cursor;
	individual *current_generation = (individual*) calloc(object_count, sizeof(individual));
	individual *next_generation = (individual*) calloc(object_count, sizeof(individual));
	individual *tmp = NULL;
	pthread_barrier_init(&barier, NULL, num_threads + 1);
	//set initial generation (composed of object_count individuals with a single item in the sack)

	for (int i = 0; i < object_count; ++i) {
		current_generation[i].fitness = 0;
		current_generation[i].chromosomes = (int*) calloc(object_count, sizeof(int));
		current_generation[i].chromosomes[i] = 1;
		current_generation[i].index = i;
		current_generation[i].chromosome_length = object_count;

		next_generation[i].fitness = 0;
		next_generation[i].chromosomes = (int*) calloc(object_count, sizeof(int));
		next_generation[i].index = i;
		next_generation[i].chromosome_length = object_count;
	}
	
	// iterate for each generation
	for (int k = 0; k < generations_count; ++k) {
		cursor = 0;

		// compute fitness and sort by it
		compute_fitness_function(objects, current_generation, object_count, sack_capacity);

		for(id = 0; id < num_threads; id++) {
			// populate the data structure
			a1[id].id = id;
			a1[id].current_generation = current_generation;
			a1[id].generations_count = generations_count;
			a1[id].num_threads = num_threads;
			a1[id].object_count = object_count;
			// create treads
			if (pthread_create(&threads[id], NULL, paralel_merge_sort, (void *)&a1[id]) != 0){
				perror("Failed to create thread");
			}
		}
		// join threads
		for(id = 0; id < num_threads; id++) {
			if (pthread_join(threads[id], NULL) != 0) {
				perror("Failed to join thread");
			}
		}
		// merging the remaining sections sequentially
		int num_per_thread = object_count / num_threads;
		merge_sections(current_generation, num_threads, 1, num_per_thread, object_count);

		// keep first 30% children (elite children selection)
		count = object_count * 3 / 10;
		for (int i = 0; i < count; ++i) {
			copy_individual(current_generation + i, next_generation + i);
		}
		cursor = count;

		// mutate first 20% children with the first version of bit string mutation
		count = object_count * 2 / 10;
		for (int i = 0; i < count; ++i) {
			copy_individual(current_generation + i, next_generation + cursor + i);
			mutate_bit_string_1(next_generation + cursor + i, k);
		}
		cursor += count;

		// mutate next 20% children with the second version of bit string mutation
		count = object_count * 2 / 10;
		for (int i = 0; i < count; ++i) {
			copy_individual(current_generation + i + count, next_generation + cursor + i);
			mutate_bit_string_2(next_generation + cursor + i, k);
		}
		cursor += count;

		// crossover first 30% parents with one-point crossover
		// (if there is an odd number of parents, the last one is kept as such)
		count = object_count * 3 / 10;

		if (count % 2 == 1) {
			copy_individual(current_generation + object_count - 1, next_generation + cursor + count - 1);
			count--;
		}

		for (int i = 0; i < count; i += 2) {
			crossover(current_generation + i, next_generation + cursor + i, k);
		}

		// switch to new generation
		tmp = current_generation;
		current_generation = next_generation;
		next_generation = tmp;

		for (int i = 0; i < object_count; ++i) {
			current_generation[i].index = i;
		}

		if (k % 5 == 0) {
			print_best_fitness(current_generation);
		}
	}
	compute_fitness_function(objects, current_generation, object_count, sack_capacity);
	print_best_fitness(current_generation);

	// // free resources
	free(current_generation);
	free(next_generation);
	
	pthread_barrier_destroy(&barier);
}