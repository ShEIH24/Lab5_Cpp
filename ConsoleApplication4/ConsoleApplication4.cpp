#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <functional> // для std::greater и std::bind2nd

class Book {
private:
    std::string name;     // Название книги
    std::string author;   // Автор книги
    int year;             // Год издания

public:
    Book(const std::string& name, const std::string& author, int year)
        : name(name), author(author), year(year) {}

    std::string getName() const { return name; }
    std::string getAuthor() const { return author; }
    int getYear() const { return year; }
};

// Функтор для поиска книг новее указанного года
struct BookYearChecker {
    bool operator()(const Book* book) const {
        return book->getYear() > 2009;
    }
};

int main() {
    setlocale(LC_ALL, "RUSSIAN");

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

    // Подсчет книг новее 2009 года с использованием std::count_if
    int newBooksCount = std::count_if(books.begin(), books.end(), BookYearChecker());

    std::cout << "Количество книг новее 2009 года: " << newBooksCount << std::endl;

    // Дополнительный вариант с использованием std::greater и std::bind
    int newBooksCountAlt = std::count_if(
        books.begin(),
        books.end(),
        std::bind(std::greater<int>(),
            std::bind(&Book::getYear, std::placeholders::_1),
            2009)
    );

    std::cout << "Количество книг новее 2009 года (альтернативный метод): " << newBooksCountAlt << std::endl;

    // Вывод списка книг новее 2009 года
    std::cout << "\nСписок книг новее 2009 года:\n";
    for (const auto& book : books) {
        if (book->getYear() > 2009) {
            std::cout << book->getAuthor() << " \"" << book->getName() << "\" ("
                << book->getYear() << " год)" << std::endl;
        }
    }

    // Освобождение памяти
    for (auto book : books) {
        delete book;
    }

    return 0;
}