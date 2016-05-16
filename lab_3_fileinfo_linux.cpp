/*
 *	OS Lab #3.
 *	Task:
 *  	Напишите программу, которая выводит для файлов, имена которых указаны в качестве параметров, 
 *	информацию о владельце, типе файла, правах доступа и времени доступа. Если файл (параметр) является 
 *	каталогом, программа должна читать записи из каталога и выводить вышеуказанную информацию для всех 
 * 	файлов в каталоге.
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

struct FileInfo{
	std::string name;
	long size;
	int userId;
};

void readDirectory (std::string dirName, 
					std::vector<FileInfo>& info) {
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

		strncpy(pathName, dirName.c_str(), PATH_MAX);
		strncat(pathName, "/", PATH_MAX);
        strncat(pathName, entry.d_name, PATH_MAX);

        if (lstat(pathName, &entryInfo) == 0){
        	// if it is folder
        	if (S_ISDIR(entryInfo.st_mode)){
        		std::string dirName(pathName);
        		readDirectory(dirName, info);
        	}
        	// if it is a simple file
        	else if (S_ISREG(entryInfo.st_mode)){
        		std::string fileName(pathName); 
        		FileInfo current;
        		current.name = fileName;
        		current.size = (long)entryInfo.st_size;
        		current.userId = entryInfo.st_uid;
        		info.push_back(current);
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
	std::vector<FileInfo> info;
	std::string directory = argc == 2 ? argv[1] : ".";

	readDirectory(directory, info);

	for (int i = 0; i < info.size(); i++) {
		printf("%s\n", info[i].name.c_str());
		printf("\tSize: %ld byte;\n", info[i].size);
		printf("\tUser ID: %d;\n", info[i].userId);
		printf("\n");
	}

	return 0;
}
