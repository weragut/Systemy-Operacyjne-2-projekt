#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <algorithm>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // required Winsock
using namespace std;

// client socket and id
struct ClientInfo {
    SOCKET socket;
    int id;
};

// list of active clients
vector<ClientInfo> active_clients;
// mutex to protect access to clients list
mutex clients_mutex;
// mutex to protect console output
mutex output_mutex;

// color based on client id
string getClientColorCode(int clientId) {
    const string codes[] = {
        "\033[91m", // red
        "\033[94m", // blue
        "\033[95m", // purple
        "\033[96m", // cyan
        "\033[92m", // green
        "\033[93m", // yellow
    };
    return codes[clientId % 6];
}

// sending message to all clients except the sender
void broadcastMessage(const string& message, SOCKET sender, int senderId) {
    // colored message
    string coloredMessage = getClientColorCode(senderId) + message + "\033[0m";

    // locking the mutex
    lock_guard<mutex> lock(clients_mutex);
    for (const auto& client : active_clients) {
        if (client.socket != sender) {
            send(client.socket, coloredMessage.c_str(), (int)coloredMessage.size(), 0); // send message to other clients
        }
    }
}

// client connection
void handleClient(SOCKET clientSocket, int clientId) {
    char buffer[1024]; // message data buffer

    while (true) {
        memset(buffer, 0, sizeof(buffer)); // clears the buffer
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0); // reciving data
        // disconnected client
        if (bytesReceived <= 0) {
            {
                lock_guard<mutex> lock(output_mutex);
                cout << getClientColorCode(clientId) << "Client #" << clientId << " disconnected." << "\033[0m" << endl;

            }

            {
                // remove client from active clients
                lock_guard<mutex> lock(clients_mutex);
                for (auto it = active_clients.begin(); it != active_clients.end(); ) {
                    // searching for the disconnected client
                    if (it->socket == clientSocket) {
                        it = active_clients.erase(it);  // returns an it to the next element
                        break;
                    }
                }
            }

            closesocket(clientSocket); // close the socket and end the thread
            return;
        }

        string msg = buffer; // data buffer to string

        // print message to server console
        {
            lock_guard<mutex> lock(output_mutex);
            cout << getClientColorCode(clientId) << "Client #" << clientId << " says: " << msg<< "\033[0m" << endl;


        }

        // send message to clients
        string fullMsg = "Client #" + to_string(clientId) + ": " + msg;
        broadcastMessage(fullMsg, clientSocket, clientId);
    }
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData); // Winsock initialization

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0); // create socket

    // TCP server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET; // IPv4
    serverAddr.sin_port = htons(54000); // port
    serverAddr.sin_addr.s_addr = INADDR_ANY; // accept connections

    // assigns a socket to the selected port and address
    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, SOMAXCONN);

    cout << "Server running. Listening for client connections. " << endl;

    int client_id = 0;

    while (true) {
        // accepts connections
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        {
            // add to active clients
            lock_guard<mutex> lock(clients_mutex);
            active_clients.push_back({ clientSocket, client_id });
        }

        {
            // print message to server console
            lock_guard<mutex> lock(output_mutex);
            cout << getClientColorCode(client_id) << "Client #" << client_id << " connected." << "\033[0m" << endl;

        }

        // create a new thread for the client
        thread t(handleClient, clientSocket, client_id++);
        t.detach(); // detach the thread to allow it to run independently
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
