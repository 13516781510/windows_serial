project(send)
include_directories(../windows_Serial)
add_executable(send send.cpp ../windows_Serial/windows_Serial.cpp ../windows_Serial/windows_Serial.cpp)
