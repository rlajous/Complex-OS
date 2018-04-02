#include "fileListParser.h"

char ** parseFileList(int argc, char ** argv, int* numOfFiles) {

	char ** filenames;
	*numOfFiles = 0;
	struct stat statBuffer;
	int i;

	filenames = (char **) malloc(sizeof(char*) * MEMORY_BLOCK);

	if (filenames == NULL){
		perror("Error allocating memory.");
		exit(1);
	}

	for (i = 1; i < argc; i++) {
		if (stat(argv[i], &statBuffer) == 0 && !S_ISDIR(statBuffer.st_mode)) {
			if (strEndsWithAsterix(argv[i])) {
				removeAsterixFromStr(argv[i]);
				addDirectoryFilesToList(argv[i], &filenames, numOfFiles);
			}
			else {
				addFileToList(argv[i], &filenames, numOfFiles);
			}
		}
	}

	/*printf("Files to be hashed: \n");
	for (j = 0; j < *numOfFiles; j++) {
		printf("%s\n", filenames[j]);
	}*/

	return filenames;
}

void addFileToList(char* fileName, char*** list, int* numOfFiles) {
	if (*numOfFiles % MEMORY_BLOCK == 0) {
		*list = realloc(*list, sizeof(char*) * (*numOfFiles + MEMORY_BLOCK));
		if (*list == NULL){
			perror("Error allocating memory.");
			exit(1);
		}
	}
	(*list)[(*numOfFiles)++] = fileName;
}

void addDirectoryFilesToList(char* dirPath, char*** filenames, int* numOfFiles) {
	DIR * dirp = opendir(dirPath);

	if (dirp != NULL) {
		struct dirent *direntry;
		while ((direntry = readdir(dirp)) != NULL) {
			if (direntry->d_type == DT_REG) {
				addFileToList(strcat(dirPath, strcat("/", direntry->d_name)), filenames, numOfFiles);
			}
			else if (direntry->d_type == DT_DIR) {
				printf("Files from the %s directory have NOT been included.\n", direntry->d_name);
			}
		}
		closedir(dirp);
	}
}

int strEndsWithAsterix(char* str) {
	int i = 0;
	while (str[i++] != '\0');
	return str[--i] == '*';
}

void removeAsterixFromStr(char* str) {
	int i = 0;
	while (str[i++] != '\0');
	str[--i] = '\0';
}
