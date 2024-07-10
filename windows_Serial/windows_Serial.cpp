//
// Created by 29451 on 2024/7/10.
//
#include <iostream>
#include "windows_Serial.hpp"
#include <string>

windows_Serial::~windows_Serial(){
	CloseHandle(this->hSerial); // �رմ���
}
bool windows_Serial::open(int portNumber, int rate ) {

	std::string portName = "\\\\.\\COM" + std::to_string(portNumber);
	this->hSerial = CreateFileA(portName.c_str(),               // ������������ʵ������޸�
	                            GENERIC_READ |GENERIC_WRITE,        // ����ģʽ��������дģʽ
	                      0,                    // ����ģʽ��0 ��ʾ������
	                      nullptr,                 // ��ȫ���ԣ�NULL ��ʾĬ��
	                      OPEN_EXISTING,        // ���Ѵ��ڵĴ���
	                      FILE_ATTRIBUTE_NORMAL,// �ļ����ԣ���������
	                      nullptr);                // ģ���ļ������NULL ��ʾ��

	if (this->hSerial == INVALID_HANDLE_VALUE) {     // �жϴ����Ƿ�ɹ���
		std::cerr << "Error opening serial port" << std::endl;
		return false;
	}

	DCB dcbSerialParams = {0};                 // ��ʼ�� DCB �ṹ��
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	if (!GetCommState(this->hSerial, &dcbSerialParams)) { // ��ȡ��ǰ����״̬
		std::cerr << "Error getting serial port state" << std::endl;
		CloseHandle(this->hSerial);
		return false;
	}

	// ���ô��ڲ���
	dcbSerialParams.BaudRate = CBR_9600;       // ���ò�����Ϊ 9600
	dcbSerialParams.ByteSize = 8;              // ��������λΪ 8
	dcbSerialParams.StopBits = ONESTOPBIT;     // ����ֹͣλΪ 1
	dcbSerialParams.Parity = NOPARITY;         // ��������żУ��

	if (!SetCommState(this->hSerial, &dcbSerialParams)) { // Ӧ���µĴ��ڲ���
		std::cerr << "Error setting serial port state" << std::endl;
		CloseHandle(this->hSerial);
		return false;
	}

	// ����д�����ĳ�ʱ
	COMMTIMEOUTS timeouts = {0};
	timeouts.WriteTotalTimeoutConstant = 1000; // 1�볬ʱ
	if (!SetCommTimeouts(this->hSerial, &timeouts)) {
		std::cerr << "Error setting timeouts" << std::endl;
		CloseHandle(this->hSerial);
		return false;
	}
	// ���ó�ʱ����
	timeouts.ReadIntervalTimeout = 50;         // �������ʱ
	timeouts.ReadTotalTimeoutConstant = 50;    // ���ܳ�ʱ����
	timeouts.ReadTotalTimeoutMultiplier = 10;  // ���ܳ�ʱ����

	if (!SetCommTimeouts(hSerial, &timeouts)) {
		std::cerr << "Error setting serial port timeouts" << std::endl;
		CloseHandle(hSerial);
		return false;
	}
	return true;                               // ���سɹ���־
}

bool windows_Serial::sendData(const TargetData &data) {
	DWORD bytes_written;
	if (!WriteFile(this->hSerial, &data, sizeof(data), &bytes_written, nullptr)) {
		std::cerr << "Error writing to serial port: " << GetLastError() << std::endl;
		return false;
	} else {
		std::cout << "Data sent successfully" << std::endl;
		return true;
	}
}

windows_Serial::windows_Serial() {
	this->hSerial = INVALID_HANDLE_VALUE;
}
void windows_Serial::receiveData(TargetData &data) {
	DWORD bytesRead;
	while (true) {
		if (ReadFile(this->hSerial, &data, sizeof(data), &bytesRead, nullptr) && bytesRead == sizeof(data)) {
			break; // �ɹ���ȡ������������
		} else {
			Sleep(10); // �ȴ�һ��ʱ���ٳ��Զ�ȡ
		}
	}
}