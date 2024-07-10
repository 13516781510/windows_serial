//
// Created by 29451 on 2024/7/10.
//
#include <iostream>
#include "windows_Serial.hpp"
#include <string>

windows_Serial::~windows_Serial(){
	CloseHandle(this->hSerial); // 关闭串口
}
bool windows_Serial::open(int portNumber, int rate ) {

	std::string portName = "\\\\.\\COM" + std::to_string(portNumber);
	this->hSerial = CreateFileA(portName.c_str(),               // 串口名，根据实际情况修改
	                            GENERIC_READ |GENERIC_WRITE,        // 访问模式，这里是写模式
	                      0,                    // 共享模式，0 表示不共享
	                      nullptr,                 // 安全属性，NULL 表示默认
	                      OPEN_EXISTING,        // 打开已存在的串口
	                      FILE_ATTRIBUTE_NORMAL,// 文件属性，正常属性
	                      nullptr);                // 模板文件句柄，NULL 表示无

	if (this->hSerial == INVALID_HANDLE_VALUE) {     // 判断串口是否成功打开
		std::cerr << "Error opening serial port" << std::endl;
		return false;
	}

	DCB dcbSerialParams = {0};                 // 初始化 DCB 结构体
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	if (!GetCommState(this->hSerial, &dcbSerialParams)) { // 获取当前串口状态
		std::cerr << "Error getting serial port state" << std::endl;
		CloseHandle(this->hSerial);
		return false;
	}

	// 设置串口参数
	dcbSerialParams.BaudRate = CBR_9600;       // 设置波特率为 9600
	dcbSerialParams.ByteSize = 8;              // 设置数据位为 8
	dcbSerialParams.StopBits = ONESTOPBIT;     // 设置停止位为 1
	dcbSerialParams.Parity = NOPARITY;         // 设置无奇偶校验

	if (!SetCommState(this->hSerial, &dcbSerialParams)) { // 应用新的串口参数
		std::cerr << "Error setting serial port state" << std::endl;
		CloseHandle(this->hSerial);
		return false;
	}

	// 设置写操作的超时
	COMMTIMEOUTS timeouts = {0};
	timeouts.WriteTotalTimeoutConstant = 1000; // 1秒超时
	if (!SetCommTimeouts(this->hSerial, &timeouts)) {
		std::cerr << "Error setting timeouts" << std::endl;
		CloseHandle(this->hSerial);
		return false;
	}
	// 设置超时参数
	timeouts.ReadIntervalTimeout = 50;         // 读间隔超时
	timeouts.ReadTotalTimeoutConstant = 50;    // 读总超时常量
	timeouts.ReadTotalTimeoutMultiplier = 10;  // 读总超时乘数

	if (!SetCommTimeouts(hSerial, &timeouts)) {
		std::cerr << "Error setting serial port timeouts" << std::endl;
		CloseHandle(hSerial);
		return false;
	}
	return true;                               // 返回成功标志
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
			break; // 成功读取到完整的数据
		} else {
			Sleep(10); // 等待一段时间再尝试读取
		}
	}
}