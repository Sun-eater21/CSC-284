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

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverHint{};
    serverHint.sin_family = AF_INET;
    serverHint.sin_port = htons(54000);
    inet_pton(AF_INET, "127.0.0.1", &serverHint.sin_addr);

    int conn = connect(sock, (sockaddr *)&serverHint, sizeof(serverHint));
    if (conn < 0)
    {
        std::cerr << "Cannot connect";
        return 1;
    }

    std::string input;
    char buffer[4096];

    while (true)
    {
        std::getline(std::cin, input);
        if (input == "quit")
            break;

        send(sock, input.c_str(), input.size(), 0);

        memset(buffer, 0, 4096);
        int bytesReceived = recv(sock, buffer, 4096, 0);
        if (bytesReceived > 0)
            std::cout << "Server: " << std::string(buffer, bytesReceived) << std::endl;
    }

    closesocket(sock);

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}