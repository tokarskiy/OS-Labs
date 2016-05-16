
/*
 *	OS Lab #3.
 *	Task:
 *  Написать утилиту, начинающую работу в заданном каталоге и спускающуюся по дереву каталогов, 
 *  записывая по пути размеры всех встретившихся ей файлов. Закончив сканирование каталога, программа 
 *  должна распечатать гистограмму размеров файлов, используя шаг гистограммы в качестве 
 *  параметра (например, при шаге 1024, файлы размером от 0 до 1023 байт попадают в одни интервал, 
 *  от 1024 до 2047 байт — в следующий интервал и т.д.).
 *
 *  OS Windows
 *
 *	Author: Andriy Tokarskiy
 *	IASA, DA-32, Variant #21
 */

#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <vector>
#include <string>

#define DIAGRAM_STEP 10000

void AddFilesSizeToVector(std::string directory, std::string filter, std::vector<long>& sizes, std::vector<std::string>& fileNames);

int main(int argc, char **argv) {

	std::vector<std::string> fileNames;
	std::vector<long> sizes;
	AddFilesSizeToVector(".\\", "*.*", sizes, fileNames);

	for (int i = 0; i < sizes.size(); i++) {
		long a = sizes[i] / DIAGRAM_STEP;
		printf("[");
		for (long i = 0; i < a; i++) {
			printf("|");
		}
		if (sizes[i] % DIAGRAM_STEP != 0) {
			printf("|");
		}
		printf("] - %s\n", fileNames[i].c_str());
	}

	system("pause");
	return 0;
}


#pragma warning(disable: 4996)
void AddFilesSizeToVector(std::string directory, std::string filter, std::vector<long>& sizes, std::vector<std::string>& fileNames) {
	WIN32_FIND_DATA fileData;
	HANDLE fileHandle = FindFirstFile((directory + filter).c_str(), &fileData);

	if (fileHandle != INVALID_HANDLE_VALUE) {
		do {
			std::string fileName(fileData.cFileName);
			if (fileName == ".." || fileName == ".") {
				continue;
			}
			if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				AddFilesSizeToVector((directory + fileName + "\\").c_str(), filter, sizes, fileNames);
				continue;
			}
			// The size of the file equals (nFileSizeHigh * (MAXDWORD + 1)) + nFileSizeLow.
			//printf("%s\n", fileData.cFileName);
			sizes.push_back((fileData.nFileSizeHigh * (MAXDWORD + 1)) + fileData.nFileSizeLow);
			fileNames.push_back(fileName);
		} while (FindNextFile(fileHandle, &fileData) != 0);
		FindClose(fileHandle);
	}
}
