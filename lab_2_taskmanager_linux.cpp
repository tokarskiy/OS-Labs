#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <vector>
#include <algorithm>
#include <sys/types.h>
#include <string>

struct ProcessInfo{
	std::string name;
	int processId;
	char state;
	int size;
	int rsize;
	int parentId;

	void PrintInfo(){
		printf("%25s%6d%4c%6d Kb%6d Kb %6d\n", 
			name.c_str(),
			processId,
			state,
			size,
			rsize,
			parentId
		);
	}

	static void PrintTableHeader(){
		printf("%25s%6s%4s%9s%9s%9s\n", 
			"NAME",
			"PID",
			"S",
			"VIRT",
			"RES",
			"PARENT"
		);
	}
};

bool CompareProcessesByName(const ProcessInfo& a, const ProcessInfo &b){
	return a.name < b.name;
}

bool CompareProcessesById(const ProcessInfo& a, const ProcessInfo& b){
	return a.processId < b.processId;
}

bool GetProcessesInfo(std::vector<ProcessInfo>& processes){
	processes.clear();

	DIR* directory;
	FILE* file;
	struct dirent* directoryInfo;
	char path[20], command[30], state;
	int processId, size, rsize, parentId;

	directory = opendir("/proc");
	if (directory == NULL){
		printf("Cannot open the directory\n");
		return false;
	}

	strcpy(path, "/proc/meminfo");
	//if (!(file = fopen(path, "r"))){
	//	printf("Error 1\n");
	//	return false;
	//}

	while (directoryInfo = readdir(directory)){
		ProcessInfo temp;
		if (!isdigit(directoryInfo->d_name[0])){
			continue;
		}
		strcpy(path, "/proc/");
		strcat(path, directoryInfo->d_name);
		strcat(path, "/stat");

		if (!(file = fopen(path, "r"))){
			printf("Error 2\n");
			continue;
		}
		fscanf(file, "%d", &temp.processId);
		fscanf(file, "%s", command);
		std::string name(command);
		temp.name = name;
		fscanf(file, "%c", &temp.state);
		fscanf(file, "%c", &temp.state);
		fscanf(file, "%d", &temp.parentId);

		fclose(file);
		strcpy(path, "/proc/");
		strcat(path, directoryInfo->d_name);
        strcat(path, "/statm");
		if (!(file = fopen(path, "r"))){
			printf("Error 3\n");
			continue;
		}

		fscanf(file, "%d", &temp.size);
		fscanf(file, "%d", &temp.rsize);
		temp.size *= 4;
		temp.rsize *= 4;
		processes.push_back(temp);
		fclose(file);
	}
	closedir(directory);
}

void PrintProcesses(const int sort){
	ProcessInfo::PrintTableHeader();
	std::vector<ProcessInfo> processes;
	GetProcessesInfo(processes);

	switch (sort){
		case 1:
			std::sort(processes.begin(), processes.end(), 
				CompareProcessesByName);
			break;
		case 2:
			std::sort(processes.begin(), processes.end(), 
				CompareProcessesById);
			break;
	}

	for (int i = 0; i < processes.size(); i++){
		processes[i].PrintInfo();
	}
}

int main(int argc, char* argv[]){
	if (argc == 2){
		std::string command(argv[1]);
		if (command == "-sortbyname"){
			PrintProcesses(1);
		}
		else if (command == "-sortbyid"){
			PrintProcesses(2);
		}
		else{
			printf("Undefined command: %s\n", argv[1]);
		}
	}
	else{
		PrintProcesses(0);
	}
	return 0;
}
