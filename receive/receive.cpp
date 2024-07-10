#include <iostream>
#include <windows.h>
#include <../windows_Serial/windows_Serial.hpp>


int main() {
	windows_Serial receiveport;
	if (!receiveport.open(51,115200)) { // 设置串口
		return 1;
	}
	TargetData data={0,0,0,0};
	while (1) {
		receiveport.receiveData(data);
		// 打印接收到的数据
		std::cout << "Received data: " << std::endl;
		std::cout << "x: " << data.x << std::endl;
		std::cout << "y: " << data.y << std::endl;
		std::cout << "z: " << data.z << std::endl;
		std::cout << "rotation: " << data.rotation << std::endl;
	}
	return 0;
}
