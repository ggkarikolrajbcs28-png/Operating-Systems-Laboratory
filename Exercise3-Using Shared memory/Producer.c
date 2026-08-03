#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>

int main()
{
    int shmid, *buf;
    int num;
    int SIZE = 10;

    // Allocate space for 10 data items + 3 control slots (in, out, producer_pid)
    shmid = shmget(7888, sizeof(int) * (SIZE + 3), IPC_CREAT | 0666);

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

    // Initialize tracking indexes
    buf[10] = 0;   // in pointer
    buf[11] = 0;   // out pointer
    buf[12] = getpid(); // Store Producer PID so consumer can monitor it
    
    printf("Producer started (PID: %d). Press Ctrl+C to terminate.\n", buf[12]);
    
    while (1)
    {
        // Check if buffer is full before asking for data
        if (((buf[10] + 1) % SIZE) == buf[11])
        {
            printf("[Buffer Full] Waiting for consumer to read space...\n");
            while (((buf[10] + 1) % SIZE) == buf[11])
            {
                usleep(100000); 
            }
        }

        printf("Enter the data: ");
        if (scanf("%d", &num) != 1) {
            break; 
        }

        buf[buf[10]] = num;             
        buf[10] = (buf[10] + 1) % SIZE; 
    }

    shmdt(buf);
    return 0;
