#include <assert.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <map>
#include <string>
#include <utility>

#include "clients_bd.h"

std::pair<std::string, std::string> parseClientMess(std::string mess, int size);
void sendToAll(std::string, int);
void* th1(void* argv);

class ClientsBD* clients = new ClientsBD();

int main(int argc, char** argv) {
	std::cout << "my not full server... v3.0 not to much secret " << std::endl;

	pthread_t id_thread;

	int id_socket = socket(AF_INET, SOCK_STREAM, 0);
	assert(id_socket > 0);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[1]));
	addr.sin_addr.s_addr = INADDR_ANY;
 
	int res = bind(id_socket, (sockaddr*)&addr, sizeof(addr));
	assert(res == 0);

	res = listen(id_socket, 32);
	
	while(1) {
		int id_client = accept(id_socket, nullptr, nullptr);
		pthread_create(&id_thread, nullptr, th1, &id_client);
		assert(id_client > 0);
	}

	close(id_socket);
	delete clients;

	return 0;
}

std::pair<std::string, std::string> parseClientMess(std::string mess, int size){
	std::string client_name = "";
   	std::string ret_mes = "";
	int count_sk = 0;
	if(mess[0] != '@'){
		return {"", mess};
	}
	for (int i = 0; i < size and mess[i] != ';'; i++){
		if (mess[i] == '@' or mess[i] == ':'){
			count_sk += 1;
			continue;
		}
		if(count_sk == 1){
			client_name += mess[i];
			continue;
		}
		if(count_sk == 2){
			ret_mes += mess[i];	
		}
	}
	return {client_name, ret_mes};
}

void* th1(void* arg) {
	char buf[64];
	int id_client = *static_cast<int*>(arg);
	std::string name_client;
	while(1){
		int count_bytes = recv(id_client, buf, 64, 0);
		if (count_bytes > 0) {
			std::pair<std::string, std::string> name_mes = parseClientMess(buf, count_bytes);
			if(name_mes.first == "" and name_mes.second != ""){
				memset(buf, 0x00, 64);
				std::cout <<"too all " <<  name_mes.second << std::endl;;
				name_mes.second = "from " + name_client + " : " + name_mes.second;
				sendToAll(name_mes.second, id_client);
				continue;
			}
			if(name_mes.first == ""){
				continue;
			}
			if(name_mes.second == "" and name_client == ""){
				::clients->insertClient(id_client, name_mes.first.c_str(), time(nullptr));
				name_client = name_mes.first;
				std::cout << name_mes.first << " connected" << std::endl;
				continue;
			}
			
			int id_recipient = ::clients->getIdClient(name_mes.first.c_str());
			std::cout << id_recipient << " " <<  name_mes.first << " : " << name_mes.second << std::endl;
			if (id_recipient <= 0){
				send(id_client, "no such client!!!!1", 20, 0);
			}

			memset(buf, 0x00, 64);
			name_mes.second = "from " + name_client + " : " + name_mes.second;
			send(id_recipient, name_mes.second.c_str(), sizeof(name_mes.second), 0);
		}
		if(count_bytes <= 0){
			std::cout << "error client " << id_client << " disconnected" << std::endl;
			::clients->deleteClient(id_client);
			break;
		}
	}
	close(id_client);
	return 0;
}

void sendToAll(std::string mess, int id_client){
	std::vector<int> id_recipients = ::clients->getAllClientsId();
	for (int i = 0; i < id_recipients.size(); i++){
		if(id_recipients[i] == id_client){
			continue;
		}
		send(id_recipients[i], mess.c_str(), sizeof(mess), 0);
	}
}
