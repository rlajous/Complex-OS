#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define MEMORY_BLOCK 8

void addFileToList(char* fileName, char** list, int* numOfFiles);
void addDirectoryFilesToList(char* dirPath, char** fileNames, int* numOfFiles);
int strEndsWithAsterix(char* str);
void removeAsterixFromStr(char* str);

int main(int argc, char ** argv){
	char ** fileNames;
	int numOfFiles = 0;

	int i;
	struct stat statBuffer;
	fileNames = (char **) malloc(sizeof(char*) * MEMORY_BLOCK);

	for (i = 1; i < argc; i++){
		if (stat(argv[i], &statBuffer) == 0 && !S_ISDIR(statBuffer.st_mode)){
			if (strEndsWithAsterix(argv[i])){
				removeAsterixFromStr(argv[i]);
				addDirectoryFilesToList(argv[i], fileNames, &numOfFiles);
			}
			else{
				addFileToList(argv[i], fileNames, &numOfFiles);
			}
		}
	}
	for (int j = 0; j < numOfFiles; j++){
		printf("%s\n", fileNames[j]);
	}

}

void addFileToList(char* fileName, char** list, int* numOfFiles){
	if (*numOfFiles % MEMORY_BLOCK == 0){
		list = realloc(list, sizeof(char*) * (*numOfFiles + MEMORY_BLOCK));
		if (list == NULL){
			perror("Error allocating memory.");
			exit(1);
		}
	}
	list[(*numOfFiles)++] = fileName;
}

void addDirectoryFilesToList(char* dirPath, char** fileNames, int* numOfFiles){
	DIR * dirp = opendir(dirPath);
	if (dirp != NULL){
		struct dirent *direntry;
		while ((direntry = readdir(dirp)) != NULL){
			if (direntry->d_type == DT_REG){
				addFileToList(strcat(dirPath, strcat("/", direntry->d_name)), fileNames, numOfFiles);
			}
			else if (direntry->d_type == DT_DIR){
				printf("Files from the %s directory have NOT been included.\n", direntry->d_name);
			}
		}
	}
}
int strEndsWithAsterix(char* str){
	int i = 0;
	while (str[i++] != '\0');
	return str[--i] == '*';
}
void removeAsterixFromStr(char* str){
	int i = 0;
	while (str[i++] != '\0');
	str[--i] = '\0';
}