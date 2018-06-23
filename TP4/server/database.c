#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/sqlite3.h"
#include "headers/database.h"

#define FILENAME "cinema.db"

#define MAX_QUERY_LENGTH 255

static sqlite3 * database = NULL;

int dbOpen() {
  int foreignKey;

  if(database != NULL)
    return OK;

  sqlite3_open(FILENAME, &database);

  if(database == NULL)
    return ERROR;

  foreignKey = sqlite3_exec(database, "PRAGMA foreign_keys = ON", NULL, NULL, NULL);
  if(foreignKey != SQLITE_OK)
    return ERROR;

  return OK;
}

void dbClose() {
  int value;
  value = sqlite3_close(database);

  if(value == SQLITE_OK)
    database = NULL;
}

void createTables() {
  char * query = "create table if not exists movies(\
    name text primary key,\
    description text,\
  );\
  \
  create table if not exists screenings(\
    id int primary key autoincrement,\
    movieName  text,\
    time datetime(),\
    theater int check(theater >= 1),\
    unique(time, theater),\
    foreign key(movieName) references movies(name) on delete cascade\
  );\
  \
  create table if not exists reservations(\
    client text,\
    screeningId int,\
    seat int,\
    primary key(screeningId, seat),\
    foreign key(screeningId) references screenings(id) on delete cascade\
  );\
  \
  create table if not exists cancellations(\
    client text,\
    screeningId int,\
    seat int,\
    primary key(screeningId, seat),\
    foreign key(screeningId) references screenings(id) on delete cascade\
  );";

  sqlite3_exec(database, query, NULL, NULL, NULL);
}

int newMovie(const char * name, const char * description) {
  int value;
  char * query;
  sqlite3_stmt * statement;

  query = "insert into movies values ('%s', '%s');";

  value = sqlite3_prepare_v2(database, query, -1, &statement, NULL);

  if(value != SQLITE_OK) {
    return ERROR;
  }

  sqlite3_bind_text(statement, 1, name, -1, NULL);
  sqlite3_bind_text(statement, 2, description, -1, NULL);

  value = sqlite3_step(statement);
  sqlite3_finalize(statement);

  if(value != SQLITE_DONE)
    return ERROR;

  return OK;
}

int newScreening(const char * movie, const char * dateTime, int theater) {
  int value;
  char * query;
  sqlite3_stmt * statement;

  query = "insert into screenings values ('%s', '%s', %d);";

  value = sqlite3_prepare_v2(database, query, -1, &statement, NULL);

  if(value != SQLITE_OK) {
    return ERROR;
  }

  sqlite3_bind_text(statement, 1, movie, -1, NULL);
  sqlite3_bind_text(statement, 2, dateTime, -1, NULL);
  sqlite3_bind_int(statement, 3, theater);

  value = sqlite3_step(statement);
  sqlite3_finalize(statement);

  if(value != SQLITE_DONE)
    return ERROR;

  return OK;
}

int deleteMovie(const char * name) {
  int value;
  char * query;
  sqlite3_stmt * statement;

  query = "delete from movies where name = '%s';";

  value = sqlite3_prepare_v2(database, query, -1, &statement, NULL);

  if(value == SQLITE_OK) {
    sqlite3_bind_text(statement, 1, name, -1, NULL);

    value = sqlite3_step(statement);
    sqlite3_finalize(statement);
  }

  if(value != SQLITE_DONE)
    return ERROR;

  return OK;
}
