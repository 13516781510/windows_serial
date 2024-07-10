#include <iostream>
#include <Windows.h>
#include <cstdlib>  // for rand, srand
#include <ctime>    // for time
#include <../windows_Serial/windows_Serial.hpp>

TargetData data = {1.0f, 2.0f, 3.0f, 45.0f}; // ʾ������
float generateRandomFloat(float min, float max) {
	return min + static_cast<float>(rand())
	/ (static_cast<float>(RAND_MAX / (max - min))
			);
}

void detect_and_location_task() {
	data.x = generateRandomFloat(0.0f, 10.0f);// ������� x
	data.y = generateRandomFloat(0.0f, 10.0f);// ������� x
	data.z = generateRandomFloat(0.0f, 10.0f); // ������� x
	data.rotation = generateRandomFloat(0.0f, 360.0f);// ������� rotation
	Sleep(10);
}

int main() {
	srand(static_cast<unsigned int>(time(0)));
	windows_Serial sendport;
	if (!sendport.open(50, 115200)) { // ���ô���
		return 1;
	}
	while (true) {
		detect_and_location_task();
		if (sendport.sendData(data)) {
			// ���͵�����
			std::cout << "x: " << data.x << std::endl;
			std::cout << "y: " << data.y << std::endl;
			std::cout << "z: " << data.z << std::endl;
			std::cout << "rotation: " << data.rotation << std::endl;
		}// ��������
	}
	system("pause");
	return 0;
}
