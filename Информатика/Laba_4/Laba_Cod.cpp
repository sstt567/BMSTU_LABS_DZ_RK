#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <pqxx/pqxx>

using namespace std;

const string CONNECTION_STR =
"dbname=task_management_db user=postgres password=StasPost2007 host=localhost port=5432";

//ЛОГИРОВАНИЕ
void logOp(const string& msg) {
    ofstream log("log.txt", ios::app);
    time_t now = time(nullptr);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
    log << "[" << buf << "] " << msg << "\n";
}

//БАЗОВЫЙ КЛАСС
class Task {
protected:
    int id;
    string title;
    string priority;
    string dueDate;
    string status;

public:
    Task(int i, const string& t, const string& p, const string& d, const string& s)
        : id(i), title(t), priority(p), dueDate(d), status(s) {
    }

    virtual ~Task() = default;
    virtual void print() const {
        cout << "ID: " << id << " | " << title << " | " << priority
            << " | " << dueDate << " | " << status << "\n";
    }
};

//полиморфизм
class CompletedTask : public Task { public: using Task::Task; };
class InProgressTask : public Task { public: using Task::Task; };
class DeferredTask : public Task { public: using Task::Task; };

// КЛАСС PAIR
template <typename T1, typename T2>
class Pair {
public:
    T1 first;
    T2 second;
    Pair(const T1& f, const T2& s) : first(f), second(s) {}
};


class User {
public:
    int id;
    string name;
    User(int i, const string& n) : id(i), name(n) {}
};

class TaskAssignment {
public:
    int assignmentId, taskId, userId;
    TaskAssignment(int a, int t, int u) : assignmentId(a), taskId(t), userId(u) {}
};


pqxx::connection connectToDB() {
    return pqxx::connection(CONNECTION_STR);
}



void addTask() {
    string title, priority, dueDate, status;
    cout << "Название: "; getline(cin >> ws, title);
    cout << "Приоритет (Высокий/Средний/Низкий): "; getline(cin, priority);
    cout << "Срок (YYYY-MM-DD): "; getline(cin, dueDate);
    cout << "Статус (Выполнена/В процессе/Отложена): "; getline(cin, status);

    try {
        pqxx::connection conn = connectToDB();
        pqxx::work txn(conn);
        txn.exec("INSERT INTO tasks (title, priority, due_date, status) VALUES (" +
            txn.quote(title) + "," + txn.quote(priority) + "," +
            txn.quote(dueDate) + "," + txn.quote(status) + ")");
        txn.commit();
        cout << "OK\n";
        logOp("Добавлена задача: " + title);
    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << "\n";
    }
}

void addUser() {
    string name;
    cout << "Имя пользователя: "; getline(cin >> ws, name);

    try {
        pqxx::connection conn = connectToDB();
        pqxx::work txn(conn);
        txn.exec("INSERT INTO users (name) VALUES (" + txn.quote(name) + ")");
        txn.commit();
        cout << "OK\n";
        logOp("Добавлен пользователь: " + name);
    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << "\n";
    }
}

void assignTask() {
    int taskId, userId;
    cout << "ID задачи: "; cin >> taskId;
    cout << "ID пользователя: "; cin >> userId;

    try {
        pqxx::connection conn = connectToDB();
        pqxx::work txn(conn);
        txn.exec("INSERT INTO task_assignments (task_id, user_id) VALUES (" +
            txn.quote(taskId) + "," + txn.quote(userId) + ")");
        txn.commit();
        cout << "OK\n";
        logOp("Назначена задача " + to_string(taskId));
    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << "\n";
    }
}

void updateStatus() {
    int taskId;
    string status;
    cout << "ID задачи: "; cin >> taskId;
    cout << "Новый статус: "; getline(cin >> ws, status);

    try {
        pqxx::connection conn = connectToDB();
        pqxx::work txn(conn);
        txn.exec("UPDATE tasks SET status = " + txn.quote(status) +
            " WHERE id = " + txn.quote(taskId));
        txn.commit();
        cout << "OK\n";
        logOp("Изменён статус задачи " + to_string(taskId));
    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << "\n";
    }
}

