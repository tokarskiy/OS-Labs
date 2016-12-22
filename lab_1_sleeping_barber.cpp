
/*
 *	OS Lab #1.
 *	The sleeping barber problem solving using
 *	semaphores. OS Windows.
 *
 *	Author: Andriy Tokarskiy
 *	IASA, DA-32, Variant #21
 */
#include <Windows.h>
#include <iostream>

#define QUEUE_SIZE 5
#define CLIENTS_COUNT 10

#define MIN_CUSTOMER_ENTER_DELAY 200 
#define MAX_CUSTOMER_ENTER_DELAY 500
#define MIN_CUTTING_TIME 500
#define MAX_CUTTING_TIME 1000

/*#define MIN_CUSTOMER_ENTER_DELAY 750
#define MAX_CUSTOMER_ENTER_DELAY 1500
#define MIN_CUTTING_TIME 200
#define MAX_CUTTING_TIME 500*/

typedef HANDLE Semaphore;

int Random(int min, int max);
void SleepRandomTime(int min, int max);
DWORD WINAPI BarberTask(LPVOID arg);
void CutHair();
DWORD WINAPI ClientTask(LPVOID id);
void Release(Semaphore semaphore);
void Wait(Semaphore semaphore);

Semaphore clients_semaphore;
Semaphore barber_semaphore;
int waiting_clients_count;
int free_seats = 5;
Semaphore counter_semaphore;

int main() {
	waiting_clients_count = 0;
	barber_semaphore = CreateSemaphore(NULL, 0, 1, NULL);
	clients_semaphore = CreateSemaphore(NULL, 0, QUEUE_SIZE, NULL);
	counter_semaphore = CreateSemaphore(NULL, 1, 1, NULL);

	CreateThread(NULL, 0, BarberTask, NULL, 0, NULL);
	for (int i = 1; i <= CLIENTS_COUNT; i++) {
		if(i!=1)free_seats-=1;
		CreateThread(NULL, 0, ClientTask, (LPVOID)i, 0, NULL);
		SleepRandomTime(MIN_CUSTOMER_ENTER_DELAY, MAX_CUSTOMER_ENTER_DELAY);
	}

	int a;
	std::cin >> a;
	return 0;
}

int Random(int min, int max) {
	return rand() % (max - min) + min;
}

void SleepRandomTime(int min, int max) {
	Sleep(Random(max, min));
}

DWORD WINAPI BarberTask(LPVOID arg) {
	while (true) {
		Wait(counter_semaphore);
		if (waiting_clients_count == 0) {
			Sleep(500);
			printf("Barber is sleeping.\n");
		}
		Release(counter_semaphore);

		Wait(clients_semaphore);
		Wait(counter_semaphore);
		waiting_clients_count--;
		Release(counter_semaphore);
		SleepRandomTime(MIN_CUTTING_TIME, MAX_CUTTING_TIME);
		Release(barber_semaphore); //cutting hair
		//printf("Barber is cutting hair.\n");
		
	}
	return 0;
}

DWORD WINAPI ClientTask(LPVOID id) {
	int client_id = (int)id;
	Wait(counter_semaphore);
	if (waiting_clients_count < QUEUE_SIZE-1) {
		//int seats_left = QUEUE_SIZE - waiting_clients_count;
		waiting_clients_count++;
		Release(counter_semaphore);
		printf("Customer #%d has entered a barber shop [%d]\n", client_id, free_seats); //waiting client counts still false
		
		Release(clients_semaphore);
		Wait(barber_semaphore);
		free_seats+=1;
		printf("Barber is cutting hair of customer #%d\n", client_id);
	}
	else {
		Release(counter_semaphore);
		printf("Customer #%d has NOT entered a barber shop.\n", client_id);
	}
	return 0;
}

void GetHaircut(int id) {
	printf("Customer #%d is getting his haircut.\n", id);
	Sleep(10000);
}

void Release(Semaphore semaphore) {
	ReleaseSemaphore(semaphore, 1, NULL);
}

void Wait(Semaphore semaphore) {
	WaitForSingleObject(semaphore, INFINITE);
}
