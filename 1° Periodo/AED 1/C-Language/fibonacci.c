//a program that shows n numbers from the Fibonacci sequence, n being given by the user

#include <stdio.h> 
// to print the sequence
void fibonacci(int n) {
    int first = 0, second = 1; // declares and initializes the first two variables of the sequence

   
    for (int i = 0; i < n; i++) {
        printf("i = %d numero = %d\n", i, first);
        int next = first + second; // calculating the next number of the sequence
        first = second; // updates first
        second = next; // updates second
    }
}

int main() {
    int n; 
    printf("Type the value of N: "); 
    scanf("%d", &n); 

    
    if (n <= 0) {
        printf("Please, write a number bigger than 0.\n"); 
        return 1; // ends the program with an error
    }

    
    fibonacci(n); 
    return 0; 
}
