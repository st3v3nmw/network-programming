#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024


void retrieve_state_info(void)
{
    char ch;
   FILE *fp;

   fp = fopen("state_info.txt", "r"); // read state info

  /* if (fp == NULL)
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }*/

   printf("The state information is as follows:- ");

   while((ch = fgetc(fp)) != EOF)
      printf("%c", ch);

   fclose(fp);

}

void write_file(int sockfd){
	int n;
	FILE *fp;
	char *filename = "recv.txt";
	char buffer[SIZE];

	fp = fopen(filename, "w");
	printf("Client Sent \n");
	while (1) {
		//receive the data from the client and store in te buffer
		n = recv(sockfd, buffer, SIZE, 0);
		if (n <= 0){
			break;
			return;
		}
		printf("%s",buffer);

		//save the data to the file specified
		fprintf(fp, "%s", buffer);
		bzero(buffer, SIZE);//empty the buffer 
	}
	return;
}


void main(){
	char *ip = "127.0.0.1"; // ip adsress
	int port = 8080; //port number
	int conn; //connection variable

	int sockfd, new_sock;
	struct sockaddr_in server_addr, new_addr;
	socklen_t addr_size;
	char buffer[SIZE];

	//create the socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		perror("Error in socket");
		exit(1);
	}
	printf("Server socket created successfully.\n");

	//specify the port and address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = port;
	server_addr.sin_addr.s_addr = inet_addr(ip);

	//bind the socket to the server address
	conn = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(conn < 0) {
		perror("Error in bind");
		exit(1);
	}
	printf("Binding successfull.\n");

	//retrieve state information 
	retrieve_state_info();

	// listen for connection request from the clients
	if(listen(sockfd, 10) == 0){
		printf("Listening....\n");
	}else{
		perror("Error in listening");
		exit(1);
	}

	addr_size = sizeof(new_addr);// address size 
	while (1) {
		//accept the connection from the cleint
		new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
		//call the write function
		write_file(new_sock);
	}
	printf("Data written in the file successfully.\n");

	
}