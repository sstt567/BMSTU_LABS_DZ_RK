/*
#include <iostream>

using namespace std;

const char* findSubstring(const char* text, const char* word) {
    while (*text != '\0') {
        const char* t = text;
        const char* w = word;

        while (*w != '\0' && *t == *w) {
            t++;
            w++;
        }

        if (*w == '\0') return text;
        text++;
    }
    return nullptr;
}

int main() {
    char text[100];
    char word[100];

    cout << "Введите исходную строку: ";
    cin.getline(text, 100);

    cout << "Введите подстроку для поиска: ";
    cin.getline(word, 100);

    const char* result = findSubstring(text, word);

    if (result != nullptr) {
        cout << "Подстрока найдена: " << result << endl;
    }
    else {
        cout << "Подстрока не найдена" << endl;
    }

    return 0;
}
*/