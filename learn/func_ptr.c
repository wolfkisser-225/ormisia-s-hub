#include<stdio.h>
#include<stdlib.h>

int add(int a, int b) {
    return a + b;
}

int main() {
    int (*func_ptr)(int, int) = add; // Function pointer declaration and initialization
    int result = func_ptr(5, 3); // Using the function pointer to call the function
    printf("Result of addition: %d\n", result);
    return 0;
}