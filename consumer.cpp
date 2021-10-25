//Isaac Fowler
//OS Programming Assignment #1
//Consumer File
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
	sharedMem = shm_open("table", O_CREAT | O_RDWR, 0666); //create shared memory object "table"
	ftruncate(sharedMem,sizeof(int)); //set size of shared memory
	table = static_cast<int*>(mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, sharedMem, 0)); //map the object to address space

	//Semaphores used to keep track of table buffer and make the consumer run synchronusly
	sem_t* full = sem_open("full", O_CREAT, 0666, 0);
	sem_t* empty = sem_open("empty", O_CREAT, 0666, 3);
	sem_t* mutex = sem_open("mutex", O_CREAT, 0666, 1);

	//For loop that runs 8 iterations (Producer will produce 8 items)
	for (int i = 0; i < 8; ++i) {
		sem_wait(full); //Increment full semaphore

		sleep(rand()%2+1); //sleep for 1-2 seconds

		sem_wait(mutex); //Unlock mutex
		if (*table > 0) { //if the table isn't empty
			--(*table); //consume an item in the table
			std::cout << "Item consumed." << std::endl << "Table contains " << *table << " items\n";
		} else {
			std::cout << "Buffer is empty!\n";
		}
		sem_post(mutex); //Close mutex
		sem_post(empty); //Decrement empty semaphore
	}

	//Close semaphores
	sem_close(full);
	sem_close(empty);
	sem_close(mutex);
	
	//unlink semaphores
	sem_unlink("full");
	sem_unlink("empty");
	sem_unlink("mutex");

	//deallocate shared memory
	munmap(table, sizeof(int));
	close(sharedMem);
	shm_unlink("table");

	return 0;
}
