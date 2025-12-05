#include <iostream>
#include <fstream>

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");

    ofstream File11("vibration_log.txt");
    File11 << "0.02" << endl;
    File11 << "0.05" << endl;
    File11 << "0.03" << endl;
    File11 << "0.10" << endl;
    File11 << "0.09" << endl;
    File11 << "0.07" << endl;
    File11 << "0.15" << endl;
    File11 << "0.04" << endl;
    File11.close();

    ifstream input("vibration_log.txt");

    if (!input.is_open()) {
        cout << "Ошибка: файл vibration_log.txt не найден!" << endl;
        return 1;
    }

    double value;
    int count = 0;
    double sum = 0;
    double min_val = 1000;
    double max_val = -1000;

    while (input >> value) {
        count++;
        sum += value;
        if (value < min_val) min_val = value;
        if (value > max_val) max_val = value;
    }

    input.close();

  
    ifstream input2("vibration_log.txt");

    ofstream report("vibration_report.txt");

    report << "Количество измерений: " << count << endl;
    report << "Фильтрованные значения (>0.05): ";

    while (input2 >> value) {
        if (value > 0.05) {
            report << value << " ";
        }
    }

    double average = (count > 0) ? sum / count : 0;
    report << endl << "Среднее значение: " << average << endl;
    report << "Минимум: " << min_val << endl;
    report << "Максимум: " << max_val << endl;

    input2.close();
    report.close();

    cout << "Анализ вибрационных данных завершён. Отчёт сохранён в vibration_report.txt" << endl;

    return 0;
}