/*
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");

    ofstream sourceFile("source.txt");
    sourceFile << "Первая строка для копирования" << endl;
    sourceFile << "Вторая строка с данными" << endl;
    sourceFile << "Третья строка исходного файла" << endl;
    sourceFile << "Четвертая строка" << endl;
    sourceFile.close();

    ifstream source("source.txt");
    ofstream dest("copy.txt");

    if (!source.is_open()) {
        cout << "Ошибка: файл source.txt не найден!" << endl;
        return 1;
    }

    if (!dest.is_open()) {
        cout << "Ошибка: не удалось создать файл copy.txt!" << endl;
        return 1;
    }

    int lineCount = 0;
    string line;

    while (getline(source, line)) {
        dest << line << endl;
        lineCount++;
    }

    cout << "Файл успешно скопирован." << endl;
    cout << "Количество скопированных строк: " << lineCount << endl;

    source.close();
    dest.close();
    return 0;
}
*/