// laba1v8.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <cstdlib>  
#include <ctime>

void reverseArray(int arr[], int left, int right) {
    while (left < right) {
        int temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;

        left++;
        right--;
    }
}
    void shiftArray(int arr[], int k, int N) {
        k = k % N;

        reverseArray(arr, 0, N - 1);
        reverseArray(arr, 0, k - 1);
        reverseArray(arr, k, N - 1);

    }



void printArray(int arr[], int N) {
    for (int i = 0; i < N; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

int sumArray(int arr[], const int N) {
    int sum = 0;
    for (int i = 0; i < N; i++) {
        sum += arr[i];
    }
    return sum;

}

void generateArray(int arr[], const int N) {
    srand(time(0));
    for (int i = 0; i < N; i++) {
        arr[i] = rand() % 10;
    }
}

int main()
{
    std::cout << "Hello World!\n";
    const int N = 5;
    int arr[N] = {};
    int k = 0;

    std::cout << "Enter the step\n";
    std::cin >> k;

    generateArray(arr, N);
    printArray(arr, N);
    shiftArray(arr, k, N);
    std::cout << "output: ";
    printArray(arr, N);
    std::cout << "sum: " << sumArray(arr, N) << "\n";



}
