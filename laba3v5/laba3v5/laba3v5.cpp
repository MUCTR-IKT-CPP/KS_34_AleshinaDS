// laba3v5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

const int MIN_YEAR = 1950;
const int MAX_YEAR = 2025;
const int MIN_PAGES = 50;
const int MAX_PAGES = 1000;
const std::string TITLES[] = {
    "The Hobbit",
    "1984",
    "Dune",
    "The Great Gatsby",
    "Crime and Punishment"
};
const std::string AUTHORS[] = {
    "J.R.R. Tolkien",
    "George Orwell",
    "Frank Herbert",
    "F. Scott Fitzgerald",
    "Fyodor Dostoevsky"
};

 

struct Book {
    std::string isbn;
    std::string title;
    std::string author;
    int year;
    bool is_available;
    int pages;
};
/**
 * Выдаёт книгу по её ISBN, изменяя статус is_available на false.
 * Перед выдачей проверяет, что книга существует и доступна.
 * Если книга уже выдана или ISBN не найден, выводит сообщение об ошибке.
 *
 * @param p_books указатель на массив книг.
 * @param n количество книг в массиве.
 * @param isbn идентификатор книги для выдачи.
 */
void issueBook(Book* p_books, int n, const std::string& isbn) {
    for (int i = 0; i < n; i++) {
        if (p_books[i].isbn == isbn) {
            if (!p_books[i].is_available) {
                std::cout << "Error: book yet unavailable\n";
                return;
            }
            p_books[i].is_available = false;
            std::cout << "Book \"" << p_books[i].title << "\"unavailable .\n";
            return;
            
        }
    }
    std::cout << "Error: book  ISBN not found.\n";
 }
/**
 * Возвращает книгу по её ISBN, изменяя статус is_available на true.
 * Перед возвратом проверяет, что книга существует и была выдана.
 * Если книга уже доступна или ISBN не найден, выводит сообщение об ошибке.
 *
 * @param p_books указатель на массив книг.
 * @param n количество книг в массиве.
 * @param isbn идентификатор книги для возврата.
 */
void returnBook(Book* p_books, int n, const std::string& isbn) {
    for (int i = 0; i < n; i++) {
        if (p_books[i].isbn == isbn) {
            if (p_books[i].is_available) {
                std::cout << "Error: book  available\n";
                return;
            }
            p_books[i].is_available = true;
            std::cout << "Книга \"" << p_books[i].title << "\"returned .\n";
            return;

        }
    }
    std::cout << "Error: book  ISBN not found.\n";
}



/**
 * Подсчитывает и выводит статистику по каталогу:
 * общее количество книг, среднее количество страниц,
 * количество доступных и недоступных книг.
 *
 * @param p_books указатель на массив книг.
 * @param n количество книг в массиве.
 */

 void printStats(const Book* p_book, int n){
     double total_pages = 0;
     int available_count = 0;
     int unavailable_count = 0;

     for (int i = 0; i < n; i++) {
         total_pages += p_book[i].pages;
         if (p_book[i].is_available) {
             available_count++;
         }
         else {
             unavailable_count++;
         }
     }
      double average_pages = total_pages / n;
      std::cout << "All books: " << n << '\n';
      std::cout << "Average pages: " << average_pages << '\n';
      std::cout << "Available: " << available_count << '\n';
      std::cout << "Unavailable: " << unavailable_count << '\n';
 }

      
 

 /**
 * Запрашивает у пользователя два года (нижнюю и верхнюю границу)
 * и выводит все книги, год публикации которых попадает
 * в указанный диапазон включительно.
 * Если подходящих книг нет, сообщает об этом.
 *
 * @param p_books указатель на массив книг.
 * @param n количество книг в массиве.
 */
void filterBooks(const Book* p_books, int n) {
    int from_year = 0;
    int to_year = 0;
    std::cout << "enter year from: ";
    std::cin >> from_year;
    std::cout << "Enter year to: ";
    std::cin >> to_year;

    bool found = false;
    for (int i = 0; i < n; i++) {
        if (p_books[i].year >= from_year && p_books[i].year <= to_year) {
            std::cout << "\nBook #" << i + 1 << '\n';
            std::cout << "ISBN: " << p_books[i].isbn << '\n';
            std::cout << "Title: " << p_books[i].title << '\n';
            std::cout << "Author: " << p_books[i].author << '\n';
            std::cout << "Year: " << p_books[i].year << '\n';
            std::cout << "Pages: " << p_books[i].pages << '\n';
            std::cout << "Available: "
                << (p_books[i].is_available ? "yes" : "no") << '\n';
            found = true;
        }
    }

    if (!found) {
        std::cout << "Нет книг в этом диапазоне.\n";
    }

}
/**
 * Ищет книги, в названии или в имени автора которых
 * встречается подстрока, введённая пользователем.
 * Выводит все найденные книги. Если совпадений нет,
 * сообщает об этом.
 *
 * @param p_books указатель на массив книг.
 * @param n количество книг в массиве.
 */
