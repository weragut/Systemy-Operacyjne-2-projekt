#include <iostream>
#include <thread>
#include <vector>
#include <winsock2.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib") // Link with WinSock library

const int PORT = 12345;
const int BUFFER_SIZE = 1024;

// Handle client connection in separate thread
void handleClient(SOCKET clientSocket, int clientId) {
    char buffer[BUFFER_SIZE];

    while (true) {
        // Clear the buffer
        memset(buffer, 0, BUFFER_SIZE);

        // Receive message from client
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived <= 0) {
            cout << "Client " << clientId << " disconnected." << endl;
            break;
        }

        // Print received message
        cout << "[Client " << clientId << "]: " << buffer << endl;
    }

    // Close socket
    closesocket(clientSocket);
}

int main() {
    // Initialize WinSock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed." << endl;
        return 1;
    }

    // Create server socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed." << endl;
        WSACleanup();
        return 1;
    }

    // Server address setup
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind socket
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Binding failed." << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Start listening
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Listen failed." << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Server listening on port " << PORT << "..." << endl;

    vector<thread> clientThreads;
    int clientId = 0;

    // Main server loop
    while (true) {
        // Accept new client
        sockaddr_in clientAddr{};
        int clientSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "Failed to accept connection." << endl;
            continue;
        }

        cout << "Client " << clientId << " connected." << endl;

        // Start new thread to handle this client
        clientThreads.emplace_back(handleClient, clientSocket, clientId);
        clientId++;
    }

    // Clean up (won't be reached in this infinite loop)
    for (auto& t : clientThreads) {
        if (t.joinable()) t.join();
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
