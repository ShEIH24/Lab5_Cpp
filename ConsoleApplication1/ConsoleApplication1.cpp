#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <cctype>
#include <locale>

// Шаблонный класс DataManager для работы с однотипным набором данных
template <typename T, size_t MAX_SIZE = 64>
class DataManager {
private:
    T data[MAX_SIZE];          // Массив для хранения данных
    size_t current_size;        // Текущее количество элементов в наборе
    const std::string DUMP_FILE = "dump.dat"; // Имя файла для выгрузки данных

    // Метод для смещения всех элементов вправо
    void shiftRight() {
        if (current_size > 0) {
            // Перемещаем каждый элемент на одну позицию вправо
            for (size_t i = current_size; i > 0; --i) {
                data[i] = data[i - 1];
            }
        }
    }

    // Метод для определения центрального индекса
    size_t getCenterIndex() const {
        if (current_size == 0) return 0;
        // Для четного количества - первый элемент слева от центра
        return current_size % 2 == 0 ? (current_size / 2) - 1 : current_size / 2;
    }

public:
    // Конструктор - инициализация пустого набора
    DataManager() : current_size(0) {}

    // Добавление одного элемента в набор
    void push(T elem) {
        // Если набор заполнен - выгружаем данные в файл
        if (current_size >= MAX_SIZE) {
            dumpToFile();
        }

        // Смещаем существующие элементы вправо
        shiftRight();

        // Вставляем новый элемент в начало
        data[0] = elem;

        // Увеличиваем размер, если не достигнут максимум
        if (current_size < MAX_SIZE) {
            ++current_size;
        }
    }

    // Добавление группы элементов
    void push(T elems[], size_t n) {
        for (size_t i = 0; i < n; ++i) {
            push(elems[i]);
        }
    }

    // Возврат центрального элемента без извлечения
    T peek() const {
        if (current_size == 0) return T();

        // Получаем центральный индекс
        size_t center = getCenterIndex();
        return data[center];
    }

    // Извлечение центрального элемента
    T pop() {
        if (current_size == 0) return T();

        // Находим центральный индекс
        size_t center = getCenterIndex();
        T elem = data[center];

        // Смещаем элементы влево, перезаписывая центральный
        for (size_t i = center; i < current_size - 1; ++i) {
            data[i] = data[i + 1];
        }

        // Уменьшаем размер
        --current_size;

        // Если набор пуст, пробуем загрузить из файла дампа
        if (current_size == 0) {
            loadFromDumpFile();
        }

        return elem;
    }

    // Выгрузка данных в файл при заполнении
    void dumpToFile() {
        // Открываем файл для добавления в конец в бинарном режиме
        std::ofstream dump(DUMP_FILE, std::ios::app | std::ios::binary);
        dump.write(reinterpret_cast<char*>(data), current_size * sizeof(T));
        dump.close();
        current_size = 0;
    }

    // Загрузка данных из файла дампа
    void loadFromDumpFile() {
        std::ifstream dump(DUMP_FILE, std::ios::binary);
        if (dump) {
            // Определяем размер файла
            dump.seekg(0, std::ios::end);
            size_t file_size = dump.tellg() / sizeof(T);

            if (file_size > 0) {
                dump.seekg(0, std::ios::beg);
                // Считываем данные, не превышая размер массива
                size_t read_size = std::min(file_size, MAX_SIZE);
                dump.read(reinterpret_cast<char*>(data), read_size * sizeof(T));
                current_size = read_size;

                // Очищаем файл после чтения
                dump.close();
                std::ofstream truncate(DUMP_FILE, std::ios::trunc);
                truncate.close();
            }
        }
    }

    // Получение текущего размера набора
    size_t size() const { return current_size; }
};

// Явная специализация для символьного типа
template <>
class DataManager<char, 64> {
private:
    char data[64];
    size_t current_size;
    const std::string DUMP_FILE = "dump.dat";

    // Метод для смещения элементов вправо
    void shiftRight() {
        if (current_size > 0) {
            for (size_t i = current_size; i > 0; --i) {
                data[i] = data[i - 1];
            }
        }
    }

    // Получение центрального индекса
    size_t getCenterIndex() const {
        if (current_size == 0) return 0;
        return current_size % 2 == 0 ? (current_size / 2) - 1 : current_size / 2;
    }

