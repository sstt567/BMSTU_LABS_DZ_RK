/*
#include <iostream>
#include<cstring>
using namespace std;

void myStrCopy(char* dest, const char* src) {
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}

int main() {
    setlocale(LC_ALL, "RU");
    char source[] = "Salam !";
    char destination[50];

    myStrCopy(destination, source);
    cout << "Скопированная строка: " << destination << endl;
    return 0;
}
*/