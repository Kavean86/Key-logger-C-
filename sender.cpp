#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fstream>
using namespace std;
//extern const char* home;
void sender(const char* home){
cout<<"hello send\n";
string file_path = string(home) + "/.ohterminal";
while(true){
int socket_recv=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
if(socket_recv>=0){
sockaddr_in addr;
addr.sin_family=AF_INET;
addr.sin_port=htons(8080);
addr.sin_addr.s_addr = INADDR_ANY;
if(bind(socket_recv,(sockaddr*)&addr,sizeof(addr))>=0){
if (listen(socket_recv, 1)>=0){
sockaddr_in clientaddr{};
socklen_t client_size =sizeof(clientaddr);
int client = accept(socket_recv, (sockaddr*)&clientaddr, &client_size);
if (client >= 0){
ifstream file(file_path, ios::binary);
char buffer[1024];
streamsize n;
while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
n = file.gcount();
send(client, buffer, n, 0);
}
close(client);
close(socket_recv);
continue;
}else{
close(socket_recv);
//perror("accept");
continue;
}
}else{
close(socket_recv);
//perror("listen");
continue;
}
}else{
close(socket_recv);
//perror("bind");
continue;
}
}else{
close(socket_recv);
//perror("socket");
continue;
}
}



}
