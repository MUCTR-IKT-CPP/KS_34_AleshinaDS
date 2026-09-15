// laba1v8.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <cstdlib>  
#include <ctime>

/*
 * Переворачивает часть массива.
 *
 * @param arr массив целых чисел.
 * @param left индекс начала участка.
 * @param right индекс конца участка.
 */
void reverseArray(int arr[], int left, int right) {
    while (left < right) {
        int temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;

        left++;
        right--;
    }
}
/*
* Выполняет циклический сдвиг массива вправо.
*
* @param arr массив целых чисел.
* @param k величина сдвига.
* @param N размер массива.
*/
    void shiftArray(int arr[], int k, const int N) {
        k = k % N;

        reverseArray(arr, 0, N - 1);
        reverseArray(arr, 0, k - 1);
        reverseArray(arr, k, N - 1);

    }
/*
 * Выводит массив на экран.
 *
 * @param arr массив целых чисел.
 * @param N размер массива.
 */
void printArray(int arr[], const int N) {
    for (int i = 0; i < N; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

/*
 * Вычисляет сумму элементов массива.
 *
 * @param arr массив целых чисел.
 * @param N размер массива.
 * @return возвращает сумму элементов массива.
 */
int sumArray(int arr[], const int N) {
    int sum = 0;
    for (int i = 0; i < N; i++) {
        sum += arr[i];
    }
    return sum;

}

/*
 * Заполняет массив случайными числами.
 *
 * @param arr массив целых чисел.
 * @param N размер массива.
 */
void generateArray(int arr[], const int N) {
    for (int i = 0; i < N; i++) {
        arr[i] = rand() % 10;
    }
}

int main()
{
    srand(time(0));
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
