/*
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");

    ofstream file8_1_("file1.txt");
    file8_1_ << "Первая строка" << endl;
    file8_1_ << "Вторая строка" << endl;
    file8_1_ << "Третья строка" << endl;
    file8_1_.close();

    ofstream file2("file2.txt");
    file2 << "Первая строка" << endl;
    file2 << "Вторая строка" << endl;
    file2 << "Третья строка" << endl;
    file2.close();

    ifstream f1("file1.txt");
    ifstream f2("file2.txt");

    if (!f1.is_open()) {
        cout << "Ошибка: файл file1.txt не найден!" << endl;
        return 1;
    }

    if (!f2.is_open()) {
        cout << "Ошибка: файл file2.txt не найден!" << endl;
        return 1;
    }

    string line1, line2;
    bool identical = true;

    while (getline(f1, line1) && getline(f2, line2)) {
        if (line1 != line2) {
            identical = false;
            break;
        }
    }

 
    if (identical && (getline(f1, line1) || getline(f2, line2))) {
        identical = false;
    }

    if (identical) {
        cout << "Файлы идентичны." << endl;
    }
    else {
        cout << "Файлы различаются." << endl;
    }

    f1.close();
    f2.close();
    return 0;
}
*/