// C program to demonstrate the use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent to child
    // Second pipe to send input string from child to parent
  
    int fd1[2];  // Used to store two ends of the first pipe (P1 to P2)
    int fd2[2];  // Used to store two ends of the second pipe (P2 to P1)
  
    char fixed_str1[] = "howard.edu";
    char fixed_str2[] = "gobison.org";
    char input_str[100]; 
    char received_str[100];
    pid_t p; 
  
    if (pipe(fd1) == -1 || pipe(fd2) == -1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
  
    printf("Enter a string to concatenate:");
    scanf("%s", input_str); 
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process (P1)
    else if (p > 0) 
    { 
        close(fd1[0]);  // Close reading end of the first pipe
        close(fd2[1]);  // Close writing end of the second pipe
  
        // Write input string to the child (P2) and close writing end of the first pipe. 
        write(fd1[1], input_str, strlen(input_str) + 1); 

        // Wait for the child (P2) to send a string 
        wait(NULL); 

        // Read the concatenated string from the child (P2)
        read(fd2[0], received_str, 100);

        printf("Concatenated string received from P2: %s\n", received_str);
  
        close(fd1[1]); // Close writing end of the first pipe
        close(fd2[0]); // Close reading end of the second pipe
    } 
  
    // Child process (P2)
    else
    { 
        close(fd1[1]);  // Close writing end of the first pipe
        close(fd2[0]);  // Close reading end of the second pipe
      
        // Read a string from the parent (P1) using the first pipe 
        read(fd1[0], received_str, 100); 
  
        // Concatenate a fixed string with it 
        int k = strlen(received_str); 
        int i; 
        for (i = 0; i < strlen(fixed_str1); i++) 
            received_str[k++] = fixed_str1[i]; 
  
        received_str[k] = '\0';   // string ends with '\0' 

        // Print the concatenated string
        printf("Concatenated string in P2: %s\n", received_str);

        // Write the string back to the parent (P1) using the second pipe
        write(fd2[1], received_str, strlen(received_str) + 1);

        // Close both reading ends 
        close(fd1[0]); 
        close(fd2[1]); 

        exit(0); 
    } 
}
