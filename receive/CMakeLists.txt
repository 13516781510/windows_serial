project(receive)
include_directories(../windows_Serial)
add_executable(receive receive.cpp ../windows_Serial/windows_Serial.cpp ../windows_Serial/windows_Serial.cpp)
