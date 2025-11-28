/*
#include <iostream>
#include <vector>

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");
    vector<int> vec = { 10, 20, 30, 40, 50 };

    cout << "Исходный вектор: ";
    for (int num : vec) cout << num << " ";
    cout << endl;

    int* p = vec.data();

    for (size_t i = 0; i < vec.size(); i++) {
        p[i] += 10;  
    }

    cout << "После увеличения: ";
    for (int num : vec) cout << num << " ";
    cout << endl;

    return 0;
}
*/