
/*
 *	OS Lab #3.
 *	Task:
 *  	Написать утилиту, начинающую работу в заданном каталоге и спускающуюся по дереву каталогов, 
 *  	записывая по пути размеры всех встретившихся ей файлов. Закончив сканирование каталога, программа 
 *  	должна распечатать гистограмму размеров файлов, используя шаг гистограммы в качестве 
 *  	параметра (например, при шаге 1024, файлы размером от 0 до 1023 байт попадают в одни интервал, 
 *  	от 1024 до 2047 байт — в следующий интервал и т.д.).
 *
 *  	OS Linux
 *
 *	Author: Andriy Tokarskiy
 *	IASA, DA-32, Variant #21
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <limits.h>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#define DIAGRAM_STEP 1024

void readDirectory (std::string dirName, 
					std::vector<long>& sizes, 
					std::vector<std::string>& names) {
	DIR* dir = NULL;
	struct dirent entry;
	struct dirent *entryPointer = NULL;
	char pathName[PATH_MAX];

	dir = opendir(dirName.c_str());
	if (dir == NULL){
		printf("Error opening %s\n", dirName.c_str());
	}

	readdir_r(dir, &entry, &entryPointer);
	while (entryPointer != NULL){
		struct stat entryInfo;

		int compare1 = strncmp(entry.d_name, ".", PATH_MAX);
		int compare2 = strncmp(entry.d_name, "..", PATH_MAX);
		// ignore . and .. folders
		if (compare1 == 0 || compare2 == 0){
			//next file
			readdir_r(dir, &entry, &entryPointer);
			continue;
		}

		strncpy( pathName, dirName.c_str(), PATH_MAX);
		strncat( pathName, "/", PATH_MAX );
        strncat( pathName, entry.d_name, PATH_MAX );

        if (lstat(pathName, &entryInfo) == 0){
        	// if it is folder
        	if (S_ISDIR(entryInfo.st_mode)){
        		std::string dirName(pathName);
        		readDirectory(dirName, sizes, names);
        	}
        	// if it is a simple file
        	else if (S_ISREG(entryInfo.st_mode)){
        		sizes.push_back((int)entryInfo.st_size);
        		std::string fileName(pathName); 
        		names.push_back(fileName);
        	}
        }
        else{
        	printf("Error statting %s", pathName);
        }
        // next file
        readdir_r(dir, &entry, &entryPointer);
	}
	closedir(dir);
}

int main(int argc, char* argv[]){
	std::vector<long> sizes;
	std::vector<std::string> names;

	readDirectory(".", sizes, names);

	for (int i = 0; i < sizes.size(); i++) {
		long a = sizes[i] / DIAGRAM_STEP;
		printf("[");
		for (long j = 0; j < a; j++) {
			printf("-");
		}
		if (sizes[i] % DIAGRAM_STEP != 0) {
			printf("-");
		}
		printf("] - %s\n", names[i].c_str());
	}

	return 0;
}
