#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> 
#include <sys/wait.h>

int main(){
   printf("p1id: %d\t p1 parentid:%d \n", getpid(),getppid()); 
   
   int p2;
   p2 = fork();
   if (p2 == 0){
      printf("p2id:%d\t p2parentid:%d\n", getpid(), getppid());
      int p4;
      p4 = fork();
      if (p4 == 0){
         printf("p4id:%d\t p4parentid:%d\n", getpid(), getppid());
         exit(0);
      }
      wait(NULL);
      exit(0);
   }
   
   int p3;
   p3 = fork();
   if(p3 == 0){
      printf("p3 id:%d\t p3parentid:%d\n", getpid(), getppid());
      int p5;
      p5 = fork();
      if(p5 == 0){
         printf("p5id:%d\t p5parentid:%d\n", getpid(), getppid()); 
         exit(0); 
      }
      wait(NULL); 
      exit(0);    
   }
   
   wait(NULL); 
   wait(NULL); 
   printf("p1 done %d\n", getpid()); 
   return 0;
}
