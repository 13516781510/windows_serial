//
// Created by 29451 on 2024/7/10.
//

#ifndef RS485TEST_RECEIVE_WINDOWS_SERIAL_HPP
#define RS485TEST_RECEIVE_WINDOWS_SERIAL_HPP
#include <Windows.h>
struct TargetData {
	float x;
	float y;
	float z;
	float rotation;
};

class windows_Serial {
public:
	windows_Serial();
	~windows_Serial();
	bool open(int portNumber, int rate = 115200);
	void close();
	HANDLE hSerial;
	bool sendData(const TargetData &data);
	void receiveData(TargetData &data);
};

#endif //RS485TEST_RECEIVE_WINDOWS_SERIAL_HPP
