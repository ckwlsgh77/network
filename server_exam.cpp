#define BUFSIZE 1024
#define TRANS_FILE 1

#include <iostream>
#include <fstream>
#include <string>
#include <WinSock2.h>

void ErrorHandling(std::string message, int code);
int dataReceive(SOCKET s, char* buf, int len, int flags);
int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	int szClntAddr;

	char message[] = "1234567891234567891234512345678979";

	if (argc != 2) {
		std::cout <<"Usage : "<<argv[0] << "<port>\n";
		exit(1);
	}
	
	int WSAStartup_state = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (WSAStartup_state != 0) {
		ErrorHandling("WSAStartup() error", WSAStartup_state);
	}

	hServSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hServSock == INVALID_SOCKET) {
		ErrorHandling("socket() error", 0);
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); //host to network long
	servAddr.sin_port = htons(atoi(argv[1])); // host to network short

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("bind() error\n", 0);
	}

	if (listen(hServSock, 5) == SOCKET_ERROR) {
		ErrorHandling("listen() error\n", 0);
	}
	szClntAddr = sizeof(clntAddr);


#ifdef TRANS_FILE
	char buf[BUFSIZE];

	while (1) {

		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);

		if (hClntSock == INVALID_SOCKET) {
			ErrorHandling("accept() error\n", 0);
		}

		std::cout << "클라이언트 접속 : IP = " << inet_ntoa(clntAddr.sin_addr) << " Port = " << ntohs(clntAddr.sin_port) << "\n";

		char filename[256];
		memset(filename, 0, 256);
		dataReceive(hClntSock, filename, 256, 0);

		std::cout << "받을 파일 이름 : " << filename << "\n";

		char bytesize[10];
		memset(bytesize, 0, 10);
		int totalbytes;
		dataReceive(hClntSock, bytesize,10, 0);

		totalbytes = atoi(bytesize);

		std::cout << "받을 파일 크기 : " << totalbytes << "바이트\n";

		FILE *fp = fopen(filename, "wb");

		int numtotal = 0;

		while (1) {
			int retval = dataReceive(hClntSock, buf, BUFSIZE, 0);

			if (retval == 0) {
				break;
			}
			else {
				fwrite(buf, 1, retval, fp);

				numtotal += retval;
			}

		}

		fclose(fp);

		if (numtotal == totalbytes) {
			std::cout << "파일을 성공적으로 수신했습니다.\n";

		}
		else {
			std::cout << "파일 수신에 실패했습니다.\n";

		}
	}
#endif

#ifndef TRANS_FILE // 서버 -> 클라
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);

	if (hClntSock == INVALID_SOCKET) {
		ErrorHandling("accept() error\n", 0);
	}

	send(hClntSock, message, sizeof(message), 0);
#endif

	

	closesocket(hClntSock);
	std::cout << "클라이언트 종료 : IP = " << inet_ntoa(clntAddr.sin_addr) << " Port = " << ntohs(clntAddr.sin_port) << "\n";

	closesocket(hServSock);

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

int dataReceive(SOCKET s, char* buf, int len, int flags) {

	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);

		if (received == 0)
			break;

		left -= received;
		ptr += received;

	}


	
	return (len - left);
}