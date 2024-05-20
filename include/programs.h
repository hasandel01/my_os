#ifndef _PROGRAMS_H
#define _PROGRAMS_H

#include <common/types.h>

namespace myos {
    void long_running_program(int n);
    void collatz(int n);
    int binary_search(int arr[], int low, int high, int x);
    int linear_search(int arr[], int size, int x);
    void ep_long_running_program();
    void ep_collatz();
    void ep_binary_search();
    void ep_linear_search();
    void quickSort(int arr[], int low, int high);
    int partition(int arr[], int low, int high);
    void swap(int arr[], int i, int j);
}

#endif