void searchBooks(const Book* p_books, int n) {
    std::string search_text = "";
    std::cout << "Enter author or title: ";
    std::cin >> search_text;

    bool found = false;
    for (int i = 0; i < n; i++) {
        if (p_books[i].author.find(search_text) != std::string::npos ||
            p_books[i].title.find(search_text) != std::string::npos)
        {
            std::cout << "\nBook #" << i + 1 << '\n';
            std::cout << "ISBN: " << p_books[i].isbn << '\n';
            std::cout << "Title: " << p_books[i].title << '\n';
            std::cout << "Author: " << p_books[i].author << '\n';
            std::cout << "Year: " << p_books[i].year << '\n';
            std::cout << "Pages: " << p_books[i].pages << '\n';
            std::cout << "Available: "
                << (p_books[i].is_available ? "yes" : "no") << '\n';

            found = true;

        }
    }

    if (!found) {
        std::cout << "No books found.\n";
    }


}
/**
 * Сортирует массив книг по году публикации
 * от самых новых к самым старым.
 * При совпадении года сортировка выполняется
 * по имени автора в алфавитном порядке.
 *
 * @param p_books указатель на массив книг.
 * @param n количество книг в массиве.
 */
void sortBooks(Book* p_books, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (p_books[j].year < p_books[j + 1].year || (
                p_books[j].year == p_books[j + 1].year &&
                p_books[j].author > p_books[j + 1].author)
            ){
                Book temp = p_books[j];
                p_books[j] = p_books[j + 1];
                p_books[j + 1] = temp;

            }
        }
    }

}
/**
 * Выводит в консоль все книги массива с их полями:
 * номер, ISBN, название, автор, год, страницы, статус доступности.
 *
 * @param p_books указатель на массив книг.
 * @param n количество книг в массиве.
 */
void printBook(const Book* p_books, int n) {

    for (int i = 0; i < n; i++) {
        std::cout << "\nBook #" << i + 1 << '\n';
        std::cout << "ISBN: " << p_books[i].isbn << '\n';
        std::cout << "Title: " << p_books[i].title << '\n';
        std::cout << "Author: " << p_books[i].author << '\n';
        std::cout << "Year: " << p_books[i].year << '\n';
        std::cout << "Pages: " << p_books[i].pages << '\n';
        std::cout << "Available: " << (p_books[i].is_available ? "yes" : "no") << '\n';

    }

}
/**
 * Заполняет массив книг случайными данными.
 * Для каждой книги генерируются: ISBN заданного формата,
 * название и автор из заранее подготовленных списков,
 * год публикации в диапазоне [MIN_YEAR; MAX_YEAR],
 * количество страниц в диапазоне [MIN_PAGES; MAX_PAGES],
 * а также случайный статус доступности.
 *
 * @param p_books указатель на массив книг.
 * @param n количество книг в массиве.
 */
void generateBooks(Book* p_books, int n) {
    for (int i = 0; i < n; i++) {
        p_books[i].year = MIN_YEAR + rand() % (MAX_YEAR - MIN_YEAR + 1);
        p_books[i].pages = MIN_PAGES + rand() % (MAX_PAGES - MIN_PAGES + 1);
        p_books[i].is_available = rand() % 2;
        p_books[i].title = TITLES[rand() % (sizeof(TITLES) / sizeof(TITLES[0]))];
        p_books[i].author = AUTHORS[rand() % (sizeof(AUTHORS) / sizeof(AUTHORS[0]))];
        p_books[i].isbn = std::to_string(100 + rand() % 900) + '-' +
            std::to_string(1 + rand() % 9) + '-' +
            std::to_string(100 + rand() % 900) + '-' +
            std::to_string(10000 + i) + '-' +
            std::to_string(1 + rand() % 9);

    }


}

int main() {
    std::srand(time(nullptr));

    int n = 0;
    std::cout << "Enter N: ";
    std::cin >> n;

    if (n <= 0) {
        std::cout << "Invalid N.\n";
        return 0;
    }

    Book* p_books = new Book[n]{};
    generateBooks(p_books, n);

    int choice = -1;
    do {
        std::cout << "\n=== MENU ===\n";
        std::cout << "1. Show all books\n";
        std::cout << "2. Search by author or title\n";
        std::cout << "3. Filter by year\n";
        std::cout << "4. Statistics\n";
        std::cout << "5. Sort\n";
        std::cout << "6. Issue book\n";
        std::cout << "7. Return book\n";
        std::cout << "0. Exit\n";
        std::cout << "Your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "\n Catalog \n";
            printBook(p_books, n);
            break;
        case 2:
            std::cout << "\n Search \n";
            searchBooks(p_books, n);
            break;
        case 3:
            std::cout << "\nFilter by year \n";
            filterBooks(p_books, n);
            break;
        case 4:
            std::cout << "\n Statistics \n";
            printStats(p_books, n);
            break;
        case 5:
            std::cout << "\n Sort \n";
            sortBooks(p_books, n);
            printBook(p_books, n);
            break;
        case 6: {
            std::cout << "\n Issue book \n";
            std::string isbn_to_issue = "";
            std::cout << "Enter ISBN: ";
            std::cin >> isbn_to_issue;
            issueBook(p_books, n, isbn_to_issue);
            break;
        }
        case 7: {
            std::cout << "\n Return book\n";
            std::string isbn_to_return = "";
            std::cout << "Enter ISBN: ";
            std::cin >> isbn_to_return;
            returnBook(p_books, n, isbn_to_return);
            break;
        }
        case 0:
            std::cout << "Exiting.\n";
            break;
        default:
            std::cout << "Invalid menu item.\n";
            break;
        }
    } while (choice != 0);

    delete[] p_books;
    p_books = nullptr;

    return 0;
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
