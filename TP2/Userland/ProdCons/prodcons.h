#ifndef PRODCONS_H
#define PRODCONS_H

int processControl(char option);

void killAllProcesses();

int producer(int argc, char * argv[]);

int consumer(int argc, char * argv[]);

#endif