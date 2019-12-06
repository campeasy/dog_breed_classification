// Client side C/C++ program to demonstrate Socket programming
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

typedef unsigned char byte;

#define PORT 9999

#define BUFFER_SIZE 1024

int main(int argc, char const *argv[])
{
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

	FILE* file_ptr = fopen("test_images/dog2.jpg", "r");
	fseek(file_ptr, 0, SEEK_END);
	long file_size = ftell(file_ptr);
	rewind(file_ptr);

	std::cout<<file_size<<std::endl;

	send(sock, &file_size, sizeof(long), /* flags: */0);

	size_t bytes_sent = 0;
	byte buffer[1024];
	while(bytes_sent < file_size)
	{
		size_t bytes_read = fread(buffer, 1, BUFFER_SIZE, file_ptr);
		send(sock, buffer, bytes_read, /* flags: */0);
		bytes_sent += bytes_read;
	}
	std::cout<<"File sent"<<std::endl;
	char result[BUFFER_SIZE];
	recv(sock, result, BUFFER_SIZE, 0);
	std::cout<<std::string(result)<<std::endl;
	fclose(file_ptr);
	close(sock);
	return 0;
}