//АНАЛИТИЧЕСКИЕ ЗАПРОСЫ

void query1() {
    try {
        pqxx::connection conn = connectToDB();
        pqxx::work txn(conn);
        pqxx::result res = txn.exec(
            "SELECT id, title, priority, due_date, status FROM tasks WHERE priority = 'Высокий'");

        cout << "\n--- Задачи с высоким приоритетом ---\n";
        for (const auto& row : res) {
            Task t(row[0].as<int>(), row[1].c_str(), row[2].c_str(),
                row[3].is_null() ? "-" : row[3].c_str(), row[4].c_str());
            t.print();
        }
        logOp("Запрос 1: высокий приоритет");
    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << "\n";
    }
}

void query2() {
    try {
        pqxx::connection conn = connectToDB();
        pqxx::work txn(conn);
        pqxx::result res = txn.exec(
            "SELECT COUNT(DISTINCT user_id) FROM task_assignments "
            "WHERE assigned_date >= CURRENT_DATE - INTERVAL '30 days'");

        cout << "\n--- Пользователи за 30 дней ---\n";
        cout << "Количество: " << res[0][0].as<int>() << "\n";
        logOp("Запрос 2: пользователи за 30 дней");
    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << "\n";
    }
}

void query3() {
    try {
        pqxx::connection conn = connectToDB();
        pqxx::work txn(conn);
        pqxx::result res = txn.exec(
            "SELECT id, title, priority, due_date, status FROM tasks "
            "WHERE due_date BETWEEN CURRENT_DATE AND CURRENT_DATE + INTERVAL '7 days'");

        cout << "\n--- Задачи на следующую неделю ---\n";
        for (const auto& row : res) {
            Task t(row[0].as<int>(), row[1].c_str(), row[2].c_str(),
                row[3].is_null() ? "-" : row[3].c_str(), row[4].c_str());
            t.print();
        }
        logOp("Запрос 3: задачи на неделю");
    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << "\n";
    }
}

void query4() {
    try {
        pqxx::connection conn = connectToDB();
        pqxx::work txn(conn);
        pqxx::result res = txn.exec(
            "SELECT users.name, COUNT(*) AS cnt FROM users "
            "JOIN task_assignments ON users.id = task_assignments.user_id "
            "JOIN tasks ON tasks.id = task_assignments.task_id "
            "WHERE tasks.status = 'Выполнена' "
            "GROUP BY users.name ORDER BY cnt DESC LIMIT 3");

        cout << "\n--- Топ-3 пользователей ---\n";
        for (const auto& row : res) {
            cout << row[0].c_str() << " - " << row[1].as<int>() << " задач\n";
        }
        logOp("Запрос 4: топ-3 пользователей");
    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << "\n";
    }
}



void menu() {
    cout << "\n=== МЕНЮ ===\n";
    cout << "1. Добавить задачу\n";
    cout << "2. Добавить пользователя\n";
    cout << "3. Назначить задачу\n";
    cout << "4. Изменить статус\n";
    cout << "5. Запрос 1 (Высокий приоритет)\n";
    cout << "6. Запрос 2 (За 30 дней)\n";
    cout << "7. Запрос 3 (На неделю)\n";
    cout << "8. Запрос 4 (Топ-3)\n";
    cout << "0. Выход\n";
    cout << "Выбор: ";
}

//ГЛАВНАЯ ФУНКЦИЯ

int main() {
    setlocale(LC_ALL, "Russian");

    while (true) {
        menu();
        int choice;
        cin >> choice;

        switch (choice) {
        case 1: addTask(); break;
        case 2: addUser(); break;
        case 3: assignTask(); break;
        case 4: updateStatus(); break;
        case 5: query1(); break;
        case 6: query2(); break;
        case 7: query3(); break;
        case 8: query4(); break;
        case 0: cout << "До свидания!\n"; return 0;
        default: cout << "Неверный выбор.\n";
        }
    }

    return 0;
}
