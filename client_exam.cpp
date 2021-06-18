#define TRANS_FILE 1
#define BUFSIZE 1024

#include <iostream>
#include <fstream>
#include <string>
#include <WinSock2.h>

void ErrorHandling(std::string message, int code);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30];
	int strLen = 0;
	int readLen = 0, idx = 0;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	int WSAStartup_state = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (WSAStartup_state != 0) {
		ErrorHandling("WSAStartup() error\n", WSAStartup_state);
	}

	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hSocket == INVALID_SOCKET) {
		ErrorHandling("socket() error\n", 0);
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("connect() error\n", 0);
	}

	std::cout << "서버 접속 : IP = " << inet_ntoa(servAddr.sin_addr) << " Port = " << ntohs(servAddr.sin_port) << "\n";


#ifdef TRANS_FILE
	char myFile[256] = "rock.jpg";
	FILE *fp = fopen(myFile, "rb");

	char filename[256];
	memset(filename, 0, 256);
	sprintf(filename, myFile);

	send(hSocket, filename, 256, 0);

	fseek(fp, 0, SEEK_END);
	int totalbytes = ftell(fp);
	char bytesize[10];
	memset(bytesize, 0, 10);

	sprintf(bytesize, "%d", totalbytes);

	send(hSocket, bytesize, 10, 0);
	std::cout << "전송 파일의 크기 :" << totalbytes << "\n";
	char buf[BUFSIZE];
	int numread;
	int numtotal = 0;


	rewind(fp);

	while (1) {
		numread = fread(buf, 1, BUFSIZE, fp);

		if (numread > 0) {
			send(hSocket, buf, numread, 0);
			numtotal += numread;
		}
		else if (numread == 0 && totalbytes == numtotal) {
			std::cout << "총 " << numtotal << "바이트의 파일 전송을 완료했습니다. \n";
			break;
		}
	}
	fclose(fp);
#endif


#ifndef TRANS_FILE

	while (readLen = recv(hSocket, &message[idx++], 1, 0)) {

		if (readLen == -1) {
			ErrorHandling("recv() error\n", 0);
			break;
		}

		strLen += readLen;
	}

	printf("Message from server: %s\n", message);
	printf("Function read call count : %d\n", strLen);

#endif
	closesocket(hSocket);
	std::cout << "서버 접속 종료 : IP = " << inet_ntoa(servAddr.sin_addr) << " Port = " << ntohs(servAddr.sin_port) << "\n";

	WSACleanup();

	return 0;
}

void ErrorHandling(std::string message, int code) {
	std::ofstream writeFile;

	writeFile.open("error_log.txt");

	writeFile.write(message.c_str(), message.size());

	writeFile.close();

	exit(1);
}