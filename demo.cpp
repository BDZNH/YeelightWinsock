#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") 

int main(int argc, char* argv[])
{
	WORD socketVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(socketVersion, &wsaData) != 0)
	{
		return 0;
	}
	SOCKET sclient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(1982);
	sin.sin_addr.S_un.S_addr = inet_addr("239.255.255.250");
	int len = sizeof(sin);

	const char * sendData = "M-SEARCH * HTTP/1.1\r\n\
				            HOST: 239.255.255.250:1982\r\n\
				            MAN: \"ssdp:discover\"\r\n\
				            ST: wifi_bulb";
	sendto(sclient, sendData, strlen(sendData), 0, (sockaddr *)&sin, len);

	char recvData[1024];
	int ret = recvfrom(sclient, recvData, 1024, 0, (sockaddr *)&sin, &len);
	if (ret > 0)
	{
		recvData[ret] = 0x00;
		printf(recvData);
	}

	closesocket(sclient);
	WSACleanup();
	return 0;
}