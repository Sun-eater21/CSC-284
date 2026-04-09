#include <iostream>
#include <string>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define closesocket close
#endif

int main()
{
#ifdef _WIN32
    WSADATA data;
    if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
    {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }
#endif

    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening < 0)
    {
        std::cerr << "Cannot create socket\n";
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.s_addr = INADDR_ANY;

    if (bind(listening, (sockaddr *)&hint, sizeof(hint)) < 0)
    {
        std::cerr << "Bind failed\n";
        closesocket(listening);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    if (listen(listening, SOMAXCONN) < 0)
    {
        std::cerr << "Listen failed\n";
        closesocket(listening);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    sockaddr_in client{};
    socklen_t clientSize = sizeof(client);

    int clientSocket = accept(listening, (sockaddr *)&client, &clientSize);
    if (clientSocket < 0)
    {
        std::cerr << "Accept failed\n";
        closesocket(listening);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    closesocket(listening);

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    std::memset(host, 0, sizeof(host));
    std::memset(service, 0, sizeof(service));

    inet_ntop(AF_INET, &client.sin_addr, host, sizeof(host));
    std::cout << "Client connected from " << host
              << ":" << ntohs(client.sin_port) << '\n';

    char buffer[4096];
    while (true)
    {
        std::memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived > 0)
        {
            std::string msg(buffer, bytesReceived);
            std::cout << "Client: " << msg << '\n';

            int bytesSent = send(clientSocket, buffer, bytesReceived, 0);
            if (bytesSent < 0)
            {
                std::cerr << "Send failed\n";
                break;
            }
        }
        else if (bytesReceived == 0)
        {
            std::cout << "Client disconnected\n";
            break;
        }
        else
        {
            std::cerr << "Receive failed\n";
            break;
        }
    }

    closesocket(clientSocket);

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}