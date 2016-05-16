/*
 *	OS Lab #3.
 *	Task:
 *  	Напишите программу, которая выводит для файлов, имена которых указаны в качестве параметров, 
 *	информацию о владельце, типе файла, правах доступа и времени доступа. Если файл (параметр) является 
 *	каталогом, программа должна читать записи из каталога и выводить вышеуказанную информацию для всех 
 * 	файлов в каталоге.
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
	long size;
	bool isHidden;
	bool isReadonly;
	std::string type;
};

void AddFilesInfoToVector(std::string directory, std::string filter, std::vector<FileInfo>& info);

int main(int argc, char **argv) {

	std::vector<std::string> fileNames;
	std::vector<FileInfo> info;
	AddFilesInfoToVector(".\\", "*.*", info);

	for (int i = 0; i < info.size(); i++) {
		std::cout << info[i].name << std::endl;
		std::cout << "\t Size: " << info[i].size << std::endl;
		std::cout << "\t Hidden: " << (info[i].isReadonly ? "+" : "-") << std::endl;
		std::cout << "\t Readonly: " << (info[i].isHidden ? "+" : "-") << std::endl;
		std::cout << std::endl;
	}

	system("pause");
	return 0;
}


void AddFilesInfoToVector(std::string directory, std::string filter, std::vector<FileInfo>& info) {
	WIN32_FIND_DATA fileData;
	HANDLE fileHandle = FindFirstFile((directory + filter).c_str(), &fileData);

	if (fileHandle != INVALID_HANDLE_VALUE) {
		do {
			std::string fileName(fileData.cFileName);
			if (fileName == ".." || fileName == ".") {
				continue;
			}
			if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				AddFilesInfoToVector((directory + fileName + "\\").c_str(), filter, info);
				continue;
			}
			// The size of the file equals (nFileSizeHigh * (MAXDWORD + 1)) + nFileSizeLow.
			//info.push_back((fileData.nFileSizeHigh * (MAXDWORD + 1)) + fileData.nFileSizeLow);
			FileInfo current;
			current.name = fileName;
			current.size = (fileData.nFileSizeHigh * (MAXDWORD + 1)) + fileData.nFileSizeLow;
			current.isReadonly = fileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY;
			current.isHidden = fileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN;
			info.push_back(current);
		} while (FindNextFile(fileHandle, &fileData) != 0);
		FindClose(fileHandle);
	}
}
