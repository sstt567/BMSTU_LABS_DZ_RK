/*
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");

    ifstream file("data.txt");

    if (!file.is_open()) {
        cout << "Ошибка: файл data.txt не найден!" << endl;
        return 1;
    }

    int lineCount = 0;
    string line;

    while (getline(file, line)) {
        lineCount++;
    }

    cout << "Количество строк в файле: " << lineCount << endl;

    file.close();
    return 0;
}
*/