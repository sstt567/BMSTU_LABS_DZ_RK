/*
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");

    ofstream outputFile("output.txt");

    if (!outputFile.is_open()) {
        cout << "Ошибка: не удалось создать файл output.txt!" << endl;
        return 1;
    }

    cout << "Введите строки (пустая строка завершает ввод):" << endl;
    string line;

    while (true) {
        getline(cin, line);
        if (line.empty()) {
            break;
        }
        outputFile << line << endl;
    }

    cout << "Данные записаны в файл output.txt." << endl;

    outputFile.close();
    return 0;
}
*/