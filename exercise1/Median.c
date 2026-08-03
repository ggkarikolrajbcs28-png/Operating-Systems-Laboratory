#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void medi(int n, int arr[]) {
   int i, j;
   for (i = 0; i < n - 1; i++) {
      for (j = 0; j < n - i - 1; j++) { 
         if (arr[j] > arr[j + 1]) {
            int t = arr[j];
            arr[j] = arr[j + 1];
            arr[j + 1] = t;
         }
      }
   }
   float median; 
   if (n % 2 == 0) {
      median = (arr[n / 2 - 1] + arr[n / 2]) / 2.0;
   }
   else {
      median = arr[n / 2];
   }
   printf("Median : %.2f\n", median);
}

int main() {
   int n, i;
   printf("Enter the value of n : ");
   if (scanf("%d", &n) != 1 || n <= 0) {
      return 1;
   }
   int arr[n];
   printf("Enter %d elements:\n", n);
   for(i = 0; i < n; i++) {
      scanf("%d", &arr[i]);
   }
   pid_t p2 = fork();
   if (p2 < 0) {
      perror("Fork failed");
      return 1;
   }
   else if (p2 == 0) {
      printf("Child Processing complete.\n");
   }
   else {
      wait(NULL);
      printf("Parent Calculating median:\n");
      medi(n, arr);
   }
   
   return 0;
}

