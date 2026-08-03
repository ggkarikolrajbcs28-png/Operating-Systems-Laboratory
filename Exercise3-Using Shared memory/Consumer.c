#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

int main()
{
    int shmid, *buf;
    int num;
    int SIZE = 10;
    int prod_pid;

    shmid = shmget(7888, sizeof(int) * (SIZE + 3), 0666);

    if (shmid == -1)
    {
        perror("shmget");
        return 0;
    }

    buf = (int *)shmat(shmid, NULL, 0);

    if (buf == (void *)-1)
    {
        perror("shmat");
        return 0;
    }
    
    printf("Consumer started. Waiting for data...\n");

    while (1)
    {
        prod_pid = buf[12]; // Fetch the producer's process ID

        if (kill(prod_pid, 0) == -1 && errno == ESRCH)
        {
            printf("\n[Alert] Producer terminated. Exiting consumer automatically...\n");
            break; 
        }

        while (buf[10] == buf[11])
        {
            usleep(10000); // Sleep 10ms

            if (kill(prod_pid, 0) == -1 && errno == ESRCH)
            {
                printf("\n[Alert] Producer terminated while waiting. Exiting...\n");
                shmdt(buf);
                shmctl(shmid, IPC_RMID, NULL); // Clean up shared memory segment
                return 0;
            }
        }

        num = buf[buf[11]];             
        printf("Data Read: %d\n", num);

        buf[11] = (buf[11] + 1) % SIZE; 
    }

    shmdt(buf);
    shmctl(shmid, IPC_RMID, NULL); 
    return 0;
}
