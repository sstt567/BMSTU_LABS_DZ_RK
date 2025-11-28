/*
#include <iostream>
using namespace std;

void parseTelemetry(const char* data) {
    const char* p = data;  

    while (*p != '\0') {  

        
        while (*p == ';' || *p == ' ') p++;
        if (*p == '\0') break;

       
        while (*p != ':' && *p != '\0') {
            cout << *p;
            p++;
        }
        if (*p == '\0') break;

        cout << ": ";  

     
        while (*p != ';' && *p != '\0') {
            cout << *p;
            p++;
        }

        cout << endl; 
    }
}

int main() {
    setlocale(LC_ALL, "RU");
    char data[100];
    cout << "¬ведите данные: ";
    cin.getline(data, 100);

    parseTelemetry(data);
    return 0;
}
*/