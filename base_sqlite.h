#ifndef USEPROFILER_BASE_SQLITE_H
#define USEPROFILER_BASE_SQLITE_H

#include <iostream>
#include <cstring>
#include <assert.h>

#include "sqlite3.h"

class BD{
	protected:
	   sqlite3 *bd;	//handle of db
	public:
		BD(){};
		~BD(){};
		bool openBd(const char* bdName);
		bool dropTable(const char* bdName);
		bool closeBd();
		void turn_on_keys();
		bool request_insert_create(const char* sqlString);
		bool request_select(const char* sqlString, int (*callback)(void*, int, char**, char**));
};

#endif //USEPROFILER_BASE_SQLITE_H
