/*
#include <iostream>
#include <cstring>

using namespace std;

void reverse(char* str) {
    if (str == nullptr) return;

    char* start = str;         
    char* end = str + strlen(str) - 1; 

 
    while (start < end) {
     
        char temp = *start;
        *start = *end;
        *end = temp;

      
        start++;
        end--;
    }
}

int main() {
    setlocale(LC_ALL, "RU");
    char str[100];

    cout << "Введите строку: ";
    cin.getline(str, 100);

    cout << "Исходная строка: " << str << endl;

    reverse(str);

    cout << "Перевернутая строка: " << str << endl;

    return 0;
}
*/