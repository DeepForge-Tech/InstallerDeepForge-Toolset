// #include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "json/json.h"
#include <iostream>
#include <fstream>

#define PORT 743
#define BUFFER_MAX_LENGHT 16384
#define IP_ADDRESS "64.226.99.105"

using namespace std;
namespace UNIX
{
    int sockfd;
    char buffer[BUFFER_MAX_LENGHT];
    struct sockaddr_in servaddr;
    int n;
    socklen_t len;
    bool isOpen = false;

    class Client
    {
    public:
        void CreateSocket()
        {
            try
            {
                // Creating socket file descriptor
                if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
                {
                    perror("socket creation failed");
                    exit(EXIT_FAILURE);
                }
            }
            catch (const std::exception &e)
            {
                throw runtime_error("Error creating socket");
            }
            /* `memset(&servaddr, 0, sizeof(servaddr));` is used to set all the bytes of the `servaddr` structure to zero. This is done to ensure that there are no garbage values present in the structure before assigning values to its members. */
            memset(&servaddr, 0, sizeof(servaddr));
            isOpen = true;
        }
        void SetInformation()
        {
            // Filling server information
            servaddr.sin_family = AF_INET;
            servaddr.sin_port = htons(PORT);
            servaddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
        }
        /* The `SEND_JSON_ARRAY` function is sending a JSON array over a UDP socket connection. */
        void SEND_JSON_ARRAY(string data)
        {
            char new_data[BUFFER_MAX_LENGHT];
            strcpy(new_data,data.c_str());
            /* The `sendto()` function is used to send data over a UDP socket connection. In this specific code snippet, it is sending the contents of the `data` string to the server specified by the `servaddr` structure. */
            sendto(sockfd, new_data, strlen(new_data), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
        }
        /* The `SEND_JSON_FILE` function is responsible for reading the contents of a file specified by the `path` variable and sending it over a UDP socket connection. */
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
            /* The `sendto()` function is used to send data over a UDP socket connection. In this specific code snippet, it is sending the contents of the `data` string to the server specified by the `servaddr` structure. The parameters of the `sendto()` function are as follows: */
            sendto(sockfd, data, strlen(data), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
        }
        void SEND_TEXT_MESSAGE(string message)
        {
            /* The line `sendto(client_socket, new_message, strlen(new_message), 0, (sockaddr*)&server, sizeof(sockaddr_in));` is sending a text message over a UDP socket to a specified server. */
            sendto(sockfd, message.c_str(), strlen(message.c_str()), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
        }
        void GetInformation()
        {
            /* The line `n = recvfrom(sockfd, (char *)buffer, BUFFER_MAX_LENGHT, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);` is receiving data from a UDP socket connection. */
            n = recvfrom(sockfd, (char *)buffer, BUFFER_MAX_LENGHT, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
            buffer[n] = '\0';
        }
        void CloseSocket()
        {
            if (isOpen == true)
            {
                close(sockfd);
                isOpen = false;
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