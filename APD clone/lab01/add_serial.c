#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/*
    schelet pentru exercitiul 5
*/
#define NUM_THREADS 4

int* arr;
int array_size;

typedef struct {
    long id;
    int start;
    int end;
    int *arr;
} data;

void *add(void *arg) {
    data info = *(data*)arg;
    for(int i = info.start; i < info.end; i++) {
        info.arr[i] += 100;
    }
}

int main(int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];
    data info[NUM_THREADS];
    long ids[NUM_THREADS];
    long id;
    int r;
    if (argc < 2) {
        perror("Specificati dimensiunea array-ului\n");
        exit(-1);
    }

    array_size = atoi(argv[1]);

    arr = malloc(array_size * sizeof(int));
    for (int i = 0; i < array_size; i++) {
        arr[i] = i;
    }

    for (int i = 0; i < array_size; i++) {
        printf("%d", arr[i]);
        if (i != array_size - 1) {
            printf(" ");
        } else {
            printf("\n");
        }
    }

    // TODO: aceasta operatie va fi paralelizata
  	// for (int i = 0; i < array_size; i++) {
    //     arr[i] += 100;
    // }
    
    for (id = 0; id < NUM_THREADS; id++) {
        ids[id]= id;
        info->id = id;
        info->start = id * (double) array_size / NUM_THREADS;
        if((id + 1) * (double) array_size / NUM_THREADS < array_size) {
            info->end = (id + 1) * (double) array_size / NUM_THREADS;
        } else {
            info->end = array_size;
        }
        info->arr = arr;
        r = pthread_create(&threads[id], NULL, add, &info);
        if (r) {
	  		printf("Eroare la crearea thread-ului %ld\n", id);
	  		exit(-1);
		}
    }

    for (int i = 0; i < array_size; i++) {
        printf("%d", arr[i]);
        if (i != array_size - 1) {
            printf(" ");
        } else {
            printf("\n");
        }
    }

  	pthread_exit(NULL);
}
