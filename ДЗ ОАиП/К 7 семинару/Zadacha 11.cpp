/*
#include <iostream>
#include <vector>

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");
    int n;
    cout << "Введите количество элементов: ";
    cin >> n;

    vector<int> vec(n);

    cout << "Введите " << n << " чисел: ";
    for (int i = 0; i < n; i++) {
        cin >> vec[i];
    }

    int* p = vec.data();
    int sum = 0;

    for (int i = 0; i < n; i++) {
        sum += *(p + i);
    }

    cout << "Сумма: " << sum << endl;

    return 0;
}
*/