    // Замена символов пунктуации на подчеркивание
    char sanitizePunctuation(char c) {
        return std::ispunct(static_cast<unsigned char>(c)) ? '_' : c;
    }

public:
    // Конструктор
    DataManager() : current_size(0) {}

    // Добавление одного символа с заменой пунктуации
    void push(char elem) {
        if (current_size >= 64) {
            dumpToFile();
        }

        shiftRight();

        // Санация символа - замена пунктуации
        data[0] = sanitizePunctuation(elem);

        if (current_size < 64) {
            ++current_size;
        }
    }

    // Добавление группы символов
    void push(char elems[], size_t n) {
        for (size_t i = 0; i < n; ++i) {
            push(elems[i]);
        }
    }

    // Возврат центрального символа
    char peek() const {
        if (current_size == 0) return '\0';

        size_t center = getCenterIndex();
        return data[center];
    }

    // Извлечение центрального символа
    char pop() {
        if (current_size == 0) return '\0';

        size_t center = getCenterIndex();
        char elem = data[center];

        // Смещение элементов
        for (size_t i = center; i < current_size - 1; ++i) {
            data[i] = data[i + 1];
        }

        --current_size;

        // Загрузка из файла, если набор пуст
        if (current_size == 0) {
            loadFromDumpFile();
        }

        return elem;
    }

    // Извлечение и преобразование в верхний регистр
    char popUpper() {
        char c = pop();
        return std::toupper(static_cast<unsigned char>(c));
    }

    // Извлечение и преобразование в нижний регистр
    char popLower() {
        char c = pop();
        return std::tolower(static_cast<unsigned char>(c));
    }

    // Выгрузка в файл
    void dumpToFile() {
        std::ofstream dump(DUMP_FILE, std::ios::app | std::ios::binary);
        dump.write(data, current_size);
        dump.close();
        current_size = 0;
    }

    // Загрузка из файла
    void loadFromDumpFile() {
        std::ifstream dump(DUMP_FILE, std::ios::binary);
        if (dump) {
            dump.seekg(0, std::ios::end);
            size_t file_size = dump.tellg();

            if (file_size > 0) {
                dump.seekg(0, std::ios::beg);
                size_t read_size = std::min(file_size, static_cast<size_t>(64));
                dump.read(data, read_size);
                current_size = read_size;

                // Очистка файла после чтения
                dump.close();
                std::ofstream truncate(DUMP_FILE, std::ios::trunc);
                truncate.close();
            }
        }
    }

    // Получение текущего размера
    size_t size() const { return current_size; }
};

int main() {
    // Установка русской локали для корректного вывода
    setlocale(LC_ALL, "Russian");

    // Демонстрация работы с целыми числами
    {
        std::cout << "Демонстрация DataManager для целых чисел:" << std::endl;
        DataManager<int> intManager;
        int intData[] = { 1, 2, 3, 4, 5, 6, 7 };
        intManager.push(intData, 7);

        std::cout << "Центральный элемент: " << intManager.peek() << std::endl;

        std::cout << "Элементы: ";
        std::copy(intData, intData + intManager.size(),
            std::ostream_iterator<int>(std::cout, " "));
        std::cout << std::endl;
    }

    // Демонстрация работы с числами с плавающей точкой
    {
        std::cout << "\nДемонстрация DataManager для чисел с плавающей точкой:" << std::endl;
        DataManager<double> doubleManager;
        double doubleData[] = { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7 };
        doubleManager.push(doubleData, 7);

        std::cout << "Центральный элемент: " << doubleManager.peek() << std::endl;

        std::cout << "Элементы: ";
        std::copy(doubleData, doubleData + doubleManager.size(),
            std::ostream_iterator<double>(std::cout, " "));
        std::cout << std::endl;
    }

    // Демонстрация работы с символами
    {
        std::cout << "\nДемонстрация DataManager для символов:" << std::endl;
        DataManager<char> charManager;
        char charData[] = { 'a', 'b', 'c', 'd', 'f', '?', 'e'};
        charManager.push(charData, 7);

        std::cout << "Центральный элемент: " << charManager.peek() << std::endl;

        std::cout << "Извлечение в верхнем регистре: " << charManager.popUpper() << std::endl;
        std::cout << "Извлечение в нижнем регистре: " << charManager.popLower() << std::endl;
    }

    return 0;
}