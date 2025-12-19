CREATE DATABASE task_management_db;

CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL
);

CREATE TABLE tasks (
    id SERIAL PRIMARY KEY,
    title VARCHAR(200) NOT NULL,
    priority VARCHAR(20) CHECK (priority IN ('Высокий', 'Средний', 'Низкий')),
    due_date DATE,
    status VARCHAR(20) CHECK (status IN ('Выполнена', 'В процессе', 'Отложена'))
);

CREATE TABLE task_assignments (
    assignment_id SERIAL PRIMARY KEY,
    task_id INTEGER REFERENCES tasks(id) ON DELETE CASCADE,
    user_id INTEGER REFERENCES users(id) ON DELETE CASCADE,
    assigned_date DATE DEFAULT CURRENT_DATE  -- ДОБАВЛЕНО
);

-- Индексы
CREATE INDEX idx_tasks_due_date ON tasks(due_date);
CREATE INDEX idx_tasks_status ON tasks(status);
CREATE INDEX idx_task_assignments_assigned_date ON task_assignments(assigned_date);