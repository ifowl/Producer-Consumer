# Producer-Consumer
OS Programming Assignment #1 - Documentation is provided via comments within code
This program aims to display the power of shared memory while solving the Producer Consumer problem using multiprocess synchronization. See instructions below for compilation and execution.

##### Step 1:
Clone repository:
> git clone git@github.com:ifowl/Producer-Consumer.git

##### Step 2:
Compile code by using:
> g++ producer.cpp -pthread -lrt -o producer 
> 
> g++ consumer.cpp -pthread -lrt -o consumer
> 
or 
> clang++ producer.cpp -pthread -lrt -o producer
> 
> clang++ consumer.cpp -pthread -lrt -o consumer

##### Step 3:
Run executables symultaneously:
> ./producer & ./consumer &

## Example output of program:

![Result](https://i.imgur.com/lsIDx8H.png)
