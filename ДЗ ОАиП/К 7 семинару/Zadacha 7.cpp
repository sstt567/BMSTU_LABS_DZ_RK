/*
#include <iostream>
#include <cstring>

using namespace std;

bool isPalindrome(const char* str) {
    const char* start = str;                    
    const char* end = str + strlen(str) - 1;    

    while (start < end) {
        if (*start != *end) {                   
            return false;                       
        }
        start++;
        end--;
    }
    return true;                                
}

int main() {
    setlocale(LC_ALL, "RU");
    char str[100];

    cout << "Введите строку: ";
    cin.getline(str, 100);

    if (isPalindrome(str)) {
        cout << "Это палиндром!" << endl;
    }
    else {
        cout << "Это не палиндром" << endl;
    }

    return 0;
}
*/