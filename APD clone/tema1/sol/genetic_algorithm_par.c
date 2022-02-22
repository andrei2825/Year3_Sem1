#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "genetic_algorithm_par.h"
#define min(a,b) a < b ? a: b
//const sack_object *objects, int object_count, int generations_count, int sack_capacity, int num_threads
typedef struct {
	int id;
	int num_threads;
	int object_count;
	individual *current_generation;
	individual *next_generation;
	const sack_object *objects;
	int generations_count;
	int sack_capacity;
	pthread_barrier_t barier;
} arguments1;


void merge_sections_of_array(individual *arr, int number, int aggregation, int num_per_turn, int len) {
    for(int i = 0; i < number; i = i + 2) {
        int left = i * (num_per_turn * aggregation);
        int right = ((i + 2) * num_per_turn * aggregation) - 1;
        int middle = left + (num_per_turn * aggregation) - 1;
        if (right >= len) {
            right = len - 1;
        }
        merge(arr, left, middle, right);
    }
    if (number / 2 >= 1) {
        merge_sections_of_array(arr, number / 2, aggregation * 2, num_per_turn, len);
    }
}

void *thread_merge_sort(void* arg)
{
    arguments1 data = *(arguments1*)arg;
	// pthread_barrier_wait(&data.barier);
	int num_per_thread = data.object_count / data.num_threads;
	int offset = data.object_count % data.num_threads;
    int left = data.id * (num_per_thread);
    int right = (data.id + 1) * (num_per_thread) - 1;
    if (data.id == data.num_threads - 1) {
        right += offset;
    }
	int middle = left + (right - left) / 2;
    if (left < right) {
        merge_sort(data.current_generation, left, right);
		merge_sort(data.current_generation, left+1, right);
		merge(data.current_generation, left, middle, right);
    }
    pthread_exit(NULL);
}



void merge_sort(individual *current_generation, int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;
        merge_sort(current_generation, left, middle);
        merge_sort(current_generation, middle + 1, right);
        merge(current_generation, left, middle, right);
    }
}




void merge(individual *arr, int left, int middle, int right) {
    int i = 0;
    int j = 0;
    int k = 0;
    int left_length = middle - left + 1;
    int right_length = right - middle;
    individual left_array[left_length];
    individual right_array[right_length];
    for (int i = 0; i < left_length; i ++) {
        left_array[i] = arr[left + i];
    }
    
    for (int j = 0; j < right_length; j ++) {
        right_array[j] = arr[middle + 1 + j];
    }
    
    i = 0;
    j = 0;


    while (i < left_length && j < right_length) {
		int res = right_array[j].fitness - left_array[i].fitness; // decreasing by fitness
		// printf("(%d, %d)\n", left_array[i].fitness, right_array[j].fitness);
		// if (res == 0) {
		// 	int first_count = 0, second_count = 0;
		// 	for (int l = 0; l < left_array[i].chromosome_length && l < right_array[j].chromosome_length; ++l) {
		// 		first_count += left_array[i].chromosomes[l];
		// 		second_count += right_array[j].chromosomes[l];
		// 	}

		// 	res = first_count - second_count; // increasing by number of objects in the sack
		// 	if (res == 0) {
		// 		res = right_array[j].index - left_array[i].index;
		// 	}
		// }
        if (res <= 0) {
            arr[left + k] = left_array[i];
            i ++;
        } else {
            arr[left + k] = right_array[j];
            j ++;
        }
        k ++;
    }
    
    while (i < left_length) {
        arr[left + k] = left_array[i];
        k ++;
        i ++;
    }
    while (j < right_length) {
        arr[left + k] = right_array[j];
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

int cmpfunc(const void *a, const void *b)
{
	int i;
	individual *first = (individual *) a;
	individual *second = (individual *) b;

	int res = second->fitness - first->fitness; // decreasing by fitness
	if (res == 0) {
		int first_count = 0, second_count = 0;

		for (i = 0; i < first->chromosome_length && i < second->chromosome_length; ++i) {
			first_count += first->chromosomes[i];
			second_count += second->chromosomes[i];
		}

		res = first_count - second_count; // increasing by number of objects in the sack
		if (res == 0) {
			return second->index - first->index;
		}
	}

	return res;
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
		// for(int l =  0; l < object_count; l++) {
		// 	printf("%d, ", current_generation[l].fitness);
		// }
		// printf("\n");

		for(id = 0; id < num_threads; id++) {
			a1[id].id = id;
			a1[id].current_generation = current_generation;
			a1[id].generations_count = generations_count;
			a1[id].num_threads = num_threads;
			a1[id].object_count = object_count;
			a1[id].barier = barier;
			if (pthread_create(&threads[id], NULL, thread_merge_sort, (void *)&a1[id]) != 0){
				perror("Failed to create thread");
			}
		}
		for(id = 0; id < num_threads; id++) {
			if (pthread_join(threads[id], NULL) != 0) {
				perror("Failed to join thread");
			}
		}
		int num_per_thread = object_count / num_threads;
		merge_sections_of_array(current_generation, num_threads, 1, num_per_thread, object_count);
		
		// for(int l =  0; l < object_count; l++) {
		// 	printf("%d, ", current_generation[l].fitness);
		// }
		// printf("\n");

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