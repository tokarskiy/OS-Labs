

/*
 *	OS Lab #3.
 *  	My 'ls' command 
 *  	от 1024 до 2047 байт — в следующий интервал и т.д.).
 *
 *  	OS Windows
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

struct FileInfo {
	std::string name;
	bool isFolder;
	bool readOnly;
	bool hidden;
	long size;
	FILETIME creationTime;

	void Print() {
		SYSTEMTIME time, timeLocal;
		FileTimeToSystemTime(&creationTime, &time);
		SystemTimeToTzSpecificLocalTime(NULL, &time, &timeLocal);

		printf("%30s %6s %8s %6s", name.c_str(), isFolder ? "FOLDER" : "------",
							readOnly ? "READONLY" : "--------",
							hidden ? "HIDDEN" : "------");
		if (isFolder) {
			printf(" %9s    ", "");
		}
		else {
			printf(" %9ld    ", size);
		}

		printf("%2d/%2d/%d  %02d:%02d\n",
			timeLocal.wDay, timeLocal.wMonth, timeLocal.wYear,
			timeLocal.wHour, timeLocal.wMinute);

	}
};

void PrintAll(std::vector<FileInfo>& files) {
	std::vector<FileInfo>::iterator at = files.begin();
	for (; at != files.end(); at++) {
		at->Print();
	}
}

void AddFilesInfoToVector(std::string directory, 
		std::string filter, 
		std::vector<FileInfo>&);

int main(int argc, char **argv) {
	std::vector<FileInfo> files;
	std::vector<bool> isDirectory;

	if (argc == 1) {
		AddFilesInfoToVector(".\\", "*.*", files);
	}
	else {
		for (int i = 1; i < argc; i++) {
			std::string current(argv[i]);
			AddFilesInfoToVector(current + "\\", "*.*", files);
		}
	}
	
	PrintAll(files);

	system("pause");
	return 0;
}


void AddFilesInfoToVector(std::string directory,
		std::string filter,
		std::vector<FileInfo>& files) {
	WIN32_FIND_DATA fileData;
	HANDLE fileHandle = FindFirstFile((directory + filter).c_str(), &fileData);

	if (fileHandle != INVALID_HANDLE_VALUE) {
		do {
			std::string fileName(fileData.cFileName);
			if (fileName == ".." || fileName == ".") {
				continue;
			}
			FileInfo info;
			info.isFolder = (bool)(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
			info.hidden = (bool)(fileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN);
			info.readOnly = (bool)(fileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY);
			info.name = fileName;
			info.size = fileData.nFileSizeHigh * (MAXDWORD + 1) + fileData.nFileSizeLow;
			info.creationTime = fileData.ftCreationTime;
			files.push_back(info);
		} while (FindNextFile(fileHandle, &fileData) != 0);
		FindClose(fileHandle);
	}
}
