

#include <process.h>
#include <Windows.h>
#include <iostream>
#include <time.h>

#define READERS_COUNT 3
#define WRITERS_COUNT 1

unsigned long __stdcall ReaderTask(void*); //в этой функции - задача, выполняемая читателями
unsigned long __stdcall WriterTask(void*); //в этой функции - задача, выполняемая питателями
int Random(int min, int max); //генератор рандомных чисел
void SleepRandomTime(int min, int max); //задержка на рандомное время (задержка задается в миллисекундах)

int readersCounter = 0; //количество текущих читателей
int writersCounter = 0; //количество текущих писателей

CRITICAL_SECTION readersBlocker;
CRITICAL_SECTION writersBlocker;
CRITICAL_SECTION readersCounterBlocker;
CRITICAL_SECTION writersCounterBlocker;

int main() {
	HANDLE readers[READERS_COUNT];
	HANDLE writers[WRITERS_COUNT];
	DWORD threadID;

	InitializeCriticalSection(&writersBlocker);
	InitializeCriticalSection(&readersBlocker);
	InitializeCriticalSection(&readersCounterBlocker);
	InitializeCriticalSection(&writersCounterBlocker);

	for (int i = 0; i < WRITERS_COUNT; ++i)
	{
		writers[i] = CreateThread(NULL, 0, WriterTask, (void*)i, 0, &threadID);
	}

	for (int i = 0; i < READERS_COUNT; ++i)
	{
		readers[i] = CreateThread(NULL, 0, ReaderTask, (void*)i, 0, &threadID);
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

unsigned long __stdcall ReaderTask(void* id) {
	int reader_id = (int)id;
	std::cout << "The reader #" << reader_id << " has been initialized.\n";

	while (true) {
		EnterCriticalSection(&readersBlocker);
		EnterCriticalSection(&readersCounterBlocker);
		readersCounter++;
		if (readersCounter == 1) {
			EnterCriticalSection(&writersBlocker);
		}
		LeaveCriticalSection(&readersCounterBlocker);
		LeaveCriticalSection(&readersBlocker);

		std::cout << "The reader #" << reader_id << " has started reading.\n";
		SleepRandomTime(2000, 3000);
		std::cout << "The reader #" << reader_id << " has ended reading.\n";

		EnterCriticalSection(&readersCounterBlocker);
		readersCounter--;
		if (readersCounter == 0) {
			LeaveCriticalSection(&writersBlocker);
		}
		LeaveCriticalSection(&readersCounterBlocker);

		SleepRandomTime(2000, 3000);
	}
	return 0;
}

unsigned long __stdcall WriterTask(void* id) {
	int writer_id = (int)id;
	std::cout << "The writer #" << writer_id << " has been initialized.\n";

	while (true) {
		EnterCriticalSection(&writersCounterBlocker);
		writersCounter++;
		if (writersCounter == 1)
		{
			EnterCriticalSection(&readersBlocker);
		}
		LeaveCriticalSection(&writersCounterBlocker);

		EnterCriticalSection(&writersBlocker);
		std::cout << "The writer #" << writer_id << " has started writing.\n";
		SleepRandomTime(5000, 6000);
		std::cout << "The writer #" << writer_id << " has ended writing.\n";
		LeaveCriticalSection(&writersBlocker);

		EnterCriticalSection(&writersCounterBlocker);
		writersCounter--;
		if (writersCounter == 0)
		{
			LeaveCriticalSection(&readersBlocker);
		}
		LeaveCriticalSection(&writersCounterBlocker);

		SleepRandomTime(10000, 10500);
	}
	return 0;
}

