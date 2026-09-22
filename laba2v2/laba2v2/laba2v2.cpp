// laba2v2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cstdlib>
#include <ctime>
/**
 * Выводит матрицу на экран с передачей через ссылку на указатель.
 *
 * @param p_matrix ссылка на указатель матрицы
 * @param N размер матрицы
 */
void printMatrixByReference(char**& p_matrix, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << p_matrix[i][j] << ' ';
        }

        std::cout << '\n';
    }
}
/**
 * Строит комплементарную матрицу.
 *
 * A заменяется на T.
 * T заменяется на A.
 * G заменяется на C.
 * C заменяется на G.
 *
 * @param p_matrix указатель на матрицу
 * @param N размер матрицы
 */
void makeComplement(char** p_matrix, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            switch (p_matrix[i][j]) {
                case 'A':
                    p_matrix[i][j] = 'T';
                    break;
                case 'T':
                    p_matrix[i][j] = 'A';
                    break;

                case 'G':
                    p_matrix[i][j] = 'C';
                    break;

                case 'C':
                    p_matrix[i][j] = 'G';
                    break;
            }

        }
    }

}
/**
 * Находит самую длинную последовательность
 * одинаковых символов в строках и столбцах матрицы.
 *
 * @param p_matrix указатель на матрицу
 * @param N размер матрицы
 */
void findLongestSequence(char** p_matrix, int N) {
    int max_length = 1;
    char max_letter = p_matrix[0][0];

    for (int i = 0; i < N; i++) {
        int current_length = 1;

        for (int j = 1; j < N; j++) {
            if (p_matrix[i][j] == p_matrix[i][j - 1]) {
                current_length++;
            }
            else {
                current_length = 1;
            }
            if (current_length > max_length) {
                max_length = current_length;
                max_letter = p_matrix[i][j];

            }
        }
    }
    for (int j = 0; j < N; j++) {
        int current_length = 1;
        for (int i = 1; i < N; i++) {
            if (p_matrix[i][j] == p_matrix[i - 1][j]) {
                current_length++;
            }
            else {
                current_length = 1;
            }
            if (current_length > max_length) {
                max_length = current_length;
                max_letter = p_matrix[i][j];
            }
        }
    }
    std::cout << "\n Longest sequence:\n";
    std::cout << "char: " << max_letter << '\n';
    std::cout << "lenght: " << max_length << '\n';

}


/**
 * Подсчитывает количество вхождений каждой буквы
 * в матрице.
 *
 * @param p_matrix указатель на матрицу
 * @param N размер матрицы
 */
void countLetters(char** p_matrix, int N) {
    int count_A = 0;
    int count_T = 0;
    int count_G = 0;
    int count_C = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {

            switch (p_matrix[i][j]) {
            case 'A':
                count_A++;
                break;
            case 'T':
                count_T++;
                break;

            case 'G':
                count_G++;
                break;

            case 'C':
                count_C++;
                break;

            }

        }
    }
    std::cout << "A: " << count_A << '\n';
    std::cout << "T: " << count_T << '\n';
    std::cout << "G: " << count_G << '\n';
    std::cout << "C: " << count_C << '\n';
}
/**
 * Выделяет память для двумерной матрицы.
 *
 * @param p_matrix ссылка на указатель матрицы
 * @param N размер матрицы
 */
void createMatrix(char**& p_matrix, int N) {
    p_matrix = new char* [N];
    for (int i = 0; i < N; i++) {
        p_matrix[i] = new char[N];
    }

}
/**
 * Заполняет матрицу случайными символами A, T, G и C.
 *
 * @param p_matrix указатель на матрицу
 * @param N размер матрицы
 */
void fillMatrix(char** p_matrix, int N) {
    char letters[4] = { 'A', 'T', 'G', 'C' };
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            p_matrix[i][j] = letters[rand() % 4];

        }
    }
}
/**
 * Освобождает память, выделенную для матрицы.
 *
 * @param p_matrix указатель на матрицу
 * @param N размер матрицы
 */
void deleteMatrix(char** p_matrix, int N) {
    for (int i = 0; i < N; i++) {
        delete[] p_matrix[i];
    }
    delete[] p_matrix;
}
/**
 * Выводит матрицу на экран.
 *
 * @param p_matrix указатель на матрицу
 * @param N размер матрицы
 */
void printMatrix(char** p_matrix, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << p_matrix[i][j] << ' ';
      
        }
        std::cout << '\n';
    }
}

int main()
{
    srand(time(0));
    int N = 0;
    std::cout << "Enter N";
    std::cin >> N;

    if (N <= 0) {
        std::cout << " need N > 0";
        return 0;
    }
    int choice = 0;
    char** p_matrix = nullptr;
    createMatrix(p_matrix, N);
    fillMatrix(p_matrix, N);
    std::cout << "\nMatrix:\n";
    printMatrix(p_matrix, N);
    
    char again;

    do {
        std::cout << "\nChoose an action:\n";
        std::cout << "1 - Count the occurrences of each letter\n";
        std::cout << "2 - Find the longest sequence of identical symbols\n";
        std::cout << "3 - Build the complementary matrix\n";
        std::cout << "4 - Pass the matrix to a function by reference to a pointer\n";
        std::cout << "Your choice: ";

        std::cin >> choice;

        if (choice == 1) {
            countLetters(p_matrix, N);
        }
        else if (choice == 2) {
            findLongestSequence(p_matrix, N);
        }
        else if (choice == 3) {
            makeComplement(p_matrix, N);

            std::cout << "\nComplementary matrix:\n";
            printMatrix(p_matrix, N);
        }
        else if (choice == 4) {
            printMatrixByReference(p_matrix, N);
        }
        else {
            std::cout << "No such action.\n";
        }

        std::cout << "\nDo you want to choose another action? (y/n): ";
        std::cin >> again;

    } while (again == 'y' || again == 'Y');
    
    
    deleteMatrix(p_matrix, N);



    


    
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
