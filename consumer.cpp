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
	sharedMem = shm_open("table", O_CREAT | O_RDWR, 0666);
	ftruncate(sharedMem,sizeof(int));
	table = static_cast<int*>(mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, sharedMem, 0));

	//Semaphores used to keep track of table buffer and make the consumer run synchronusly
	sem_t* full = sem_open("full", O_CREAT, 0666, 0);
	sem_t* empty = sem_open("empty", O_CREAT, 0666, 3);
	sem_t* mutex = sem_open("mutex", O_CREAT, 0666, 1);

	//For loop that runs 8 iterations (Producer will produce 8 items)
	for (int i = 0; i < 8; ++i) {
		sem_wait(full); //Increment full semaphore

		sleep(rand()%2+1);

		sem_wait(mutex); //Unlock mutex
		if (*table > 0) {
			--(*table);
			std::cout << "Item consumed." << std::endl << "Table contains " << *table << " items\n";
		} else {
			std::cout << "Buffer is empty!\n";
		}
		sem_post(mutex); //Close mutex
		sem_post(empty); //Decrement empty semaphore
	}

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

	return 0;
}
