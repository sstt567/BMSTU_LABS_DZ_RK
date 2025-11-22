#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

struct Task {
    string title;
    int priority;
    string deadline;
};

vector<Task> tasks;

void loadTasks()
{
    ifstream file("tasks.txt");
    string line;
    while (getline(file, line)) {
        int pos1 = line.find(',');
        int pos2 = line.find(',', pos1 + 1);
        if (pos1 >= 0 && pos2 >= 0) {
            tasks.push_back({ line.substr(0, pos1),
                           stoi(line.substr(pos1 + 1, pos2 - pos1 - 1)),
                           line.substr(pos2 + 1) });
        }
    }
    file.close();
}

void saveTsk() // Сохранение задач
{
    ofstream file("tasks.txt");
    for (auto& task : tasks) {
        file << task.title << "," << task.priority << "," << task.deadline << endl;
    }
    file.close();
}

void addTsk() //Добавление задачи
{
    Task task;
    cout << "Название: "; getline(cin, task.title);
    if (task.title.empty()) return;
    cout << "Приоритет: "; cin >> task.priority;
    cin.ignore(); cout << "Срок: "; getline(cin, task.deadline);

    tasks.push_back(task);
    saveTsk();

    ofstream out("output.txt", ios::app);
    out << "Добавлено: " << task.title << endl;
    cout << "Задача добавлена!\n";
}

void searchTsk() //Поиск задачи
{
    string title;
    cout << "Поиск: "; getline(cin, title);

    ofstream out("output.txt", ios::app);
    out << "Поиск: " << title << endl;

    for (auto& task : tasks) {
        if (task.title.find(title) != string::npos) {
            cout << "Найдено: " << task.title << " (приоритет: " << task.priority
                << ", срок: " << task.deadline << ")\n";
            out << "Найдено: " << task.title << endl;
        }
    }
}

void sortTsk() //Сортировка
{
    cout << "1-приоритет, 2-срок: ";
    int choice; cin >> choice; cin.ignore();

    if (choice == 1) {
        sort(tasks.begin(), tasks.end(), [](Task a, Task b) {
            return a.priority < b.priority;
            });
        cout << "Отсортировано по приоритету\n";
    }
    else if (choice == 2) {
        sort(tasks.begin(), tasks.end(), [](Task a, Task b) {
            return a.deadline < b.deadline;
            });
        cout << "Отсортировано по сроку\n";
    }

    saveTsk();
    ofstream out("output.txt", ios::app);
    out << "Сортировка выполнена" << endl;
}

void tskByPr() // По приоритету
{
    int maxPr;
    cout << "Макс.приоритет: "; cin >> maxPr; cin.ignore();

    ofstream out("output.txt", ios::app);
    out << "Задачи с приоритетом <= " << maxPr << endl;

    for (auto& task : tasks) {
        if (task.priority <= maxPr) {
            cout << task.title << " (приоритет: " << task.priority << ")\n";
            out << task.title << endl;
        }
    }
}

void showAllTks() {
    for (auto& task : tasks) {
        cout << "- " << task.title << " (приоритет: " << task.priority
            << ", срок: " << task.deadline << ")\n";
    }
}

int main() {
    setlocale(LC_ALL, "RU");
    loadTasks();
    int choice;

    do {
        cout << "\n1.Добавить 2.Найти 3.Сортировать 4.По приоритету 5.Все задачи 0.Выход\nВыбор: ";
        cin >> choice; cin.ignore();

        switch (choice) {
        case 1: addTsk(); break;
        case 2: searchTsk(); break;
        case 3: sortTsk(); break;
        case 4: tskByPr(); break;
        case 5: showAllTks(); break;
        case 0: cout << "Выход\n"; break;
        default: cout << "Неверный выбор\n";
        }
    } while (choice != 0);

    return 0;
}