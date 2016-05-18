
/*
 *	OS Lab #3.
 *	Task:
 *  	Написать утилиту, начинающую работу в заданном каталоге и спускающуюся по дереву каталогов, 
 *  	записывая по пути размеры всех встретившихся ей файлов. Закончив сканирование каталога, программа 
 *  	должна распечатать гистограмму размеров файлов, используя шаг гистограммы в качестве 
 *  	параметра (например, при шаге 1024, файлы размером от 0 до 1023 байт попадают в одни интервал, 
 *  	от 1024 до 2047 байт — в следующий интервал и т.д.).
 *
 *  	OS Windows
 *
 *	Author: Andriy Tokarskiy
 *	IASA, DA-32, Variant #21
 */

#include <string>
#include <map>

#define DIAGRAM_STEP 1
#define SIZE_STEP 2000

struct FileInfo {
	std::string name;
	long size;
	bool isDirectory;
	
};

void AddFilesSizeToVector(std::string directory, std::string filter, std::vector<long>& sizes, std::vector<std::string>& fileNames);

int main(int argc, char **argv) {
	std::map<long, int> statistic;
	std::vector<std::string> fileNames;
	std::vector<long> sizes;
	AddFilesSizeToVector(".\\", "*.*", sizes, fileNames);

	for (int i = 0; i < sizes.size(); i++) {
		if (statistic.find(sizes[i] / SIZE_STEP) != statistic.end()) {
			statistic[sizes[i] / SIZE_STEP]++;
		}
		else {
			statistic.insert(std::pair<long, int>(sizes[i] / SIZE_STEP, 1));

		}
		//printf("%ld %d\n", sizes[i] / 1024, statistic[sizes[i] / 1024]);
	}

	std::map<long, int>::iterator at = statistic.begin();

	for (; at != statistic.end(); ++at) {
		if (at->second == 0) {
			continue;
		}
		printf("%ld - %ld\n", at->first * 1024, (at->first + 1) * 1024);
		printf("[");
		for (int j = 0; j < at->second; j += DIAGRAM_STEP) {
			printf("=");
		}
		printf("] - %d files.\n", at->second);
	}

	system("pause");
	return 0;
}


#pragma warning(disable: 4996)
void AddFilesSizeToVector(std::string directory, 
		std::string filter, 
		std::vector<long>& sizes, 
		std::vector<std::string>& fileNames) {
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
