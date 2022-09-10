#include <time.h>
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
int isfaulty()
{ // simulating corruption of message
    int d = rand() % 4;
    return (d > 2);
}
int main()
{
    srand(time(0));
    int c_sock;
    c_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons(9009);
    client.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(c_sock, (struct sockaddr *)&client, sizeof(client)) == -1)
    {
        printf("Connection failed");
        return 0;
    }
    printf("\nClient -with individual acknowledgement scheme\n\n");
    char msg1[50] = "acknowledgement of ";
    char msg3[50] = "negative ack ";
    char msg2[50];
    char buff[100];
    int count = -1, flag = 1;
    while (count < 8)
    {
        bzero(buff, sizeof(buff));
        bzero(msg2, sizeof(msg2));

        if (count == 7 && flag == 1)
        {
            printf("here\n"); // simulate loss
            flag = 0;
            read(c_sock, buff, sizeof(buff));
            continue;
        }

        int n = read(c_sock, buff, sizeof(buff));
        char i = buff[strlen(buff) - 1];
        printf("Message received from server : %s \n", buff);
        int isfault = isfaulty();
        printf("corruption status : %d \n", isfault);
        printf("Response/acknowledgement sent for message \n");

        if (isfault)
            strcpy(msg2, msg3);
        else
        {
            strcpy(msg2, msg1);
            count++;
        }
        msg2[strlen(msg2)] = i;
        write(c_sock, msg2, sizeof(msg2));
    }
    close(c_sock);
    return 0;
}

/*
$ ./server
Server Up
Selective repeat scheme



$ ./client

Client -with individual acknowledgement scheme

Message received from server : server message :0
corruption status : 1
Response/acknowledgement sent for message
Message received from server :
corruption status : 0
Response/acknowledgement sent for message
Message received from server : reserver message :0
corruption status : 0
Response/acknowledgement sent for message
Message received from server : reserver message :2
corruption status : 0
Response/acknowledgement sent for message
Message received from server : server message :3
corruption status : 0
Response/acknowledgement sent for message
Message received from server : server message :4
corruption status : 0
Response/acknowledgement sent for message
Message received from server : server message :5
corruption status : 1
Response/acknowledgement sent for message
Message received from server : reserver message :5
corruption status : 1
Response/acknowledgement sent for message
Message received from server : reserver message :5
corruption status : 0
Response/acknowledgement sent for message
Message received from server : server message :6
corruption status : 0
Response/acknowledgement sent for message
Message received from server : server message :7
corruption status : 0
Response/acknowledgement sent for message
here
Message received from server : reserver message :8
corruption status : 0
Response/acknowledgement sent for message

$ ./server
Server Up
Selective repeat scheme

Message sent to client :server message :0       0       0
Message sent to client :server message :1       0       1
Message sent to client :server message :2       0       2
Message from Client: negative ack 0
 corrupt message acknowledgement (msg 0)
Resending Message to client :reserver message :0
Message from Client: acknowledgement of
Message from Client: acknowledgement of 0
Timeout for message :2
Resending Message to client :reserver message :2
Message from Client: acknowledgement of 2
Message sent to client :server message :3       3       3
Message sent to client :server message :4       3       4
Message sent to client :server message :5       3       5
Message from Client: acknowledgement of 3
Message from Client: acknowledgement of 4
Message from Client: negative ack 5
 corrupt message acknowledgement (msg 5)
Resending Message to client :reserver message :5
Message from Client: negative ack 5
 corrupt message acknowledgement (msg 5)
Resending Message to client :reserver message :5
Message from Client: acknowledgement of 5
Message sent to client :server message :6       6       6
Message sent to client :server message :7       6       7
Message sent to client :server message :8       6       8
Message from Client: acknowledgement of 6
Message from Client: acknowledgement of 7
Timeout for message :8
Resending Message to client :reserver message :8
Message from Client: acknowledgement of 8
*/