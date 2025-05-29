#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

const int PORT = 12345;

int main() {
    // Initialize WinSock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed." << endl;
        return 1;
    }

    // Create socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed." << endl;
        WSACleanup();
        return 1;
    }

    // Set up server address
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost

    // Connect to server
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Connection to server failed." << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    cout << "Connected to server. Type messages below:" << endl;

    // Message sending loop
    string message;
    while (true) {
        getline(cin, message);
        if (message == "/exit") break;

        send(clientSocket, message.c_str(), message.length(), 0);
    }

    // Cleanup
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
