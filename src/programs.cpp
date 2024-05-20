#include <programs.h>
#include <prints.h>
#include <unixsyscalls.h>

using namespace myos;
using namespace myos::common;

namespace myos {

    void long_running_program(int n) {
        char buffer[256]; 

        long result = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                result += i * j;
            }
        }
        // Convert the result to a string
        itoa(result, buffer);
        // Print the result
        printf(buffer);
        printf("\n");
    }


    void collatz(int n) {
        char buffer[256];
        char num_buffer[10];
        int offset = 0;

        // Concatenate "Output " to buffer
        buffer[offset++] = 'O';
        buffer[offset++] = 'u';
        buffer[offset++] = 't';
        buffer[offset++] = 'p';
        buffer[offset++] = 'u';
        buffer[offset++] = 't';
        buffer[offset++] = ' ';

        // Convert integer to string and concatenate to buffer
        itoa(n, num_buffer);
        for (int i = 0; num_buffer[i] != '\0'; i++) {
            buffer[offset++] = num_buffer[i];
        }

        // Concatenate ": " to buffer
        buffer[offset++] = ':';
        buffer[offset++] = ' ';

        // Generate Collatz sequence
        while (n != 1) {
            // Convert integer to string and concatenate to buffer
            itoa(n, num_buffer);
            for (int i = 0; num_buffer[i] != '\0'; i++) {
                buffer[offset++] = num_buffer[i];
            }

            // Concatenate ", " to buffer
            buffer[offset++] = ',';
            buffer[offset++] = ' ';

            // Calculate next Collatz number
            if (n % 2 == 0)
                n = n / 2;
            else
                n = 3 * n + 1;
        }

        // Convert last number to string and concatenate to buffer
        itoa(n, num_buffer);
        for (int i = 0; num_buffer[i] != '\0'; i++) {
            buffer[offset++] = num_buffer[i];
        }

        // Concatenate newline character to buffer
        buffer[offset++] = '\n';
        
        // Null-terminate buffer
        buffer[offset] = '\0';

        // Print buffer using custom printf function
        printf(buffer);
        printf("\n");

    }


    int binary_search(int arr[], int low, int high, int x) {
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (arr[mid] == x)
                return mid;
            else if (arr[mid] < x)
                low = mid + 1;
            else
                high = mid - 1;
        }
        return -1; // element not found
    }


    int linear_search(int arr[], int size, int x) {
        for (int i = 0; i < size; i++) {
            if (arr[i] == x)
                return i;
        }
        return -1; // element not found
    }


    void ep_linear_search() {
        printf("Linear Search: \n");
        int array[10] = {10, 20, 80, 30, 60, 50, 110, 100, 130, 170};
        char res[20];
        int result = linear_search(array, 10, 175);
        printf("Input: {10, 20, 80, 30, 60, 50, 110, 100, 130, 170}, 175\n");
        printf("Output: ");

        if (result == -1) {
            printf("Value not found\n");
        } else {
            itoa(result, res);
            printf(res);
        }

        printf("\n");
        sys_exit();
        while (true);
    }

    void ep_binary_search() {
        printf("Binary Search: \n");
        int array[10] = {10, 20, 80, 30, 60, 50, 110, 100, 130, 170};
        char res[20];
        // Sort the array using Quick Sort algorithm
        quickSort(array, 0, 9);
        int result = binary_search(array,0,9,110);
        printf("Input: {10, 20, 80, 30, 60, 50, 110, 100, 130, 170}, 110");
        printf("Output: ");
        
        if (result == -1) {
            printf("Value not found\n");
        } else {
            itoa(result, res);
            printf(res);
        }
        printf("\n");
        sys_exit();
        while(true);
    }

    void ep_collatz() {
        int number = 100;
        collatz(number);
        sys_exit();
        while(true);
    }

    // Entry point for the long running program
    void ep_long_running_program() {
        long_running_program(1000);
        sys_exit();
        while(true);
    }


    // Swap function to swap two elements in an array
    void swap(int arr[], int i, int j) {
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    // Partition function for Quick Sort
    int partition(int arr[], int low, int high) {
        int pivot = arr[high]; // Choose the last element as the pivot
        int i = low - 1; // Index of smaller element

        for (int j = low; j < high; j++) {
            // If current element is smaller than or equal to the pivot
            if (arr[j] <= pivot) {
                i++; // Increment index of smaller element
                swap(arr, i, j); // Swap arr[i] and arr[j]
            }
        }
        swap(arr, i + 1, high); // Swap arr[i+1] and arr[high] (or pivot)
        return i + 1; // Return the partition index
    }

    // Quick Sort function
    void quickSort(int arr[], int low, int high) {
        if (low < high) {
            // Partition the array and get the partition index
            int pi = partition(arr, low, high);

            // Recursively sort elements before partition and after partition
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }

}