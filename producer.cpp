//Isaac Fowler
//OS Programming Assignment #1
//Producer File
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
	int sharedMem;
	int *table;
	//Allocating shared memory using shm_open and mmap
	sharedMem = shm_open("table", O_CREAT | O_RDWR, 0666);
	ftruncate(sharedMem,sizeof(int));
	table = static_cast<int*>(mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, sharedMem, 0));

	//Semaphores used to keep track of buffer and synchoronize the producer
	sem_t* full = sem_open("full", O_CREAT, 0666, 0);
	sem_t* empty = sem_open("empty", O_CREAT, 0666, 3);
	sem_t* mutex = sem_open("mutex", O_CREAT, 0666, 1);

	std::cout << "Producer & Consumer are running...\n" << std::endl;

	//For loop that will run 8 iterations (Consumes 8 items in total)
	for (int i = 0; i < 8; ++i) {
		sem_wait(empty); //Increment empty semaphore
		sleep(rand()%2+1);

		sem_wait(mutex); //Unlock mutex
		if(*table < 2) {
			++(*table);
			std::cout << "Item produced." << std::endl << "Table contains: " << *table << " items\n";
		} else {
			std::cout << "Buffer is full!\n";
		}
		sem_post(mutex); //Lock mutex
		sem_post(full); //Decrement full semaphore
	}

	sleep(5);
	std::cout << "Process completed. Press ENTER to exit.";

	//Close semaphores and deallocate shared mem
	
	sem_close(full);
	sem_close(empty);
	sem_close(mutex);

	sem_unlink("full");
	sem_unlink("empty");
	sem_unlink("mutex");

	munmap(table, sizeof(int));
	close(sharedMem);
	shm_unlink("table");
	//shm_unlink(test);
	return 0;
}
