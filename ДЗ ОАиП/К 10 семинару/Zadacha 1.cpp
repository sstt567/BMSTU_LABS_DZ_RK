/*
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");

    ofstream File1("input.txt");
    if (File1.is_open()) {
        File1 << "Это первая строка." << endl;
        File1 << "А это вторая." << endl;
        File1 << "Третья строка завершает файл." << endl;
        File1.close();
    }
    else {
        cout << "Ошибка: не удалось создать файл input.txt!" << endl;
        return 1;
    }

    ifstream inputFile("input.txt");

    if (!inputFile.is_open()) {
        cout << "Ошибка: файл input.txt не найден!" << endl;
        return 1;
    }

    cout << "Содержимое файла:" << endl;
    string line;

    while (getline(inputFile, line)) {
        cout << line << endl;
    }

    inputFile.close();
    return 0;
}
*/