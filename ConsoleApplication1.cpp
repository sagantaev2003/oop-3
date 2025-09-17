#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <functional> // std::hash

class HashMap {
private:
    struct Entry {
        std::string key;
        std::string value;
    };

    std::vector<std::list<Entry>> table; 
    size_t capacity; 
    size_t size;     
    const float loadFactor = 0.75f;

    // хеш-функция
    size_t getIndex(const std::string& key) const {
        return std::hash<std::string>{}(key) % capacity;
    }

    // проверка на необходимость расширения
    void checkLoadFactor() {
        if ((float)size / capacity > loadFactor) {
            rehash(capacity * 2);
        }
    }

    // перехеширование
    void rehash(size_t newCapacity) {
        std::vector<std::list<Entry>> newTable(newCapacity);

        for (auto& bucket : table) {
            for (auto& entry : bucket) {
                size_t newIndex = std::hash<std::string>{}(entry.key) % newCapacity;
                newTable[newIndex].push_back(entry);
            }
        }

        table = std::move(newTable);
        capacity = newCapacity;
    }

public:
    HashMap(size_t initCapacity = 7) : capacity(initCapacity), size(0) {
        table.resize(capacity);
    }

    // вставка или обновление
    void put(const std::string& key, const std::string& value) {
        size_t index = getIndex(key);
        for (auto& entry : table[index]) {
            if (entry.key == key) {
                entry.value = value; 
                return;
            }
        }
        table[index].push_back({ key, value });
        size++;
        checkLoadFactor();
    }

    // поиск
    std::string get(const std::string& key) const {
        size_t index = getIndex(key);
        for (const auto& entry : table[index]) {
            if (entry.key == key) {
                return entry.value;
            }
        }
        return " Не найдено";
    }

    // удаление
    void remove(const std::string& key) {
        size_t index = getIndex(key);
        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (it->key == key) {
                table[index].erase(it);
                size--;
                std::cout << " Удалено\n";
                return;
            }
        }
        std::cout << " Ключ не найден\n";
    }

    // печать для отладки
    void print() const {
        std::cout << "=== HashMap ===\n";
        for (size_t i = 0; i < table.size(); i++) {
            std::cout << "[" << i << "]: ";
            for (const auto& entry : table[i]) {
                std::cout << "(" << entry.key << " -> " << entry.value << ") ";
            }
            std::cout << "\n";
        }
    }
};

// меню
int main() {
    setlocale(LC_ALL, "Russian");
    HashMap map;

    while (true) {
        std::cout << "\n--- Меню ---\n";
        std::cout << "1. Добавить лицензию\n";
        std::cout << "2. Найти лицензию\n";
        std::cout << "3. Удалить лицензию\n";
        std::cout << "4. Показать таблицу\n";
        std::cout << "0. Выход\n";
        std::cout << "Выбор: ";

        int choice;
        std::cin >> choice;

        if (choice == 0) break;

        std::string key, value;
        switch (choice) {
        case 1:
            std::cout << "Введите серийный номер: ";
            std::cin >> key;
            std::cout << "Введите описание: ";
            std::cin.ignore();
            std::getline(std::cin, value);
            map.put(key, value);
            break;
        case 2:
            std::cout << "Введите серийный номер: ";
            std::cin >> key;
            std::cout << "Результат: " << map.get(key) << "\n";
            break;
        case 3:
            std::cout << "Введите серийный номер: ";
            std::cin >> key;
            map.remove(key);
            break;
        case 4:
            map.print();
            break;
        default:
            std::cout << "Неверный ввод\n";
        }
    }

    return 0;
}
