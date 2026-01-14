# Система интернет‑магазина (C++17 + PostgreSQL)

## 3.1 Описание задачи

**Цель работы** — разработать систему управления интернет‑магазином на C++ с хранением данных в PostgreSQL и доступом через библиотеку libpqxx. [file:94]

Реализована консольная система интернет‑магазина с ролями **Администратор**, **Менеджер**, **Покупатель**: управление товарами, создание и обработка заказов, изменение статусов, возврат заказа (в течение 30 дней), аудит действий и экспорт отчёта в CSV. [file:94]

Используемые технологии:
- C++17
- PostgreSQL
- libpqxx [file:94][web:103]

## 3.2 Архитектура проекта

### Классы и взаимосвязи
- `DatabaseConnection<T>` — шаблонный класс для подключения к PostgreSQL и выполнения SQL команд. [file:94]
- `User` — абстрактный базовый класс пользователей (виртуальный метод `menu`). [file:94]
  - `Admin` — управление товарами, заказами, аудитом, экспорт CSV. [file:94]
  - `Manager` — работа с заказами в ожидании, подтверждение/изменение статуса, обновление остатков. [file:94]
  - `Customer` — создание заказа, добавление товаров, оплата, просмотр статуса, возврат (≤ 30 дней). [file:94]
- `Payment` — абстрактный класс стратегии оплаты (полиморфизм). [file:94]
  - `CardPayment`, `WalletPayment`, `SBPPayment` — конкретные варианты оплаты. [file:94]
- `ReportGenerator` — экспорт данных в CSV (отчёт). [file:94]
- `OrderService` — сервисный класс, выполняющий операции с заказами через БД. [file:94]

### Применение ООП
- Наследование: `User -> Admin/Manager/Customer`. [file:94]
- Полиморфизм: `Payment` (виртуальный интерфейс) и конкретные виды оплаты. [file:94]
- Композиция: оплата в процессе оформления заказа реализуется через `std::unique_ptr<Payment>` (владение стратегией оплаты). [file:94]
- Агрегация: логика работы ролей агрегирует доступные операции (меню) и обращается к сервисам БД. [file:94]

### Шаблонный класс DatabaseConnection
Подключение к БД создаётся как:
```cpp
DatabaseConnection<std::string> db("host=localhost user=postgres password=password dbname=ekz_task");
```
Класс предоставляет методы `executeQuery()` и `executeNonQuery()` для выполнения SQL команд. [file:94][web:104]

## 3.3 Работа с базой данных

### Структура БД
Используется PostgreSQL база данных `ekz_task`. [file:94]

Список таблиц (6 шт.): [file:94]
- `users` — пользователи (роль: admin/manager/customer, флаг лояльности). [file:94]
- `products` — товары (цена, остаток, ограничения CHECK). [file:94]
- `orders` — заказы (статус: pending/completed/canceled/returned, сумма, дата). [file:94]
- `order_items` — позиции заказа (товар, количество, цена). [file:94]
- `order_status_history` — история изменения статусов. [file:94]
- `audit_log` — аудит операций (insert/update/delete). [file:94]

### Функции, процедуры, триггеры
Функции (примерный набор):
- `calculate_order_total(order_id)` — расчёт суммы заказа по позициям. [file:94]
- Дополнительные функции для получения данных (роль пользователя, скидка, наличие товара и т.п.). [file:94]

Хранимые процедуры/функции (ключевые):
- `create_order_with_transaction(...)` — создание заказа с проверками и транзакционной логикой. [file:94]
- `update_order_status(order_id, new_status, changed_by)` — смена статуса с записью в `order_status_history`. [file:94]
- `process_return_order(order_id, performed_by)` — возврат заказа (ограничение: ≤ 30 дней). [file:94]

Триггеры (3 шт. минимум):
- Логирование вставки товара (`products` INSERT → `audit_log`). [file:94]
- Логирование обновления товара (`products` UPDATE → `audit_log`). [file:94]
- Логирование изменения заказа (`orders` UPDATE → `audit_log`). [file:94]

### Транзакции и откат
Критичные операции (создание заказа, изменение статуса, возврат) выполняются внутри транзакций PostgreSQL (plpgsql) и при ошибке приводят к откату. [file:94]

## 3.4 Умные указатели и STL

