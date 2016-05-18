
/*
 *		OS Lab #3.
 *  	My 'ls' command 
 *  	от 1024 до 2047 байт — в следующий интервал и т.д.).
 *
 *  	OS Linux
 *
 *		Author: Andriy Tokarskiy
 *		IASA, DA-32, Variant #21
 */

#include <string.h>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h> //chdir
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

struct FileInfo{
    std::string name;
    mode_t mode;
    nlink_t nlink;
    std::string uid;
    std::string gid;
    off_t size;
    time_t mtim;
	
	void Print(){
		printf((S_ISDIR(mode)) ? "d" : "-");
        printf((mode & S_IRUSR) ? "r" : "-");
        printf((mode & S_IWUSR) ? "w" : "-");
        printf((mode & S_IXUSR) ? "x" : "-");
        printf((mode & S_IRGRP) ? "r" : "-");
        printf((mode & S_IWGRP) ? "w" : "-");
        printf((mode & S_IXGRP) ? "x" : "-");
        printf((mode & S_IROTH) ? "r" : "-");
        printf((mode & S_IWOTH) ? "w" : "-");
        printf((mode & S_IXOTH) ? "x" : "-");
        printf("%4d %7s %7s ", nlink, uid.c_str(), gid.c_str());
        time ( &mtim);
		
		printf("%7d ", (int)size);
        printf("%30s   ", name.c_str());
        printf ( "%s", ctime (&mtim) );
	}
};


void FindFiles(char dir[256], std::vector<FileInfo>& files)
{
    DIR *cdir;
    dirent *direntry; // struct
    struct stat fileinfo;
    chdir(dir); //change working directory
    cdir=opendir(dir);
    if(cdir!=NULL)
    {
    	files.clear();
        while((direntry=readdir(cdir))!=NULL)
        {
            if(lstat(direntry->d_name,&fileinfo)!=0){
                continue;
            }
            FileInfo tempDir;
            tempDir.name = direntry->d_name;
            tempDir.mode = fileinfo.st_mode;
            tempDir.nlink = fileinfo.st_nlink;
            struct passwd *pw = getpwuid(fileinfo.st_uid);
            struct group  *gr = getgrgid(fileinfo.st_gid);
            tempDir.uid = std::string(pw->pw_name);
            tempDir.gid = std::string(gr->gr_name);
            tempDir.size = fileinfo.st_size;
            tempDir.mtim = fileinfo.st_mtime;
            
            files.push_back(tempDir);
        }
    }
}

void PrintAll(std::vector<FileInfo>& files){
	std::vector<FileInfo>::iterator at = files.begin();
	for (; at != files.end(); at++){
		at->Print();
	}
}

int main(int argc, char* argv[]){
	std::vector<FileInfo> names;

	FindFiles(argv[1], names);
	PrintAll(names);

	return 0;
}
