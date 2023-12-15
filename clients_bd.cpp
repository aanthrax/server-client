#include "clients_bd.h"

void ClientsBD::createClients(){
	request_insert_create("CREATE TABLE IF NOT EXISTS clients(id_client INTEGER PRIMARY KEY, name TEXT, connection_time INTEGER);");
}

int ClientsBD::callback(void* outputStruct, int countRec, char** argv, char** colName){
	for (int i = 0; i < countRec; i++){
		std::cout << "\t" << colName[i] << " '" << argv[i] << "' "; 
	}
	std::cout << std::endl;
	return 0;
}

void ClientsBD::insertClient(int id_client, const char* clientName, int time){
	request_insert_create(sqlite3_mprintf("INSERT INTO clients(id_client, name, connection_time) VALUES (%d, '%q', %d);", id_client, clientName, time));
}

void ClientsBD::deleteClient(int id_client){
	request_insert_create(sqlite3_mprintf("DELETE FROM clients WHERE id_client = %d;", id_client));
}

int ClientsBD::getIdClient(const char* name){
	sqlite3_stmt* statement = nullptr;
	int status = sqlite3_prepare_v2(bd, sqlite3_mprintf("SELECT id_client FROM clients WHERE name = '%q';", name), -1, &statement, nullptr);
	assert(status == SQLITE_OK);
	sqlite3_step(statement);
	int result = sqlite3_column_int(statement, 0);
	sqlite3_finalize(statement);
	return result;
}

ClientsBD::ClientsBD(){
	openBd("clients.bd");
	dropTable("clients");
	createClients();
}

ClientsBD::~ClientsBD(){
	closeBd();
}
