// Client (Go Back N)
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int c_sock;
    c_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons(9009);
    client.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(c_sock, (struct sockaddr *)&client, sizeof(client)) == -1)
    {
        printf("Connection failed\n");
        return 0;
    }
    printf("Client -with individual acknowledgement scheme\n\n");
    char msg1[50] = "acknowledgement of: ";
    char msg2[50];
    char buff[100];
    int flag = 1, flg = 1;
    for (int i = 0; i <= 9; i++)
    {
        flg = 1;
        bzero(buff, sizeof(buff));
        bzero(msg2, sizeof(msg2));
        if (i == 8 && flag == 1)
        {
            printf("here\n"); // simulating loss
            flag = 0;
            read(c_sock, buff, sizeof(buff));
        }
        int n = read(c_sock, buff, sizeof(buff));
        if (buff[strlen(buff) - 1] != i + '0')
        { // out of order
            printf("Discarded as out of order \n");
            i--;
        }
        else
        {
            printf("Message received from server: %s \t %d\n", buff, i);
            printf("Acknowledgement sent for message \n");
            strcpy(msg2, msg1);
            msg2[strlen(msg2)] = i + '0';
            write(c_sock, msg2, sizeof(msg2));
        }
    }
    close(c_sock);
    return 0;
}

/*
$ ./server
Server Up
Go back n (n=3) used to send 10 messages


$ ./client
Client -with individual acknowledgement scheme

Message received from server: server message: 0          0
Acknowledgement sent for message
Message received from server: server message: 1          1
Acknowledgement sent for message
Message received from server: server message: 2          2
Acknowledgement sent for message
Message received from server: server message: 3          3
Acknowledgement sent for message
Message received from server: server message: 4          4
Acknowledgement sent for message
Message received from server: server message: 5          5
Acknowledgement sent for message
Message received from server: server message: 6          6
Acknowledgement sent for message
Message received from server: server message: 7          7
Acknowledgement sent for message
here
Discarded as out of order
Message received from server: server message: 8          8
Acknowledgement sent for message
Message received from server: server message: 9          9
Acknowledgement sent for message

$ ./server
Server Up
Go back n (n=3) used to send 10 messages

Message sent to client: server message: 0
Message sent to client: server message: 1
Message sent to client: server message: 2
Message from Client: acknowledgement of: 0
Message sent to client: server message: 3
Message from Client: acknowledgement of: 1
Message sent to client: server message: 4
Message from Client: acknowledgement of: 2
Message sent to client: server message: 5
Message from Client: acknowledgement of: 3
Message sent to client: server message: 6
Message from Client: acknowledgement of: 4
Message sent to client: server message: 7
Message from Client: acknowledgement of: 5
Message sent to client: server message: 8
Message from Client: acknowledgement of: 6
Message sent to client: server message: 9
Message from Client: acknowledgement of: 7
Going back from 9:timeout on last 2
Message from Client: acknowledgement of: 8
Message from Client: acknowledgement of: 9
*/