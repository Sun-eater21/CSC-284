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

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        std::cerr << "Failed to create socket\n";
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    sockaddr_in serverHint{};
    serverHint.sin_family = AF_INET;
    serverHint.sin_port = htons(54000);

    int ptonResult = inet_pton(AF_INET, "127.0.0.1", &serverHint.sin_addr);
    if (ptonResult <= 0)
    {
        std::cerr << "Invalid address or address conversion failed\n";
        closesocket(sock);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    int conn = connect(sock, (sockaddr *)&serverHint, sizeof(serverHint));
    if (conn < 0)
    {
        std::cerr << "Cannot connect to server\n";
        closesocket(sock);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    std::string input;
    char buffer[4096];

    while (true)
    {
        std::cout << "> ";
        if (!std::getline(std::cin, input))
        {
            std::cerr << "Input error\n";
            break;
        }

        if (input == "quit")
            break;

        int bytesSent = send(sock, input.c_str(), static_cast<int>(input.size()), 0);
        if (bytesSent < 0)
        {
            std::cerr << "Failed to send data to server\n";
            break;
        }

        std::memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);

        if (bytesReceived > 0)
        {
            std::cout << "Server: " << std::string(buffer, bytesReceived) << '\n';
        }
        else if (bytesReceived == 0)
        {
            std::cout << "Server disconnected\n";
            break;
        }
        else
        {
            std::cerr << "Receive failed\n";
            break;
        }
    }

    closesocket(sock);

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}