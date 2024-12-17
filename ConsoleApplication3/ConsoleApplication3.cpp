#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

// Класс книги с приватными полями и публичными методами доступа
class Book {
private:
    std::string name;     // Название книги
    std::string author;   // Автор книги
    int year;             // Год издания

public:
    // Конструктор класса Book
    Book(const std::string& name, const std::string& author, int year)
        : name(name), author(author), year(year) {}

    // Геттер для получения названия книги
    std::string getName() const { return name; }

    // Геттер для получения автора книги
    std::string getAuthor() const { return author; }

    // Геттер для получения года издания
    int getYear() const { return year; }
};

// Функтор для сортировки книг по автору (первичный ключ) и названию (вторичный ключ)
struct BookSorter {
    bool operator()(const Book* a, const Book* b) const {
        // Сначала сравниваем авторов
        if (a->getAuthor() != b->getAuthor()) {
            return a->getAuthor() < b->getAuthor();
        }
        // Если авторы одинаковы, сравниваем названия книг
        return a->getName() < b->getName();
    }
};

// Функтор для поиска книг в указанном диапазоне лет
struct BookFinder {
    int minYear;  // Минимальный год
    int maxYear;  // Максимальный год

    // Конструктор функтора с заданием диапазона лет
    BookFinder(int min, int max) : minYear(min), maxYear(max) {}

    // Перегрузка оператора() для проверки попадания в диапазон
    bool operator()(const Book* book) const {
        return book->getYear() >= minYear && book->getYear() <= maxYear;
    }
};

int main() {
    setlocale(LC_ALL, "RUSSIAN");

    // Создание вектора указателей на книги
    std::vector<Book*> books;
    books.push_back(new Book("Война и мир", "Толстой Л.Н.", 2010));
    books.push_back(new Book("Подросток", "Достоевский Ф.М.", 2004));
    books.push_back(new Book("Обрыв", "Гончаров И.А.", 2010));
    books.push_back(new Book("Анна Каренина", "Толстой Л.Н.", 1999));
    books.push_back(new Book("Обыкновенная история", "Гончаров И.А.", 2011));
    books.push_back(new Book("Утраченные иллюзии", "Бальзак О.", 2009));
    books.push_back(new Book("Оливер Твист", "Диккенс Ч.", 2001));
    books.push_back(new Book("Фауст", "Гёте И.В.", 2010));
    books.push_back(new Book("Лилия долины", "Бальзак О.", 1998));

    std::cout << "\nКниги в алфавитном порядке:\n\n";

    // Создание функтора для сортировки
    BookSorter book_sorter;

    // Сортировка книг с помощью функтора
    std::sort(books.begin(), books.end(), book_sorter);

    // Вывод отсортированных книг
    for (std::vector<Book*>::iterator i = books.begin(); i != books.end(); ++i) {
        std::cout << (*i)->getAuthor() << " \"" << (*i)->getName() << "\"" << std::endl;
    }

    // Создание функтора для поиска книг в диапазоне 2005-2014 годов
    BookFinder book_finder(2005, 2014);

    std::cout << "\nКниги в диапазоне года издания 2005 - 2014:\n\n";

    // Поиск книг с помощью find_if и функтора
    std::vector<Book*>::iterator finder = std::find_if(books.begin(), books.end(), book_finder);

    // Вывод найденных книг
    while (finder != books.end()) {
        std::cout << (*finder)->getAuthor() << " \"" << (*finder)->getName() << "\"" << std::endl;
        finder = std::find_if(++finder, books.end(), book_finder);
    }

    // Освобождение динамически выделенной памяти
    for (std::vector<Book*>::iterator i = books.begin(); i != books.end(); ++i) {
        delete (*i);
    }

    return 0;
}