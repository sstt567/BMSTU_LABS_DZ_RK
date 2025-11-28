/*
#include <iostream>
#include <string>

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");
    string text;
    char target;

    cout << "Введите строку: ";
    getline(cin, text);

    cout << "Введите символ для поиска: ";
    cin >> target;

    int count = 0;
    const char* ptr = text.c_str();

    while (*ptr != '\0') {
        if (*ptr == target) count++;
        ptr++;
    }

    cout << "Символ '" << target << "' встречается " << count << " раз(а)" << endl;

    return 0;
}
*/