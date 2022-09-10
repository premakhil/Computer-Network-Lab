// Stop and Wait Protocol
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int i, j = 0, noframes, x, x1 = 10, x2;

    printf("Enter the number of frames: ");
    scanf("%d", &noframes);
    printf("\nnumber of frames is %d", noframes);

    while (noframes > 0)
    {
        printf("\nsending frame %d", i);
        srand(x1++); // The srand() function sets the starting point for producing a series of pseudo-random integers
        x = rand() % 10;
        if (x % 2 == 0)
        {
            for (x2 = 1; x2 < 2; x2++)
            {
                printf("\nwaiting for %d seconds\n", x2);
                sleep(x2);
                printf("Missing Acknowledgement %d", i);
            }
            printf("\nsending frame %d", i);
            srand(x1++);
            x = rand() % 10;
        }
        printf("\nack received for frame %d", j);
        noframes -= 1;
        i++;
        j++;
    }
    printf("\nend of stop and wait protocol\n");
    
    return 0;
}

/*
$ ./a.out
Enter the number of frames: 6

number of frames is 6
sending frame 0
ack received for frame 0
sending frame 1
ack received for frame 1
sending frame 2
waiting for 1 seconds
Missing Acknowledgement 2
sending frame 2
ack received for frame 2
sending frame 3
ack received for frame 3
sending frame 4
ack received for frame 4
sending frame 5
waiting for 1 seconds
Missing Acknowledgement 5
sending frame 5
ack received for frame 5
end of stop and wait protocol
*/