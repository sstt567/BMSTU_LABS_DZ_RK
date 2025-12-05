/*
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");

    ofstream File9("temperature_data.txt");
    File9 << "Sensor1 22.5" << endl;
    File9 << "Sensor2 21.8" << endl;
    File9 << "Sensor3 23.1" << endl;
    File9 << "Sensor1 22.7" << endl;
    File9 << "Sensor2 22.0" << endl;
    File9 << "Sensor3 23.3" << endl;
    File9.close();

    ifstream input("temperature_data.txt");

    if (!input.is_open()) {
        cout << "Ошибка: файл temperature_data.txt не найден!" << endl;
        return 1;
    }

    double s1_min = 1000, s1_max = -1000, s1_sum = 0;
    double s2_min = 1000, s2_max = -1000, s2_sum = 0;
    double s3_min = 1000, s3_max = -1000, s3_sum = 0;
    int c1 = 0, c2 = 0, c3 = 0;

    string sensor;
    double temp;

    while (input >> sensor >> temp) {
        if (sensor == "Sensor1") {
            if (temp < s1_min) s1_min = temp;
            if (temp > s1_max) s1_max = temp;
            s1_sum += temp;
            c1++;
        }
        else if (sensor == "Sensor2") {
            if (temp < s2_min) s2_min = temp;
            if (temp > s2_max) s2_max = temp;
            s2_sum += temp;
            c2++;
        }
        else if (sensor == "Sensor3") {
            if (temp < s3_min) s3_min = temp;
            if (temp > s3_max) s3_max = temp;
            s3_sum += temp;
            c3++;
        }
    }

    input.close();


    ofstream report("report.txt");

    report << "Sensor1: min = " << s1_min << ", max = " << s1_max
        << ", avg = " << (c1 > 0 ? s1_sum / c1 : 0) << endl;

    report << "Sensor2: min = " << s2_min << ", max = " << s2_max
        << ", avg = " << (c2 > 0 ? s2_sum / c2 : 0) << endl;

    report << "Sensor3: min = " << s3_min << ", max = " << s3_max
        << ", avg = " << (c3 > 0 ? s3_sum / c3 : 0) << endl;

    report.close();
    cout << "Отчёт о температурных данных сохранён в report.txt" << endl;

    return 0;
}
*/