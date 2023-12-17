#ifndef _CLIENTS_BD_H
#define _CLIENTS_BD_H_

#include <vector>

#include "base_sqlite.h"

class ClientsBD : private BD {
	private:
		void createClients();
		static int callback(void* outputStruct, int countRec, char** argv, char** colName);
	public:
		ClientsBD();
		~ClientsBD();
		void insertClient(int id_client, const char* clientName, int time);
		void deleteClient(int id_client);
		int getIdClient(const char* name);
		std::vector<int> getAllClientsId();
};

#endif //_CLIENTS_BD_H_ 
