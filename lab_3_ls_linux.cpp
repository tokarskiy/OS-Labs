
/*
 *	OS Lab #3.
 *  	My 'ls' command 
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

void readDirectory (std::string dirName, 
		std::vector<std::string>& names,
		std::vector<bool>& isFolder) {
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

        std::string fileName(entry.d_name); 
        

        if (lstat(pathName, &entryInfo) == 0){
        	// if it is folder
        	names.push_back(fileName);
        	isFolder.push_back(S_ISDIR(entryInfo.st_mode));
        }
        else{
        	continue;
        }
        // next file
        readdir_r(dir, &entry, &entryPointer);
	}
	closedir(dir);
}

int main(int argc, char* argv[]){
	std::vector<std::string> names;
	std::vector<std::string> folders;
	std::vector<bool> isFolder;
	if (argc == 1){
		folders.push_back(".");
	}
	else
	{
		for (int i = 1; i < argc; i++){
			std::string folderName(argv[i]);
			folders.push_back(folderName);
		}
	}

	for (int i = 0; i < folders.size(); i++){
		readDirectory(folders[i], names, isFolder);
	}

	for (int i = 0; i < names.size(); i++) {
		printf("%40s %6s\n", names[i].c_str(), isFolder[i] ? "FOLDER" : "FILE");
	}

	return 0;
}
