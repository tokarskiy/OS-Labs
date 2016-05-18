
#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <vector>
#include <string>

#define DIAGRAM_STEP 10000

void AddFilesInfoToVector(std::string directory, 
		std::string filter, 
		std::vector<std::string>& names,
		std::vector<bool>& isDirectory);

int main(int argc, char **argv) {
	std::vector<std::string> fileNames;
	std::vector<bool> isDirectory;

	if (argc == 1) {
		AddFilesInfoToVector(".\\", "*.*", fileNames, isDirectory);
	}
	else {
		for (int i = 1; i < argc; i++) {
			std::string current(argv[1]);
			AddFilesInfoToVector(current + "\\", "*.*", fileNames, isDirectory);
		}
	}

	

	for (int i = 0; i < fileNames.size(); i++) {
		printf("%40s %6s\n", fileNames[i].c_str(), isDirectory[i] ? "FOLDER" : "FILE");
	}

	system("pause");
	return 0;
}


void AddFilesInfoToVector(std::string directory,
		std::string filter,
		std::vector<std::string>& names,
		std::vector<bool>& isDirectory) {
	WIN32_FIND_DATA fileData;
	HANDLE fileHandle = FindFirstFile((directory + filter).c_str(), &fileData);

	if (fileHandle != INVALID_HANDLE_VALUE) {
		do {
			std::string fileName(fileData.cFileName);
			if (fileName == ".." || fileName == ".") {
				continue;
			}
			isDirectory.push_back((bool)(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY));
			names.push_back(fileName);
		} while (FindNextFile(fileHandle, &fileData) != 0);
		FindClose(fileHandle);
	}
}
