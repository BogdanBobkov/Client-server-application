#include "stdafx.h"
#include <iostream>
#include <vector>
#include <winsock2.h>
#include <iomanip>
#include <fstream>
#include <string>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable: 4996)
#define PATH "vectors.txt"
#define PRECISION 3
#define PORT 1234
#define IP "127.0.0.1"
using namespace std;

void listOfVectors(ifstream &stream, int &sumOfVectors);	// Вывод всех векторов на экран
void takeElements(ifstream &stream, int &numOfVector, vector<double> &vec);	// Запись значений в vector

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int sumOfVectors = 0;
	double totalSum = 0;
	vector<double> vec;
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
	SOCKET connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(connection, (SOCKADDR*)&addr, size_of_addr) != 0) {
		cout << "Error: failed connect to server" << endl;
		system("pause");
		return 1;
	}
	ifstream stream;
	stream.open(PATH);
	if (!stream.is_open()) {
		cout << "Не удалось открыть файл!" << endl;
		system("pause");
		return 2;
	}
	listOfVectors(stream, sumOfVectors);
	cout << "Введите номер вектора для подсчета суммы его элементов: ";
	int numOfVector;
	do {
		cin >> numOfVector;
		if (numOfVector > sumOfVectors || numOfVector < 1) cout << "Некорректно. Введите заново: ";
	} while (numOfVector > sumOfVectors || numOfVector < 1);
	takeElements(stream, numOfVector, vec);
	send(connection, (char*)vec.data(), vec.size() * sizeof(double), NULL);
	recv(connection, (char*)&totalSum, sizeof(double), NULL);
	cout << "Сумма элементов вектора: " << fixed << setprecision(PRECISION) << totalSum << endl;
	stream.close();
	system("pause");
	return 0;
}

void listOfVectors(ifstream &stream, int &sumOfVectors) {
	string str;
	cout << "Векторы в файле: " << endl;
	while (!stream.eof()) {
		++sumOfVectors;
		str = "";
		getline(stream, str);
		cout << sumOfVectors << ". " << str << endl;
	}
	stream.clear();
	stream.seekg(NULL);
}

void takeElements(ifstream &stream, int &numOfVector, vector<double> &vec) {
	int numOfElement = 0;
	--numOfVector;
	while (numOfVector > 0) {
		string str;
		getline(stream, str);
		str.clear();
		str.shrink_to_fit();
		--numOfVector;
	}
	char symbol = stream.get();
	string number;
	while (symbol != ')') {
		if (symbol >= '0' && symbol <= '9' || symbol == '.') {
			number += symbol;
		}
		else if (symbol != '(') {
			vec.push_back(atof(number.c_str()));
			number.clear();
			number.shrink_to_fit();
		}
		stream.get(symbol);
	}
	vec.push_back(atof(number.c_str()));
	number.clear();
	number.shrink_to_fit();
}

