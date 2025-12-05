/*
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");

    ofstream File3("text.txt");
    File3 << "Привет" << endl;
    File3 << "Как дела?" << endl;
    File3 << "Что делаешь?" << endl;
    File3.close();

    ifstream file("text.txt");

    if (!file.is_open()) {
        cout << "Ошибка: файл text.txt не найден!" << endl;
        return 1;
    }

    int wordCount = 0;
    string word;

    while (file >> word) {
        wordCount++;
    }

    cout << "Количество слов в файле: " << wordCount << endl;

    file.close();
    return 0;
}
*/