/*
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");

    ofstream File7("numbers.txt");
    File7 << "10 20 30 40 50 60" << endl;
    File7 << "5 15 25 35" << endl;
    File7.close();


    ifstream file("numbers.txt");

    if (!file.is_open()) {
        cout << "Ошибка: файл numbers.txt не найден!" << endl;
        return 1;
    }

    int number;
    int count = 0;
    int sum = 0;

    while (file >> number) {
        sum += number;
        count++;
    }

    cout << "Количество чисел: " << count << endl;
    cout << "Сумма: " << sum << endl;

    if (count > 0) {
        double average = static_cast<double>(sum) / count;
        cout << "Среднее: " << average << endl;
    }
    else {
        cout << "Среднее: 0 (нет чисел)" << endl;
    }

    file.close();
    return 0;
}
*/