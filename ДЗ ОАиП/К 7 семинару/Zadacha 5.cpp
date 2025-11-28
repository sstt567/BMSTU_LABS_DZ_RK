/*
#include <iostream>
#include <string>

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");
    string str;

    cout << "¬ведите строку: ";
    getline(cin, str);

    char* ptr = &str[0]; 

    while (*ptr != '\0') {
        if (*ptr == ' ') {
            *ptr = '_'; 
        }
        ptr++;
    }

    cout << str;

    return 0;
}
*/