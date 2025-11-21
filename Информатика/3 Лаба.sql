--Задание 2--
--2.1--
CREATE TABLE users (
    id INT PRIMARY KEY,
    name VARCHAR(100)
);
--2.2--
CREATE TABLE tasks (
    id INT PRIMARY KEY,
    title VARCHAR(200),
    priority VARCHAR(20) CHECK (priority IN ('Высокий', 'Средний', 'Низкий')),
    due_date DATE,
    status VARCHAR(20) CHECK (status IN ('Выполнена', 'В процессе', 'Отложена'))
);
--2.3--
CREATE TABLE task_assignments (
    assignment_id INT PRIMARY KEY,
    task_id INT,
    user_id INT
);

--Задание 3--  -- ← ПЕРЕМЕСТИТЬ РАНЬШЕ ВСТАВКИ ДАННЫХ
--3.1--
ALTER TABLE task_assignments 
ADD FOREIGN KEY (task_id) REFERENCES tasks(id);
--3.2--
ALTER TABLE task_assignments 
ADD FOREIGN KEY (user_id) REFERENCES users(id);

--Задание 4--
--4.1--
INSERT INTO users (id, name) VALUES 
(1, 'Иван Сухов'),
(2, 'Нахид Сидоров'),
(3, 'Алексей Цуп');
--4.2--
INSERT INTO tasks (id, title, priority, due_date, status) VALUES 
(1, 'Разработать системы', 'Высокий', '2024-02-25', 'В процессе'),
(2, 'Создать несколько документов', 'Средний', '2024-02-20', 'Отложена'),
(3, 'Протестировать авторизацию', 'Низкий', '2024-01-10', 'Выполнена');
--4.3--
INSERT INTO task_assignments (assignment_id, task_id, user_id) VALUES 
(1, 1, 3),
(2, 1, 2),
(3, 2, 1);

--Задание 5--
--5.1--
SELECT id, title, priority, due_date, status 
FROM tasks;
--5.2--
SELECT t.id, t.title, t.priority, t.due_date, t.status 
FROM tasks t
JOIN task_assignments ta ON t.id = ta.task_id
JOIN users u ON ta.user_id = u.id
WHERE u.name = 'Иван Сухов';
--5.3--
SELECT id, title, due_date, status 
FROM tasks 
WHERE priority = 'Высокий' 
AND due_date BETWEEN '2024-01-01' AND '2024-12-31';  
--5.4--
SELECT status, COUNT(*) as task_count 
FROM tasks 
GROUP BY status;
--Задание 6--
--6.1--
SELECT 
    t.title,
    t.due_date,
    u.name
FROM tasks t
JOIN task_assignments ta ON t.id = ta.task_id
JOIN users u ON ta.user_id = u.id
WHERE t.due_date < CURRENT_DATE 
AND t.status != 'Выполнена';
--6.2--
UPDATE tasks 
SET status = 'Выполнена' 
WHERE id = 1;
--6.3--
DELETE FROM task_assignments WHERE task_id = 2;
DELETE FROM tasks WHERE id = 2;