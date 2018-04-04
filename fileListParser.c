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
			addFileToList(argv[i], &filenames, numOfFiles);
		}
	}

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