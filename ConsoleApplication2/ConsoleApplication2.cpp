#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iomanip>


class WordFrequencyCounter {
private:
    // Контейнер для хранения частоты слов
    // Ключ - слово, значение - количество повторений
    std::map<std::string, int> wordFrequency;

    // Метод для проверки, является ли символ разделителем слов
    bool isSeparator(char c) {
        return c == ' ' || c == '.' || c == ',' || c == '-' ||
            c == ':' || c == '!' || c == ';';
    }

    // Метод для очистки слова от пунктуации и приведения к нижнему регистру
    std::string cleanWord(const std::string& word) {
        std::string cleaned;
        for (char c : word) {
            if (!ispunct(c)) {
                cleaned += std::tolower(c);
            }
        }
        return cleaned;
    }

public:
    // Метод для чтения файла и подсчета частоты слов
    void processFile(const std::string& filename) {
        // Открытие файла для чтения
        std::ifstream file(filename);

        // Проверка успешности открытия файла
        if (!file.is_open()) {
            std::cerr << "Не удалось открыть файл: " << filename << std::endl;
            return;
        }

        // Буфер для чтения строк файла
        std::string line;

        // Чтение файла построчно
        while (std::getline(file, line)) {
            // Буфер для накопления текущего слова
            std::string word;

            // Проход по каждому символу в строке
            for (char c : line) {
                // Если встретили разделитель
                if (isSeparator(c)) {
                    // Если слово не пустое
                    if (!word.empty()) {
                        // Очистка слова от пунктуации
                        std::string cleanedWord = cleanWord(word);

                        // Учитываем только слова длиннее 3 символов
                        if (cleanedWord.length() > 3) {
                            // Увеличиваем счетчик частоты слова
                            wordFrequency[cleanedWord]++;
                        }

                        // Очистка буфера слова
                        word.clear();
                    }
                }
                else {
                    // Накопление символов в текущее слово
                    word += c;
                }
            }

            // Обработка последнего слова в строке
            if (!word.empty()) {
                std::string cleanedWord = cleanWord(word);
                if (cleanedWord.length() > 3) {
                    wordFrequency[cleanedWord]++;
                }
            }
        }
    }

    // Метод для вывода слов, встречающихся не менее 7 раз
    void printFrequentWords() {
        // Создаем вектор пар для сортировки по частоте
        std::vector<std::pair<std::string, int>> sortedWords;

        // Заполнение вектора словами, встречающимися не менее 7 раз
        for (const auto& pair : wordFrequency) {
            if (pair.second >= 7) {
                sortedWords.push_back(pair);
            }
        }

        // Сортировка вектора по убыванию частоты
        std::sort(sortedWords.begin(), sortedWords.end(),
            [](const auto& a, const auto& b) {
                return a.second > b.second;
            });

        // Вывод отсортированных слов и их частоты в требуемом формате
        for (const auto& pair : sortedWords) {
            // Выравнивание по левому краю для слова и по правому для числа
            std::cout << std::left << std::setw(10) << pair.first
                << std::right << std::setw(5) << pair.second
                << std::endl;
        }
    }
};

// Главная функция - точка входа в программу
int main() {
    setlocale(LC_ALL, "Russian");
    // Создание экземпляра класса для подсчета частоты слов
    WordFrequencyCounter counter;

    // Обработка текстового файла
    counter.processFile("input.txt");

    // Вывод слов, встречающихся не менее 7 раз
    counter.printFrequentWords();

    return 0;
}