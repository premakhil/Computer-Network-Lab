// Client (SMTP)  
#include<string.h>
#include<sys/socket.h>
#include<netdb.h>
#include<stdlib.h>
#include<stdio.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<netinet/in.h>
#include <unistd.h>
#define BUF_SIZE 256

int main(int argc, char* argv[])
{
    struct sockaddr_in server, client;
    char str[50] = "hi";
    char mail_f[50], mail_to[50], msg[20], c;
    int t = 0;
    socklen_t l = sizeof (server);
    if (argc != 3)
        printf ("Input format not correct!");
    int sockfd = socket (AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
        printf ("Error in socket()!");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons (atoi (argv[2]));

    client.sin_family = AF_INET;
    client.sin_addr.s_addr = INADDR_ANY;
    client.sin_port = htons (atoi (argv[2]));

    printf ("Sending 'hi' to server...");
    sleep (10);
    if (sendto (sockfd, str, sizeof (str), 0, (struct sockaddr*) &server, sizeof (server)) < 0)
        printf ("Error in sendto()!");
    if (recvfrom (sockfd, str, sizeof (str), 0, (struct sockaddr *) &server, &l) < 0)
        printf ("Error in recvfrom()!");
    printf ("\ngreeting msg is %s", str);
    if (strncmp (str, "220", 3))
        printf ("\nConnection not established \ncode 220 expected");
    printf ("\nSending 'HELLO'...");
    strcpy (str, "HELLO 127.0.0.1");
    if (sendto (sockfd, str, sizeof (str), 0, (struct sockaddr *) &server, sizeof (server)) < 0)
        printf ("Error in sendto()!");
    sleep (3);
    printf ("\nReceiving from server");
    if ((recvfrom (sockfd, str, sizeof (str), 0, (struct sockaddr *) &server, &l)) < 0)
        printf ("Error in recvfrom()!");
    if (strncmp (str, "250", 3))
        printf ("\nOk not received from server!");
    printf ("\nServer has send %s",str);

    printf ("\n\nEnter FROM address: ");
    scanf ("%s", mail_f);
    strcpy (str, "MAIL FROM "); 
    strcat (str, mail_f);
    if (sendto (sockfd, str, sizeof (str), 0, (struct sockaddr *) &server, sizeof (server)) < 0)
    printf ("Error in sendto()!");
    sleep (3);
    if ((recvfrom (sockfd, str, sizeof (str), 0, (struct sockaddr *) &server, &l)) < 0)
    printf ("Error in recvfrom()!");
    if (strncmp(str, "250", 3))
    printf ("\nOk not received from server!");

    printf ("%s", str);
    printf ("\nEnter TO address: ");
    scanf ("%s", mail_to);
    strcpy (str, "RCPT TO ");
    strcat (str, mail_to);
    if (sendto (sockfd, str, sizeof (str), 0, (struct sockaddr *) &server, sizeof (server)) < 0)
        printf ("Error in sendto()!");
    sleep (3);
    if ((recvfrom (sockfd, str, sizeof (str), 0, (struct sockaddr *) &server, &l)) < 0)
        printf ("Error in recvfrom()!");
    if (strncmp (str, "250", 3))
        printf ("\nOk not received from server!");

    printf ("%s", str);
    printf ("\nSending 'DATA' to server...");
    strcpy (str, "DATA");
    if (sendto (sockfd, str, sizeof (str), 0, (struct sockaddr *) &server, sizeof (server)) < 0)
        printf ("Error in sendto()!");
    sleep (3);
    if ((recvfrom (sockfd, str, sizeof (str), 0, (struct sockaddr *) &server, &l)) < 0)
        printf ("Error in recvfrom()!");
    if (strncmp (str, "354", 3))
        printf ("\nOk not received from server!");
    printf ("%s", str);
    printf ("\nEnter mail body: ");
    while (1)
    {
        c = getchar ();
        if (c == '$')
        {
            msg[t] = '\0';
            break;
        }
        if (c == '\0')
            continue;
        msg[t++] = c;
    }

    if (sendto (sockfd, msg, sizeof (msg), 0, (struct sockaddr *) &server, sizeof(server)) < 0)
        printf("Error in sendto()!");
    sleep(3);
    printf("\nSending 'QUIT' to server...");
    strcpy(str,"QUIT");

    if (sendto (sockfd, str, sizeof (str), 0, (struct sockaddr *) &server,sizeof (server)) < 0)
        printf("Error in sendto()!");
    if ((recvfrom (sockfd, str, sizeof (str), 0, (struct sockaddr *) &server, &l)) < 0)
        printf ("Error in recvfrom()!");

    if (strncmp (str, "221", 3))
        printf ("\nOk not received from server!");

    printf ("\nServer has send GOODBYE... \nClosing connection...\n");
    printf ("Bye\n");

    close (sockfd);
    return 0;
}

/*
$ ./server 3000
Server waiting...
Got message from client: hi
Sending greeting message to client...
HELLO 127.0.0.1
Sending response...
MAIL FROM studentforum.mec@gmail.com
Sending response...
RCPT TO studentforum.cet@gmail.com
Sending response...
DATA
Sending response...
mail body received

hi programmers!
Sending quit...

$ ./client localhost 3000
Sending 'hi' to server...
greeting msg is 220 127.0.0.1
Sending 'HELLO'...
Receiving from server
Server has send 250 ok

Enter FROM address: studentforum.mec@gmail.com
250 ok
Enter TO address: studentforum.cet@gmail.com
250 ok
Sending 'DATA' to server...354 Go ahead
Enter mail body: hi programmers! $

Sending 'QUIT' to server...
Server has send GOODBYE...
Closing connection...
Bye
*/