### Где используются `std::unique_ptr` и `std::shared_ptr`
- `std::unique_ptr<Payment>` — владение стратегией оплаты (полиморфизм и безопасное управление памятью без `new/delete`). [file:94]
- `std::unique_ptr<User>` — выбор текущей роли в меню (без ручного освобождения). [file:94]

### Примеры STL алгоритмов и лямбд
В демонстрационном пункте меню покупателя используются: [file:94]
- `std::find_if` — поиск первого заказа (суммы) больше заданного порога.
- `std::copy_if` — выборка заказов по условию.
- `std::accumulate` — суммарные траты пользователя.

Фрагмент:
```cpp
auto it = std::find_if(totals.begin(), totals.end(), [](double v){ return v > 100.0; });
std::vector<double> big;
std::copy_if(totals.begin(), totals.end(), std::back_inserter(big), [](double v){ return v > 100.0; });
double sum = std::accumulate(totals.begin(), totals.end(), 0.0);
```

## 3.5 Логика ролей и прав доступа

- Администратор:
  - Добавление/изменение/удаление товаров.
  - Просмотр всех заказов.
  - Изменение статусов заказов.
  - Просмотр `audit_log`.
  - Экспорт отчёта CSV. [file:94]

- Менеджер:
  - Просмотр заказов в статусе `pending`.
  - Подтверждение заказа (перевод в `completed`).
  - Обновление остатков товаров. [file:94]

- Покупатель:
  - Создание заказа.
  - Добавление товара в заказ.
  - Оплата заказа.
  - Просмотр своих заказов и их статусов.
  - Возврат заказа (≤ 30 дней). [file:94]

Ограничения доступа:
- `audit_log` и все заказы доступны только администратору. [file:94]
- Возврат возможен только для своих заказов, проверяется `user_id` заказа. [file:94]

## 3.6 Аудит и история изменений

- `order_status_history` хранит переходы статусов: `old_status`, `new_status`, `changed_at`, `changed_by`. [file:94]
- `audit_log` хранит операции над сущностями: `entity_type`, `entity_id`, `operation`, `performed_by`, `performed_at`. [file:94]

Автоматическое логирование реализовано триггерами PostgreSQL (при INSERT/UPDATE), а смена статуса также фиксируется через процедуру `update_order_status`. [file:94]

Пример выборки последних записей аудита:
```sql
SELECT log_id, entity_type, entity_id, operation, performed_by, performed_at
FROM audit_log
ORDER BY log_id DESC
LIMIT 10;
```

## 3.7 Отчёт в формате CSV

Отчёт: **«История изменений заказов и действий пользователей»** формируется в файл `reports/audit_report.csv`. [file:94]

SQL-запрос для отчёта (пример):
```sql
SELECT o.order_id,
       u.name AS user_name,
       o.status,
       o.total_price,
       o.order_date
FROM orders o
JOIN users u ON u.user_id=o.user_id
ORDER BY o.order_id;
```

Пример содержимого CSV:
```csv
order_id,user,status,total,order_date
1,Customer,completed,129.99,2026-01-14 20:10:00
2,Premium,pending,29.99,2026-01-14 20:20:00
```

## 3.8 Сборка и запуск проекта

### Требования к окружению
- Компилятор: g++ с поддержкой C++17. [file:94]
- PostgreSQL 11+ (или совместимый). [file:94]
- libpqxx (C++ интерфейс PostgreSQL). [file:94][web:103]

### Инструкции по сборке
Makefile:
```bash
make
```

Подготовка БД (создание таблиц/функций/процедур/триггеров и тестовых данных):
```bash
make setup_db
```

Запуск:
```bash
make run
```

### Пример запуска
```text
========== ОНЛАЙН МАГАЗИН ==========
1) Admin
2) Manager
3) Customer
0) Exit
>
```

### Примеры работы меню
Admin:
```text
ADMIN: 1)add product 2)all orders 3)set status 4)audit 5)csv 0)back
>
```

Manager:
```text
MANAGER: 1)pending 2)approve 3)stock 0)back
>
```

Customer:
```text
CUSTOMER: 1)new 2)add 3)my 4)status 5)pay 6)return 7)stl 0)back
>
```

### Пример логов и истории изменений
История статусов:
```sql
SELECT order_id, old_status, new_status, changed_at, changed_by
FROM order_status_history
ORDER BY history_id DESC
LIMIT 10;
```

Логи аудита:
```sql
SELECT log_id, entity_type, entity_id, operation, performed_by, performed_at
FROM audit_log
ORDER BY log_id DESC
LIMIT 10;
```
