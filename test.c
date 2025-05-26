/* Example Mini-C program demonstrating various features */

/* Function to calculate factorial */
int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

/* Function to find maximum element in an array */
int find_max(int arr[], int size) {
    int max = arr[0];
    int i = 1;
    
    while (i < size) {
        if (arr[i] > max) {
            max = arr[i];
        }
        i = i + 1;
    }
    
    return max;
}

/* Main function */
void main() {
    int numbers[5];
    int i;
    int fact;
    
    /* Initialize array */
    i = 0;
    while (i < 5) {
        numbers[i] = i + 1;
        i = i + 1;
    }
    
    /* Find maximum */
    int max = find_max(numbers, 5);
    
    /* Calculate factorial of maximum */
    fact = factorial(max);
    
    /* The compiler doesn't support printf, but in a full implementation,
       we would print the results here */
} 