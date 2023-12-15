#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <iostream>
#include <assert.h>
#include <unistd.h>

void* tr_send(void*);
void* tr_recv(void*);

int main() {
	std::cout << "simple client ver 1.0...." << std::endl;
	
	pthread_t thread_send;
	pthread_t thread_recv;	

	int id_socket = socket(AF_INET, SOCK_STREAM, 0);
	assert(id_socket > 0);

	sockaddr_in addr;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	std::cout << "127.0.0.1  " << inet_addr("127.0.0.1") << std::endl;;
	addr.sin_port = htons(33001);
	addr.sin_family = AF_INET;

	int res = connect(id_socket, (sockaddr *)&addr, sizeof(addr));
	assert(res == 0);

	send(id_socket, "привет  你好  test", 64, 0);
	pthread_create(&thread_send, nullptr, tr_send, &id_socket);
	pthread_create(&thread_recv, nullptr, tr_recv, &id_socket);
	
	while(1){}
	
	close(id_socket);
	return 0;
}

void* tr_send(void* argv){
	int id_socket = *static_cast<int*>(argv);
	while(1){
		std::string mes;
		getline(std::cin, mes);
		send(id_socket, mes.c_str(), mes.size(), 0);
	}
}

void* tr_recv(void* argv){
	int id_socket = *static_cast<int*>(argv);
	char buf[64];
	while(1){
		int count_bytes = recv(id_socket, buf, 64, 0);
		if(count_bytes > 0){
			std::cout << buf << std::endl;
		}
	}
}
