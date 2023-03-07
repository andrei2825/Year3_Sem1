#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
	int rank;
	int nProcesses;
	int numWorkers;
	int numMaster = 3;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	int *workers;
	int master;
	int *topology;
	int *recv_topology;
	int recv_size;

    if(rank == 0) {

		/*------------------TASK 1------------------*/
		// Citesc in coordonator din fisierul destinat acestuia
		char filename[13] = "cluster0.txt";
		FILE *f = fopen(filename, "rt");
		fscanf(f, "%d", &numWorkers);
		workers = malloc(numWorkers * sizeof(int));
		for(int i = 0; i < numWorkers; i++) {
			fscanf(f, "%d", &workers[i]);
			// trimit fiecarui worker legat la coordonator, rank-ul acestuia
			MPI_Send( &rank , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank, workers[i]);
		}
		// creez topologia
		topology = malloc(nProcesses * sizeof(int));
		for (int i = 0; i < nProcesses; i++) {
			topology[i] = -1;
		}
		for (int i = 0; i < numWorkers; i++)
		{
			topology[workers[i]] = rank;
		}
		// trimit topologia acestui cluster catre celelalte clustere
		MPI_Send( &numWorkers , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD);
		printf("M(%d,%d)\n", 0, 2);
		for (int i = 0; i < numWorkers; i++)
		{
			MPI_Send( &workers[i] , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", 0, 2);
		}

		MPI_Status recv_status;
		// primesc topologiile de la celelalte clustere
		MPI_Recv( &recv_size , 1, MPI_INT , 2 , 0 , MPI_COMM_WORLD , &recv_status);
		recv_topology = malloc(recv_size * sizeof(int));
		for (int i = 0; i < recv_size; i++)
		{
			MPI_Recv( &recv_topology[i] , 1, MPI_INT , 2 , 0 , MPI_COMM_WORLD , &recv_status);
			topology[recv_topology[i]] = 2;
		}


		MPI_Recv( &recv_size , 1, MPI_INT , 2 , 0 , MPI_COMM_WORLD , &recv_status);
		for (int i = 0; i < recv_size; i++)
		{
			MPI_Recv( &recv_topology[i] , 1, MPI_INT , 2 , 0 , MPI_COMM_WORLD , &recv_status);
			topology[recv_topology[i]] = 1;
		}


		// coordonatorul printeaza topologia
		printf("%d -> ", rank);
		for (int i = 0; i < 3; i++)
		{
			int tmp = 0;
			printf("%d:", i);
			for (int j = 0; j < nProcesses; j++)
			{
				if(topology[j] == i) {
					tmp++;
				}
			}
				
			for (int j = 0; j < nProcesses; j++)
			{
				if(topology[j] == i){
					if(tmp-- > 1){
						printf("%d,", j);	
					} else {
						printf("%d", j);
					}
				}
			}
			printf(" ");
		}
		printf("\n");
		// coordonatorul trimite topologia catre toti workerii lui
		for (int i = 0; i < numWorkers; i++)
		{
			MPI_Send( topology , nProcesses , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank, workers[i]);
		}


		
		/*------------------TASK 2------------------*/

		int N = atoi(argv[1]);
		int *V = malloc(N * sizeof(int));
		int totalWorkers = nProcesses - 3;
		// coordonatorul 0 genereaza vectorul
		for (int i = 0; i < N; i++) {
			V[i] = i;
		}
		// calculeaza numarul de workeri ai fiecarui coordonator
		int workers_per_cluster[3] = {0, 0, 0};
		for (int i = 0; i < nProcesses; i++)
		{
			if(topology[i] == 0)  {
				workers_per_cluster[0]++;
			} else if(topology[i] == 1)  {
				workers_per_cluster[1]++;
			} else if(topology[i] == 2)  {
				workers_per_cluster[2]++;
			}
		}
		// calculeaza numarul minim de taskuri pe care le va avea fiecare worker
		int tasks = N / totalWorkers;
		int index = 0;
		// trimite taskurile fiecarui worker
		for (int i = 0; i < numWorkers; i++) {
			MPI_Send( &tasks , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank,  workers[i]);
			int *tmp = malloc(tasks * sizeof(int));
			int k = 0;
			for (int j = index; j < index + tasks; j++)
			{
				tmp[k++] = V[j];
			}
			
			MPI_Send( tmp , tasks , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank,  workers[i]);
			index += tasks;
		}
		// trimite taskurile coordonatorilor 1 si 2 prin 2
		for (int i = 1; i < 3; i++) {
			int numTasks = workers_per_cluster[i] * tasks;
			MPI_Send( &numTasks , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank,  2);
			int *tmp = malloc(numTasks * sizeof(int));
			int k = 0;
			for (int j = index; j < index + workers_per_cluster[i] * tasks; j++)
			{
				tmp[k++] = V[j];
			}
			MPI_Send( tmp , numTasks , MPI_INT , 2 , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank,  2);
			index += workers_per_cluster[i] * tasks;
		}

		
		int *ret_vec = malloc(numWorkers * tasks * sizeof(int));
		int *result = malloc(N * sizeof(int));
		index = 0;
		// primeste rezultatele de la workeri
		for (int i = 0; i < numWorkers; i++)
		{
			int *rcv_tmp = malloc(tasks * sizeof(int));
			MPI_Recv( rcv_tmp , tasks , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD , &recv_status);
			for (int j = 0; j < tasks; j++)
			{
				result[index++] =  rcv_tmp[j];
			}	
		}
		
		// primeste rezultatele trimise de ceilalti coordonatori
		for (int j = 1; j < 3; j++)
		{
			int numTasks = workers_per_cluster[j] * tasks;
			int *rcv_tmp = malloc(numTasks * sizeof(int));
			MPI_Recv( rcv_tmp , numTasks , MPI_INT ,2 , 0 , MPI_COMM_WORLD , &recv_status);
			for(int i = 0; i < numTasks; i++) {
				result[index++] = rcv_tmp[i];
			}
		}
		int remainingTasks = N - (tasks * totalWorkers);
		// variabile pe care coordonatorul 0 le va trimite pentru a anunta daca este nevoie sau nu de acel worker/coordonator
		int noNeed = -1;
		int need = -2;
		int needOne = -3;
		int noNeedOne = -4;


		// daca dupa aceste calcule mai raman taskuri netrimise, le trimite pe rand workerilor, apoi celorlalti coordonatori in functie de nevoie
		if (remainingTasks > 0 && remainingTasks <= numWorkers)
		{
			for (int i = 0; i < remainingTasks; i++)
			{
				MPI_Send( &need , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
				printf("M(%d,%d)\n", rank,  workers[i]);
				MPI_Send( &V[index] , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
				printf("M(%d,%d)\n", rank,  workers[i]);
				MPI_Recv( &result[index++] , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD , &recv_status);
			}
			for (int i = remainingTasks; i < numWorkers; i++)
			{
				MPI_Send( &noNeed , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
				printf("M(%d,%d)\n", rank,  workers[i]);
			}
			MPI_Send( &noNeedOne , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank,  2);
			MPI_Send( &noNeed , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank,  2);
		}else if(remainingTasks > numWorkers && remainingTasks <= (workers_per_cluster[0] + workers_per_cluster[1])) {
			for (int i = 0; i < numWorkers; i++)
			{
				MPI_Send( &need , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
				printf("M(%d,%d)\n", rank,  workers[i]);
				MPI_Send( &V[index] , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
				printf("M(%d,%d)\n", rank,  workers[i]);
				MPI_Recv( &result[index++] , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD , &recv_status);
			}


			MPI_Send( &needOne , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank,  2);
			int tasksFor1 = remainingTasks - numWorkers;
			MPI_Send( &tasksFor1 , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank,  2);
			for (int i = 0; i < tasksFor1; i++)
			{
				MPI_Send( &V[index] , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD);
				printf("M(%d,%d)\n", rank,  2);
				MPI_Recv( &result[index++] , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD , &recv_status);
			}
			MPI_Send( &noNeed , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank,  2);
			


		}else if(remainingTasks > (workers_per_cluster[0] + workers_per_cluster[1]) && remainingTasks <= (workers_per_cluster[0] + workers_per_cluster[1] + workers_per_cluster[2])) {
			for (int i = 0; i < numWorkers; i++)
			{
				MPI_Send( &need , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
				printf("M(%d,%d)\n", rank,  workers[i]);
				MPI_Send( &V[index] , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
				printf("M(%d,%d)\n", rank,  workers[i]);
				MPI_Recv( &result[index++] , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD , &recv_status);
			}


			MPI_Send( &needOne , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank,  2);
			int tasksFor1 = workers_per_cluster[1];
			MPI_Send( &tasksFor1 , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank,  2);
			for (int i = 0; i < tasksFor1; i++)
			{
				MPI_Send( &V[index] , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD);
				printf("M(%d,%d)\n", rank,  2);
				MPI_Recv( &result[index++] , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD , &recv_status);
			}

			MPI_Send( &need , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank,  2);
			int tasksFor2 = remainingTasks - (numWorkers + workers_per_cluster[1]) ;
			MPI_Send( &tasksFor2 , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank,  2);
			for (int i = 0; i < tasksFor2; i++)
			{
				MPI_Send( &V[index] , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD);
				printf("M(%d,%d)\n", rank,  2);
				MPI_Recv( &result[index++] , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD , &recv_status);
			}
		}else {
			MPI_Send( &noNeedOne , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank,  2);
			MPI_Send( &noNeed , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank,  2);
			for (int i = 0; i < numWorkers; i++)
			{
				MPI_Send( &noNeed , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
				printf("M(%d,%d)\n", rank,  workers[i]);
			}
		}
		

		sleep(1);


		printf("Rezultat: ");
		for (int i = 0; i < index; i++)
		{
			printf("%d ",  result[i]);
		}
		printf("\n");

		
			
		
		
		


	} else if(rank == 1) {
		
		/*------------------TASK 1------------------*/
		// Citesc in coordonator din fisierul destinat acestuia
		char filename[13] = "cluster1.txt";
		FILE *f = fopen(filename, "rt");
		fscanf(f, "%d", &numWorkers);
		workers = malloc(numWorkers * sizeof(int));
		for(int i = 0; i < numWorkers; i++) {
			fscanf(f, "%d", &workers[i]);
			MPI_Send( &rank , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank, workers[i]);
		}

		topology = malloc(nProcesses * sizeof(int));
		for (int i = 0; i < nProcesses; i++) {
			topology[i] = -1;
		}
		for (int i = 0; i < numWorkers; i++)
		{
			topology[workers[i]] = rank;
		}
		
		MPI_Send( &numWorkers , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD);
		printf("M(%d,%d)\n", 1, 2);
		for (int i = 0; i < numWorkers; i++)
		{
			MPI_Send( &workers[i] , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", 1, 2);
		}

		MPI_Status recv_status;
		MPI_Recv( &recv_size , 1, MPI_INT , 2 , 0 , MPI_COMM_WORLD , &recv_status);
		recv_topology = malloc(recv_size * sizeof(int));
		for (int i = 0; i < recv_size; i++)
		{
			MPI_Recv( &recv_topology[i] , 1, MPI_INT , 2 , 0 , MPI_COMM_WORLD , &recv_status);
			topology[recv_topology[i]] = 2;
		}

		MPI_Recv( &recv_size , 1, MPI_INT , 2 , 0 , MPI_COMM_WORLD , &recv_status);
		for (int i = 0; i < recv_size; i++)
		{
			MPI_Recv( &recv_topology[i] , 1, MPI_INT , 2 , 0 , MPI_COMM_WORLD , &recv_status);
			topology[recv_topology[i]] = 0;
		}

		printf("%d -> ", rank);
		for (int i = 0; i < 3; i++)
		{
			int tmp = 0;
			printf("%d:", i);
			for (int j = 0; j < nProcesses; j++)
			{
				if(topology[j] == i) {
					tmp++;
				}
			}
				
			for (int j = 0; j < nProcesses; j++)
			{
				if(topology[j] == i){
					if(tmp-- > 1){
						printf("%d,", j);	
					} else {
						printf("%d", j);
					}
				}
			}
			printf(" ");
		}
		printf("\n");


		for (int i = 0; i < numWorkers; i++)
		{
			MPI_Send( topology , nProcesses , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank, workers[i]);
		}

		
		/*------------------TASK 2------------------*/
		int numTasks;
		MPI_Recv( &numTasks , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD , &recv_status);
		int *vec = malloc(numTasks*sizeof(int));
		MPI_Recv( vec , numTasks , MPI_INT , 2 , 0 , MPI_COMM_WORLD , &recv_status);
		int index = 0;
		int tasks = numTasks / numWorkers;
		for (int i = 0; i < numWorkers; i++) {
			MPI_Send( &tasks , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank,  workers[i]);
			int *to_send = malloc(tasks * sizeof(int)); 
			int k = 0;
			for (int j = index; j < index + tasks; j++)
			{
				to_send[k++] = vec[j];
			}
			MPI_Send( to_send , tasks , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank,  workers[i]);
			index += tasks;
		}
		
		int *ret_vec = malloc(numWorkers * tasks * sizeof(int));
		index = 0;
		for (int i = 0; i < numWorkers; i++)
		{
			int *ret_worker = malloc(tasks * sizeof(int));
			MPI_Recv( ret_worker , tasks , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD , &recv_status);
			for (int j = 0; j < tasks; j++)
			{
				ret_vec[index++] = ret_worker[j];
			}	
		}

		MPI_Send( ret_vec , numTasks , MPI_INT , 2 , 0 , MPI_COMM_WORLD);
		printf("M(%d,%d)\n", rank,  2);

		int signal;
		int value;
		int finals;
		int need = -2;
		int noNeed = -1;
		MPI_Recv( &signal , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD , &recv_status);
		if(signal == -2) {
			MPI_Recv( &finals , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD , &recv_status);

			for (int i = 0; i < finals; i++)
			{
				MPI_Recv( &value , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD , &recv_status);
				MPI_Send( &need , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
				printf("M(%d,%d)\n", rank,  workers[i]);
				MPI_Send( &value , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
				printf("M(%d,%d)\n", rank,  workers[i]);
				MPI_Recv( &value , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD , &recv_status);
				MPI_Send( &value , 1 , MPI_INT , 2 , 0 , MPI_COMM_WORLD);
				printf("M(%d,%d)\n", rank,  2);
			}
			for (int i = finals; i < numWorkers; i++)
			{
				MPI_Send( &noNeed , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
				printf("M(%d,%d)\n", rank,  workers[i]);
			}
		} else if(signal == -1) {
			for (int i = 0; i < numWorkers; i++)
			{
				MPI_Send( &noNeed , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
				printf("M(%d,%d)\n", rank,  workers[i]);
			}
		}	


		
		
	} else if(rank == 2) {
		
		/*------------------TASK 1------------------*/
		// Citesc in coordonator din fisierul destinat acestuia
		char filename[13] = "cluster2.txt";
		FILE *f = fopen(filename, "rt");
		fscanf(f, "%d", &numWorkers);
		workers = malloc(numWorkers * sizeof(int));
		for(int i = 0; i < numWorkers; i++) {
			fscanf(f, "%d", &workers[i]);
			MPI_Send( &rank , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank, workers[i]);
		}

		topology = malloc(nProcesses * sizeof(int));
		for (int i = 0; i < nProcesses; i++) {
			topology[i] = -1;
		}
		for (int i = 0; i < numWorkers; i++)
		{
			topology[workers[i]] = rank;
		}
		//coordonatorul 2 va fi folosit pe post de intermediar intre coordonatorul 0 si coordonatorul 1
		MPI_Send( &numWorkers , 1 , MPI_INT , 1 , 0 , MPI_COMM_WORLD);
		printf("M(%d,%d)\n", 2, 1);
		for (int i = 0; i < numWorkers; i++)
		{
			MPI_Send( &workers[i] , 1 , MPI_INT , 1 , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", 2, 1);
		}
		
		MPI_Send( &numWorkers , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD);
		printf("M(%d,%d)\n", 2, 0);
		for (int i = 0; i < numWorkers; i++)
		{
			MPI_Send( &workers[i] , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", 2, 0);
		}

		MPI_Status recv_status;
		MPI_Recv( &recv_size , 1, MPI_INT , 1 , 0 , MPI_COMM_WORLD , &recv_status);
		MPI_Send( &recv_size , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD);
		printf("M(%d,%d)\n", 2, 0);
		recv_topology = malloc(recv_size * sizeof(int));
		for (int i = 0; i < recv_size; i++)
		{
			MPI_Recv( &recv_topology[i] , 1, MPI_INT , 1 , 0 , MPI_COMM_WORLD , &recv_status);
			topology[recv_topology[i]] = recv_status.MPI_SOURCE;
			MPI_Send( &recv_topology[i] , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", 2, 0);
		}

		MPI_Recv( &recv_size , 1, MPI_INT , 0 , 0 , MPI_COMM_WORLD , &recv_status);
		MPI_Send( &recv_size , 1 , MPI_INT , 1 , 0 , MPI_COMM_WORLD);
		printf("M(%d,%d)\n", 2, 1);
		for (int i = 0; i < recv_size; i++)
		{
			MPI_Recv( &recv_topology[i] , 1, MPI_INT , 0 , 0 , MPI_COMM_WORLD , &recv_status);
			topology[recv_topology[i]] = recv_status.MPI_SOURCE;
			MPI_Send( &recv_topology[i] , 1 , MPI_INT , 1 , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", 2, 1);
		}

		printf("%d -> ", rank);
		for (int i = 0; i < 3; i++)
		{
			int tmp = 0;
			printf("%d:", i);
			for (int j = 0; j < nProcesses; j++)
			{
				if(topology[j] == i) {
					tmp++;
				}
			}
				
			for (int j = 0; j < nProcesses; j++)
			{
				if(topology[j] == i){
					if(tmp-- > 1){
						printf("%d,", j);	
					} else {
						printf("%d", j);
					}
				}
			}
			printf(" ");
		}
		printf("\n");

		for (int i = 0; i < numWorkers; i++)
		{
			MPI_Send( topology , nProcesses , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank, workers[i]);
		}

		
		/*------------------TASK 2------------------*/
		int numTasksForOne;
		MPI_Recv( &numTasksForOne , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD , &recv_status);
		int *vecForOne = malloc(numTasksForOne*sizeof(int));
		MPI_Recv( vecForOne , numTasksForOne , MPI_INT , 0 , 0 , MPI_COMM_WORLD , &recv_status);
		MPI_Send( &numTasksForOne , 1 , MPI_INT , 1 , 0 , MPI_COMM_WORLD);
		printf("M(%d,%d)\n", rank,  1);
		MPI_Send( vecForOne , numTasksForOne , MPI_INT , 1 , 0 , MPI_COMM_WORLD);
		printf("M(%d,%d)\n", rank,  1);
		int numTasks;
		MPI_Recv( &numTasks , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD , &recv_status);
		int *vec = malloc(numTasks*sizeof(int));
		MPI_Recv( vec , numTasks , MPI_INT , 0 , 0 , MPI_COMM_WORLD , &recv_status);

		int index = 0;
		int tasks = numTasks / numWorkers;
		for (int i = 0; i < numWorkers; i++) {
			MPI_Send( &tasks , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank,  workers[i]);
			int *to_send = malloc(tasks * sizeof(int)); 
			int k = 0;
			for (int j = index; j < index + tasks; j++)
			{
				to_send[k++] = vec[j];
			}
			MPI_Send( to_send , tasks , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank,  workers[i]);
			index += tasks;
		}
		
		int *ret_vec = malloc(numWorkers * tasks * sizeof(int));
		index = 0;
		for (int i = 0; i < numWorkers; i++)
		{
			int *ret_worker = malloc(tasks * sizeof(int));
			MPI_Recv( ret_worker , tasks , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD , &recv_status);
			for (int j = 0; j < tasks; j++)
			{
				ret_vec[index++] = ret_worker[j];
			}	
		}

		int *ret_vec_for_one = malloc(numTasksForOne * sizeof(int));
		MPI_Recv( ret_vec_for_one , numTasksForOne , MPI_INT , 1 , 0 , MPI_COMM_WORLD , &recv_status);
		MPI_Send( ret_vec_for_one , numTasksForOne , MPI_INT , 0 , 0 , MPI_COMM_WORLD);
		printf("M(%d,%d)\n", rank,  0);

		MPI_Send( ret_vec , numWorkers*tasks , MPI_INT , 0 , 0 , MPI_COMM_WORLD);
		printf("M(%d,%d)\n", rank,  0);



		int signal;
		int value;
		int finals;
		int need = -2;
		int noNeed = -1;
		MPI_Recv( &signal , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD , &recv_status);
		if(signal == -3) {
			MPI_Send( &need , 1 , MPI_INT , 1 , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank,  1);
			MPI_Recv( &finals , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD , &recv_status);
			MPI_Send( &finals , 1 , MPI_INT , 1 , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank,  1);
			for (int i = 0; i < finals; i++)
			{
				MPI_Recv( &value , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD , &recv_status);
				MPI_Send( &value , 1 , MPI_INT , 1 , 0 , MPI_COMM_WORLD);
				printf("M(%d,%d)\n", rank,  1);
				MPI_Recv( &value , 1 , MPI_INT , 1 , 0 , MPI_COMM_WORLD , &recv_status);
				MPI_Send( &value , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD);
				printf("M(%d,%d)\n", rank,  0);
			}
		} else if(signal == -4) {
			MPI_Send( &noNeed , 1 , MPI_INT , 1 , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank,  1);
		}

		MPI_Recv( &signal , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD , &recv_status);
		if(signal == -2) {
			MPI_Recv( &finals , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD , &recv_status);
			

			for (int i = 0; i < finals; i++)
			{
				MPI_Recv( &value , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD , &recv_status);
				MPI_Send( &need , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
				printf("M(%d,%d)\n", rank,  workers[i]);
				MPI_Send( &value , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
				printf("M(%d,%d)\n", rank,  workers[i]);
				MPI_Recv( &value , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD , &recv_status);
				MPI_Send( &value , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD);
				printf("M(%d,%d)\n", rank,  0);
			}
			for (int i = finals; i < numWorkers; i++)
			{
				MPI_Send( &noNeed , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
				printf("M(%d,%d)\n", rank,  workers[i]);
			}
		} else if(signal == -1) {
			for (int i = 0; i < numWorkers; i++)
			{
				MPI_Send( &noNeed , 1 , MPI_INT , workers[i] , 0 , MPI_COMM_WORLD);
				printf("M(%d,%d)\n", rank,  workers[i]);
			}
		}

	} else {
		
		/*------------------TASK 1------------------*/
		
		MPI_Status recv_status;
		MPI_Recv( &master , 1 , MPI_INT , MPI_ANY_SOURCE , 0 , MPI_COMM_WORLD , &recv_status);
		topology = malloc(nProcesses * sizeof(int));
		MPI_Recv( topology , nProcesses , MPI_INT , master , 0 , MPI_COMM_WORLD , &recv_status);

		printf("%d -> ", rank);
		for (int i = 0; i < 3; i++)
		{
			int tmp = 0;
			printf("%d:", i);
			for (int j = 0; j < nProcesses; j++)
			{
				if(topology[j] == i) {
					tmp++;
				}
			}
				
			for (int j = 0; j < nProcesses; j++)
			{
				if(topology[j] == i){
					if(tmp-- > 1){
						printf("%d,", j);	
					} else {
						printf("%d", j);
					}
				}
			}
			printf(" ");
		}
		printf("\n");

		
		/*------------------TASK 2------------------*/
		
		int numTasks;
		int task;
		MPI_Recv( &numTasks , 1 , MPI_INT , master , 0 , MPI_COMM_WORLD , &recv_status);

		int *numbers = malloc(numTasks * sizeof(int));
		MPI_Recv( numbers , numTasks , MPI_INT , master , 0 , MPI_COMM_WORLD , &recv_status);
		for (int i = 0; i < numTasks; i++)
		{
			numbers[i] *= 2;
		}
		MPI_Send( numbers , numTasks , MPI_INT , master , 0 , MPI_COMM_WORLD);
		printf("M(%d,%d)\n", rank, master);
		
		int signal;
		int recv_value;
		MPI_Recv( &signal , 1 , MPI_INT , master , 0 , MPI_COMM_WORLD , &recv_status);
		if(signal == -2) {
			MPI_Recv( &recv_value , 1 , MPI_INT , master , 0 , MPI_COMM_WORLD , &recv_status);
			recv_value *= 2;
			MPI_Send( &recv_value , 1 , MPI_INT , master , 0 , MPI_COMM_WORLD);
			printf("M(%d,%d)\n", rank, master);
		}
	}	

	
	MPI_Finalize();
	return 0;
}
