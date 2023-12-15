#include "base_sqlite.h"

bool BD::openBd(const char* bdName) {
    int status = sqlite3_open(bdName, &bd);
    assert(status == SQLITE_OK);
    return status;
}

bool BD::dropTable(const char* bdName){
	int status = request_insert_create(sqlite3_mprintf("DROP TABLE IF EXISTS %q;", bdName));
	assert(status == SQLITE_OK);
	return status;
}

bool BD::closeBd() {
    int status = sqlite3_close(bd);
    assert(status == SQLITE_OK);
    return status;
}

bool BD::request_insert_create(const char* sqlString) {
    char* errMsg;
    int er = sqlite3_exec(bd, sqlString, nullptr, nullptr, &errMsg);
    if (er) {
        std::cerr << "error request " << sqlString << " : " << errMsg << std::endl;      
	}
    return er;
}

bool BD::request_select(const char* sqlString, int (*callback)(void*, int, char**, char**)) {
	char* errMsg;
	int er = sqlite3_exec(bd, sqlString, callback, nullptr, &errMsg);
	
	if (er != SQLITE_OK){
		std::cerr << "error request " << sqlString << " : " << errMsg << std::endl;
	}
	assert(er == SQLITE_OK);
	return er;
}

void BD::turn_on_keys(){
	request_insert_create("PRAGMA FOREIGN_KEYS = ON;");
}
