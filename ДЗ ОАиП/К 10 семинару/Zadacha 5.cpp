/*
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");

    ofstream File5("input.txt");
    File5 << "Этот текст содержит несколько строк." << endl;
    File5 << "Вторая строка файла." << endl;
    File5 << "Третья строка для подсчета символов." << endl;
    File5.close();

    ifstream file("input.txt");

    if (!file.is_open()) {
        cout << "Ошибка: файл input.txt не найден!" << endl;
        return 1;
    }

    int charCount = 0;
    char ch;

    while (file.get(ch)) {
        charCount++;
    }

    cout << "Количество символов в файле: " << charCount << endl;

    file.close();
    return 0;
}
*/