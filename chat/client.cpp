#include <iostream>
#include <thread>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

// receive messages from server
void receiveMessages(SOCKET sock) {
    char buffer[1024]; // data buffer
    while (true) {
        memset(buffer, 0, sizeof(buffer)); // clear buffer before receiving
        int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
        // print message
        if (bytesReceived > 0) {
            cout << endl << buffer << endl << "> ";
        }
    }
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData); // Winsock initialization

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); // create TCP socket
    sockaddr_in server; // server address structure (type, port, IP nr)
    server.sin_family = AF_INET; // IPv4
    server.sin_port = htons(54000); // port
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); // convert IP address to binary

    // connect to server
    if (connect(sock, (sockaddr*)&server, sizeof(server)) != 0) {
        cerr << "Connection to server failed." << endl;
        return 1;
    }

    cout << "Connected to chat server!" << endl;

    // new thread for receiving messages
    thread t(receiveMessages, sock);
    t.detach();

    string message;
    while (true) {
        cout << "> ";
        getline(cin, message);
        if (message == "/exit") break;
        // send message to server
        send(sock, message.c_str(), (int)message.length(), 0);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
