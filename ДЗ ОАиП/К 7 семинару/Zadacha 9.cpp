/* 
#include <iostream>

using namespace std;

void removeSpaces(char* str) {
    char* dest = str;  

    while (*str != '\0') {
        if (*str != ' ') {
            *dest = *str; 
            dest++;
        }
        str++;  
    }

    *dest = '\0';  
}

int main() {
 setlocale(LC_ALL, "RU");
    char str[100];

    cout << "Введите строку: ";
    cin.getline(str, 100);

    cout << "Исходная строка: " << str << endl;

    removeSpaces(str);

    cout << "Без пробелов: " << str << endl;

    return 0;
}
*/