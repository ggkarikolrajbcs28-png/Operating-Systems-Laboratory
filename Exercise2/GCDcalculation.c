#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int gcd(int a, int b)
{
    while (b != 0)
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int main()
{
    int fd[2];
    pid_t pid;
    int num1, num2;

    if (pipe(fd) == -1)
    {
        perror("Pipe failed");
        exit(1);
    }
    pid = fork();

    if (pid < 0)
    {
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0)
    {
        close(fd[0]);
        printf("Child: Enter two numbers: ");
        scanf("%d %d", &num1, &num2);
        write(fd[1], &num1, sizeof(int));
        write(fd[1], &num2, sizeof(int));
        close(fd[1]);
    }
    else
    {
        close(fd[1]);
        read(fd[0], &num1, sizeof(int));
        read(fd[0], &num2, sizeof(int));
        close(fd[0]);
        wait(NULL);
        printf("Parent: Numbers received = %d and %d\n", num1, num2);
        printf("Parent: GCD = %d\n", gcd(num1, num2));
    }
    return 0;
}
