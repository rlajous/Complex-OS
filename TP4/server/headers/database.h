#ifndef DATABASE_H
#define DATABASE_H

#define ERROR -1
#define OK 0

int dbOpen();

void createTables();

int newMovie(const char * name, const char * description);

int newScreening(const char * movie, const char * dateTime, int theater);

int deleteMovie(const char * name);

#endif
