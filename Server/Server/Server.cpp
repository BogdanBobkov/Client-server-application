#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable: 4996)
#define MAX_ELEMENTS 10
#define PORT 1234
#define IP "0.0.0.0"
using namespace std;

double sumOfElements(vector<double> &vec);

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	WSAData wsadata;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsadata) != 0) {
		cout << "ERROR" << endl;
		system("pause");
		return 1;
	}
	SOCKADDR_IN addr;
	int size_of_addr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr(IP);
	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET;
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, size_of_addr);
	listen(sListen, SOMAXCONN);
	while (true) {
		SOCKET newConnection;
		newConnection = accept(sListen, (SOCKADDR*)&addr, &size_of_addr);
		if (newConnection == 0) cout << "ERROR" << endl;
		else cout << "A client has been connected" << endl;
		vector<double> vec(MAX_ELEMENTS);
		recv(newConnection, (char *)vec.data(), MAX_ELEMENTS * sizeof(double), NULL);
		double sum = sumOfElements(vec);
		vec.clear();
		vec.shrink_to_fit();
		send(newConnection, (char *)&sum, sizeof(double), NULL);
	}
	system("pause");
	return 0;
}

double sumOfElements(vector<double> &vec) {
	double sum = 0;
	for (double el : vec)
		sum += el;
	return sum;
}