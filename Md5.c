#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void md5(char * fileName);

int main(int argc, char ** argv)
{
	md5(argv[1]);
}

void md5(char * fileName){
 	char command[50] = "md5 ";
 	strcat(command,fileName);
	system(command);
 }