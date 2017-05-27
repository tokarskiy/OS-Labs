

/*
 *	OS Lab #1. 
 *	The dining philosophers problem solving using 
 *	mutex. OS Linux.
 *
 *	Author: Andriy Tokarskiy
 *	IASA, DA-32, Variant #21
 *
 *	compile this using g++ -pthread lab_1_philosophers.cpp -o philosophers
 */

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <stdio.h>

#define PHILOSOPHERS_COUNT 5

#define MIN_THINKING_TIME 500000
#define MAX_THINKING_TIME 1000000
#define MIN_EATING_TIME 500000
#define MAX_EATING_TIME 1000000

pthread_mutex_t resourses[PHILOSOPHERS_COUNT];
pthread_t threads[PHILOSOPHERS_COUNT]; 

void* PhilosopherTask(void* id);
void Think(long long id);
void Eat(long long id);

int main(){
	srand(time(NULL));
	for (int i = 0; i < PHILOSOPHERS_COUNT; i++){
		if (pthread_mutex_init(&resourses[i], NULL) != 0){
			std::cout << "Mutex init failed!\n";
			return 1;
		}
	}

	for (long long i = 0; i < PHILOSOPHERS_COUNT; i++){
		if (pthread_create(&threads[i], NULL, PhilosopherTask, (void*)i) != 0){
			std::cout << "Threads creation failed!\n";
			return 1;
		}
	}

	for (int i = 0; i < PHILOSOPHERS_COUNT; i++){
		pthread_join(threads[i], NULL);
	}

	for (int i = 0; i < PHILOSOPHERS_COUNT; i++){
		pthread_mutex_destroy(&resourses[i]);
	}
}

void* PhilosopherTask(void* id){
	long long philosopher_id = (long long)id;
	long long resourse1_id = (long long)id;
	long long resourse2_id = ((long long)id + 1) % PHILOSOPHERS_COUNT;

	while(true) {
		Think(philosopher_id);
		if (pthread_mutex_trylock(&resourses[resourse1_id]) != 0){
			continue;
		}
		if (pthread_mutex_trylock(&resourses[resourse2_id]) != 0){
			pthread_mutex_unlock(&resourses[resourse1_id]);
			continue;
		}
		Eat(philosopher_id);
		pthread_mutex_unlock(&resourses[resourse2_id]);
		pthread_mutex_unlock(&resourses[resourse1_id]);
	}
}

void Think(long long id){
	int min = MIN_THINKING_TIME;
	int max = MAX_THINKING_TIME;

	usleep(rand() % (max - min) + min);
}

void Eat(long long id){
	int min = MIN_EATING_TIME;
	int max = MAX_EATING_TIME;

	printf("Philosopher #%lld start eating.\n", id);
	usleep(rand() % (max - min) + min);
	printf("Philosopher #%lld end eating.\n", id);
}
