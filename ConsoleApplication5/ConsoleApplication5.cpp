#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>

// Основной шаблонный класс кэша
template <typename T>
class Cache {
private:
    std::vector<T> data; // Контейнер для хранения элементов

public:
    // Метод добавления элемента в кэш
    void put(T elem) {
        // Добавляем элемент, если его еще нет в кэше
        if (!contains(elem)) {
            data.push_back(elem);
        }
    }

    // Перегрузка оператора += для добавления элемента
    Cache& operator+=(const T& elem) {
        put(elem);
        return *this;
    }

    // Метод проверки наличия элемента в кэше
    bool contains(T elem) const {
        // Используем std::find для поиска элемента
        return std::find(data.begin(), data.end(), elem) != data.end();
    }
};

// Явная специализация для std::string
template <>
class Cache<std::string> {
private:
    std::vector<std::string> data; // Контейнер для строк

public:
    // Специализированный метод добавления строки
    void put(const std::string& elem) {
        // Если в кэше уже 100 строк, генерируем исключение
        if (data.size() >= 100) {
            throw std::runtime_error("Максимальное количество строк в кэше достигнуто");
        }
        data.push_back(elem);
    }

    // Перегрузка оператора += 
    Cache& operator+=(const std::string& elem) {
        put(elem);
        return *this;
    }

    // Специализированный метод проверки наличия строки
    bool contains(const std::string& elem) const {
        // Проверяем только первый символ строки
        for (const auto& str : data) {
            if (!str.empty() && !elem.empty() && str[0] == elem[0]) {
                return true;
            }
        }
        return false;
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    // Создаем кэш для целых чисел
    Cache<int> cache;
    cache.put(1);       // Добавление элемента через put()
    cache.put(2);
    cache += 5;         // Добавление элемента через оператор +=

    // Создаем кэш для строк
    Cache<std::string> voc;
    voc.put("OK");

    // Демонстрация работы contains() для разных типов
    std::cout << "Проверка наличия для cache: " << std::endl;
    std::cout << "Contains(5): " << cache.contains(5) << std::endl;   // Должно вывести 1
    std::cout << "Contains(10): " << cache.contains(10) << std::endl; // Должно вывести 0

    std::cout << "\nПроверка наличия для voc: " << std::endl;
    std::cout << "Contains(\"Only\"): " << voc.contains("Only") << std::endl; // Должно вывести 1
    std::cout << "Contains(\"Hello\"): " << voc.contains("Hello") << std::endl; // Должно вывести 0

    return 0;
}