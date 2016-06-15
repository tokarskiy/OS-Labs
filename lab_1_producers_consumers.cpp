
/*
 *	OS Lab #1.
 *	The producers-consumers problem solving using 
 *	events. OS Linux.
 *
 *	Author: Andriy Tokarskiy
 *	IASA, DA-32, Variant #21
 *
 *	compile this using g++ -pthread prodcons.cpp -o prodcons
 */

#include "List.h"
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <stdio.h>

#define PRODUCERS_COUNT 2
#define CONSUMERS_COUNT 3
#define MAX_LIST_SIZE 10

#define MIN_PRODUCER_DELAY 500000
#define MAX_PRODUCER_DELAY 1000000
#define MIN_CONSUMER_DELAY 5000000
#define MAX_CONSUMER_DELAY 10000000

void* ProducerTask(void* id);
void* ConsumerTask(void* id);
void Sleep(int min, int max);

List<int>* resources;
int resourcesCounter;

pthread_mutex_t mutex;
pthread_cond_t producer_busy;
pthread_cond_t consumer_busy;
pthread_cond_t queue_not_empty;
pthread_cond_t queue_not_full;
pthread_mutex_t adding;
pthread_mutex_t reading;

int main(){
	resources = new List<int>();
	resourcesCounter = 0; 
	pthread_t producer_threads[PRODUCERS_COUNT]; 
	pthread_t consumer_threads[CONSUMERS_COUNT]; 

	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&adding, NULL);
	pthread_mutex_init(&reading, NULL);
	//pthread_cond_init(&producer_busy, NULL);
	//pthread_cond_init(&consumer_busy, NULL);
	pthread_cond_init(&queue_not_full, NULL);
	pthread_cond_init(&queue_not_empty, NULL);

	for (long long i = 0; i < PRODUCERS_COUNT; i++){
		if (pthread_create(&producer_threads[i], NULL, ProducerTask, (void*)i) != 0){
			std::cout << "Threads creation failed!\n";
			return 1;
		}
	}

	for (long long i = 0; i < CONSUMERS_COUNT; i++){
		if (pthread_create(&consumer_threads[i], NULL, ConsumerTask, (void*)i) != 0){
			std::cout << "Threads creation failed!\n";
			return 1;
		}
	}

	for (int i = 0; i < PRODUCERS_COUNT; i++){
		pthread_join(producer_threads[i], NULL);
	}

	for (int i = 0; i < CONSUMERS_COUNT; i++){
		pthread_join(consumer_threads[i], NULL);
	}

	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&adding);
	pthread_mutex_destroy(&reading);
	//pthread_cond_destroy(&producer_busy);
	//pthread_cond_destroy(&consumer_busy);
	pthread_cond_destroy(&queue_not_full);
	pthread_cond_destroy(&queue_not_empty);
}

void* ProducerTask(void* id){
	long long producer_id = (long long)id;

	while (true){
		pthread_mutex_lock(&adding);
		if (resources->Size() == MAX_LIST_SIZE){
			pthread_cond_wait(&queue_not_full, &mutex);
		}
		resources->PushBack(resourcesCounter);
		pthread_cond_signal(&queue_not_empty);
		printf("The producer #%lld has pushed resource #%d.\n", producer_id, resourcesCounter);
		pthread_mutex_unlock(&adding);
		resourceCounter++;
		Sleep(MIN_PRODUCER_DELAY, MAX_PRODUCER_DELAY);
	}
}

void* ConsumerTask(void* id){
	long long consumer_id = (long long)id;

	while(true){
		pthread_mutex_lock(&reading);
		if (resources->Empty()){
			pthread_cond_wait(&queue_not_empty, &mutex);
		}
		resources->PopFront();
		pthread_cond_signal(&queue_not_full);
		printf("The consumer #%lld has got resource.\n", consumer_id);
		pthread_mutex_unlock(&reading);
		Sleep(MIN_CONSUMER_DELAY, MAX_CONSUMER_DELAY);
	}
}

void Sleep(int min, int max){
	usleep(rand() % (max - min) + min);
}
