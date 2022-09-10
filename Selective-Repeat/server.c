// Server (Selective Repeat)
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
void rsendd(int ch, int c_sock)
{
    char buff2[60];
    bzero(buff2, sizeof(buff2));
    strcpy(buff2, "reserver message :");
    buff2[strlen(buff2)] = (ch) + '0';
    buff2[strlen(buff2)] = '\0';
    printf("Resending Message to client :%s \n", buff2);
    write(c_sock, buff2, sizeof(buff2));
    usleep(1000);
}
int main()
{
    int s_sock, c_sock;
    s_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server, other;
    memset(&server, 0, sizeof(server));
    memset(&other, 0, sizeof(other));
    server.sin_family = AF_INET;
    server.sin_port = htons(9009);
    server.sin_addr.s_addr = INADDR_ANY;
    socklen_t add;

    if (bind(s_sock, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        printf("Binding failed\n");
        return 0;
    }

    printf("Server Up\nSelective repeat scheme\n\n");
    listen(s_sock, 10);
    add = sizeof(other);
    c_sock = accept(s_sock, (struct sockaddr *)&other, &add);

    time_t t1, t2;
    char msg[50] = "server message :";
    char buff[50];
    int flag = 0;
    fd_set set1, set2, set3;
    struct timeval timeout1, timeout2, timeout3;
    int rv1, rv2, rv3;
    int tot = 0;
    int ok[20];

    memset(ok, 0, sizeof(ok));
    while (tot < 9)
    {
        int toti = tot;
        for (int j = (0 + toti); j < (3 + toti); j++)
        {
            bzero(buff, sizeof(buff));
            char buff2[60];
            bzero(buff2, sizeof(buff2));
            strcpy(buff2, "server message :");
            buff2[strlen(buff2)] = (j) + '0';
            buff2[strlen(buff2)] = '\0';
            printf("Message sent to client :%s \t%d\t%d\n", buff2, tot, j);
            write(c_sock, buff2, sizeof(buff2));
            usleep(1000);
        }
        for (int k = 0 + toti; k < (toti + 3); k++)
        {
        qq: FD_ZERO(&set1);
            FD_SET(c_sock, &set1);
            timeout1.tv_sec = 2;
            timeout1.tv_usec = 0;

            rv1 = select(c_sock + 1, &set1, NULL, NULL, &timeout1);
            if (rv1 == -1)
                perror("select error ");
            else if (rv1 == 0)
            {
                printf("Timeout for message :%d \n", k);
                rsendd(k, c_sock);
                goto qq;
            } // a timeout occured
            else
            {
                read(c_sock, buff, sizeof(buff));
                printf("Message from Client: %s\n", buff);
                if (buff[0] == 'n')
                {
                    printf(" corrupt message acknowledgement (msg %d) \n", buff[strlen(buff) - 1] - '0');
                    rsendd((buff[strlen(buff) - 1] - '0'), c_sock);
                    goto qq;
                }
                else
                    tot++;
            }
        }
    }
    close(c_sock);
    close(s_sock);

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