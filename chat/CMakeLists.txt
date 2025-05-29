cmake_minimum_required(VERSION 3.15)
project(SO)
set(CMAKE_CXX_STANDARD 20)
# chat/CMakeLists.txt

add_executable(chat_server server.cpp)
add_executable(chat_client client.cpp)

if (WIN32)
    target_link_libraries(chat_server ws2_32)
    target_link_libraries(chat_client ws2_32)
endif()

