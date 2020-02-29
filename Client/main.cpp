#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

void ShowErrorMessage(string message)
{
    cout << "[오류 발생]: " << message << "\n";
    system("pause");
    exit(1);
}

int main()
{
    WSADATA wsaData;
    SOCKET clientSocket;
    SOCKADDR_IN serverAddress;

    int serverPort = 9876;
    char received[256];
    string sent;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //Winsock 을 초기화 합니다.
        ShowErrorMessage("WSAStartup()");

    clientSocket = socket(PF_INET, SOCK_STREAM, 0); // TCP소켓을 생성합니다.

    if (clientSocket == INVALID_SOCKET)
        ShowErrorMessage("socket()");

    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); //문자열 IP를 네트워크 바이트 형식으로
    serverAddress.sin_port = htons(serverPort);             // 2바이트 정수 네트워크 바이트 형식으로
    if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
        ShowErrorMessage("connect()");
    cout << "[현재 상태] connect()\n";

    // 반복적으로 서버로 메시지를 전송하고 에코 메시지를 전달 받습니다.
    while (1)
    {
        cout << "[메세지 전송]: ";
        getline(cin, sent);
        if (sent == "")
            continue;
        send(clientSocket, sent.c_str(), sent.length(), 0);
        int length = recv(clientSocket, received, sizeof(received), 0);
        received[length] = '\0';
        if (strcmp(received, "[exit]") == 0)
        {
            cout << "[서버 종료]\n";
            break;
        }
        cout << "[서버 메세지]: " << received << "\n";
    }

    closesocket(clientSocket);
    WSACleanup();
    system("pause");
    return 0;
}