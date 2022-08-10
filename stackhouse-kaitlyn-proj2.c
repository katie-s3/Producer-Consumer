//Kaitlyn Stackhouse
//COMP323 Project 2

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "buffer.h"

int secs;
int in = 0, out = 0;

buffer_item buffer[BUFFER_SIZE];

sem_t empty, full;
pthread_mutex_t mutex;


int insert_item(buffer_item item) {
  int ret = 0;

  sem_wait(&empty);
  pthread_mutex_lock(&mutex);
    
  //insert item into buffer
  buffer[in] = item;

  //check that item was inserted correctly
  if (buffer[in] != item) {
    ret = -1;
  }

  //increment in
  in = (in + 1) % BUFFER_SIZE;

  pthread_mutex_unlock(&mutex);
  sem_post(&full);

  return ret;
}

int remove_item() {

  buffer_item item;

  sem_wait(&full);
  pthread_mutex_lock(&mutex);

  //check that out is not indexing an empty buffer
  if (buffer[out] == -1) {
    item = -1;
  }
  //consumer item at buffer[out]
  else {
    item = buffer[out];
  }

  //increment out
  out = (out + 1) % BUFFER_SIZE;

  pthread_mutex_unlock(&mutex);
  sem_post(&empty);

  return item;
}


void *producer(void *arg) {

  buffer_item item;
  int id = *((int *) arg);
  
  while (true) {

    //sleep for 1 to 3 seconds
    sleep(rand() % (3 + 1 - 1) + 1);

    //Generate a random number
    item = rand() % (10 + 1 - 1) + 1;

    //insert number into buffer
    if (insert_item(item) == -1) {
      printf("Synchronization Error: Producer %d overwrote %d at slot %d\n", id, buffer[id], in);
    }
    else {
      printf("Producer %d produced %d at slot %d\n", id, item, in);
    }
    
  }
}

void *consumer(void *arg) {

  int id = *((int *) arg);
  
  while (true) {

    //sleep for 1 to 3 seconds
    sleep(rand() % (3 + 1 - 1) + 1);

    //remove item from buffer
    buffer_item item = remove_item();
    if (item == -1) {
      printf("Synchronization Error: Consumer %d read from empty slot %d\n", id, out);
    }
    else {
      printf("Consumer %d consumed %d at slot %d\n", id, item, out);
    }

  }
}

int main(int argc, char *argv[]) {
  printf("Kaitlyn Stackhouse\nProject 2\n---------------------\n\n");

  //get seconds to sleep
  char *a = argv[1];
  secs = atoi(a);

  //get number of producer and consumer threads
  char *b = argv[2];
  int prod = atoi(b);
  
  char *c = argv[3];
  int cons = atoi(c);

  //initialize buffer with default values
  for (int i = 0; i < BUFFER_SIZE; i++) {
    buffer[i] = -1;
  }

  //initialize producer and consumer threads
  pthread_t tidp[prod], tidc[cons];

  //create mutex lock
  pthread_mutex_init(&mutex, NULL);

  //create empty and full semaphores
  sem_init(&empty, 0, BUFFER_SIZE);
  sem_init(&full, 0, 0);

  //IDs to track producers and consumers
  int prodId[prod];
  int consId[cons];
  for (int i = 1; i <= prod; i++) {
    prodId[i] = i;
  }
  for (int i = 1; i <= cons; i++) {
    consId[i] = i;
  }

  //create producer threads
  for (int i = 1; i <= prod; i++) {
    pthread_create(&tidp[i], NULL, producer, (void *) &prodId[i]);
  }
  //create consumer threads
  for (int i = 1; i <= cons; i++) {
    pthread_create(&tidc[i], NULL, consumer, (void *) &consId[i]);
  }

  //sleep for given number of seconds
  sleep(secs);

  //exit
  exit(0);
}
