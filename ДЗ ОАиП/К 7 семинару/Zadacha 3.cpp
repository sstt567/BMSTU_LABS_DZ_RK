/*
#include <iostream>
#include<cstring>
using namespace std;

void myStrCat(char* dest, const char* src) {
    while (*dest != '\0') {
        dest++;
    }
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}

int main() {
setlocale(LC_ALL, "RU");
    char str1[50] = "Privet, ";
    char str2[] = "Bratan!";

    myStrCat(str1, str2);
    cout << "Результат конкатенации: " << str1 << endl;
    return 0;
}
*/