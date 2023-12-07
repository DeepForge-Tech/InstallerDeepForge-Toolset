// Importing libraries
#include <iostream>
#include <winsock2.h>
#include <fstream>
using namespace std;

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)

#define IP_ADDRESS "64.226.99.105" // or "localhost" - ip address of UDP server
#define BUFFER_MAX_LENGHT 16384           // max length of answer
#define PORT 743

using namespace std;

namespace POSIX
{
    class Client
    {
        // Init variables
        WSADATA ws;
        /* `sockaddr_in server;` is declaring a variable named `server` of type `sockaddr_in`. `sockaddr_in` is a structure that represents an Internet Protocol (IP) socket address. In this case, it is used to store the server information for the client. The server's IP address, port number, and other relevant information will be set in this structure before sending or receiving data from the server. */
        sockaddr_in server;
        int client_socket;
        bool isOpen = false;

    public:
        void CreateSocket()
        {
            try
            {
                /* The code `WSAStartup(MAKEWORD(2, 2), &ws)` initializes the Winsock library. It is necessary to call this function before using any other Winsock functions. */
                WSAStartup(MAKEWORD(2, 2), &ws);
                if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) // <<< UDP socket
                {
                    printf("socket() failed with error code: %d", WSAGetLastError());
                }
                isOpen = true;
            }
            catch (exception &error)
            {
                cerr << error.what() << endl;
            }
        }

        /* The `SetInformation()` function is responsible for setting up the server information for the client. */
        void SetInformation()
        {
            /* `memset((char *)&server, 0, sizeof(server));` is a function that sets the memory of the `server` object to zero. */
            memset((char *)&server, 0, sizeof(server));
            server.sin_family = AF_INET;
            server.sin_port = htons(PORT);
            server.sin_addr.S_un.S_addr = inet_addr(IP_ADDRESS);
        }
        /* The `SEND_JSON_ARRAY` function is sending a JSON array over a UDP socket to a specified server. */
        void SEND_JSON_ARRAY(string data)
        {
            /* The line `sendto(client_socket, data.c_str(), strlen(data.c_str()), 0, (sockaddr *)&server, sizeof(sockaddr_in));` is sending data over a UDP socket to a specified server. */
            sendto(client_socket, data.c_str(), strlen(data.c_str()), 0, (sockaddr *)&server, sizeof(sockaddr_in));
        }
        void SEND_JSON_FILE(string path)
        {
            /* This code is responsible for reading the contents of a file specified by the `path` variable and storing it in the `data` array. */
            FILE *fp;
            char data[BUFFER_MAX_LENGHT];
            if ((fp = fopen(path.c_str(), "rb")) == NULL)
            {
                printf("Cannot open file.\n");
                exit(1);
            }
            fread(data, sizeof(char), BUFFER_MAX_LENGHT, fp);
            fclose(fp);
            /* The line `sendto(client_socket, data, strlen(data), 0, (sockaddr *)&server, sizeof(sockaddr_in));` is sending data over a UDP socket to a specified server. */
            sendto(client_socket, data, strlen(data), 0, (sockaddr *)&server, sizeof(sockaddr_in));
        }

        void SEND_TEXT_MESSAGE(string message)
        {
            /* The line `sendto(client_socket, new_message, strlen(new_message), 0, (sockaddr*)&server, sizeof(sockaddr_in));` is sending a text message over a UDP socket to a specified server. */
            sendto(client_socket, message.c_str(), strlen(message.c_str()), 0, (sockaddr*)&server, sizeof(sockaddr_in));
        }

        /* The `GetInformation()` function is responsible for receiving information from the server over the UDP socket. */
        void GetInformation()
        {
            char answer[BUFFER_MAX_LENGHT];
            int slen = sizeof(sockaddr_in);
            int answer_length;
            /* The line `answer_length = recvfrom(client_socket, answer, BUFFER_MAX_LENGHT, 0, (sockaddr *)&server, &slen);` is receiving information from the server over the UDP socket. */
            answer_length = recvfrom(client_socket, answer, BUFFER_MAX_LENGHT, 0, (sockaddr *)&server, &slen);
        }

        void CloseSocket()
        {
            if (isOpen == true)
            {
                closesocket(client_socket);
                /* `WSACleanup();` is a function that is used to clean up and terminate the use of the Winsock library. It is called after the client is done using the Winsock library and wants to release any resources that were allocated during the initialization of the library. This function should be called once for every successful call to `WSAStartup()`. */
                WSACleanup();
            }
        }
        Client()
        {
            CreateSocket();
            SetInformation();
        }
        ~Client()
        {
            CloseSocket();
        }

    private:
    };
}