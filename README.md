# Producer-Consumer
This project uses multithreading and a combination of semaphores and mutex locks to solve the Producer-Consumer synchronization problem. 

The program accepts three command line arguments: the number of seconds to sleep, the number of producer threads and the number of consumer threads. The producer and consumer threads alternate between sleeping for a random period of time and inserting or removing an item from the buffer. The main() function initializes the buffer, which is defined in buffer.h, creates the producer and consumer threads, sleeps for the given number of seconds, and then terminates. 
