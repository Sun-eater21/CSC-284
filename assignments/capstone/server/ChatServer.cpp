#include <iostream>
#include <string>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define closesocket close
#endif

int main()
{
#ifdef _WIN32
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);
#endif

    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening < 0)
    {
        std::cerr << "Cannot create socket";
        return 1;
    }

    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.s_addr = INADDR_ANY;

    bind(listening, (sockaddr *)&hint, sizeof(hint));
    listen(listening, SOMAXCONN);

    sockaddr_in client{};
    socklen_t clientSize = sizeof(client);

    int clientSocket = accept(listening, (sockaddr *)&client, &clientSize);
    closesocket(listening);

    char buffer[4096];
    while (true)
    {
        memset(buffer, 0, 4096);
        int bytesReceived = recv(clientSocket, buffer, 4096, 0);
        if (bytesReceived <= 0)
            break;

        std::string msg(buffer, bytesReceived);
        std::cout << "Client: " << msg << std::endl;

        send(clientSocket, buffer, bytesReceived, 0);
    }

    closesocket(clientSocket);

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}