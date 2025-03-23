# 📝 Collaborative To-Do List Application

This project is a **command-line based task management system** implemented in both **Python** and **C++**, supporting **multiple users**, **shared tasks**, **session handling**, and **concurrent access** with file-based persistence.

---

## 🌟 Features

- 🔐 **User Authentication**: Register and log in with credentials.
- ✅ **Task Management**:
  - Add, view, update, delete tasks
  - Mark tasks as completed
  - Assign priorities: Low, Medium, High
- 👤 **Personal & Shared Tasks**:
  - Users can manage private tasks
  - Shared tasks are visible to all users
- 💾 **File Storage**:
  - Tasks and users are stored in `tasks.txt` and `users.txt`
- 🧵 **Concurrency Support**:
  - Thread-safe operations using locks/mutexes
- 💻 **CLI Interface**:
  - Menu-driven text interface

---

## 📁 Project Structure

```
project/
├── C++/
│   ├── main.cpp
│   ├── session.h
│   ├── task.h
│   ├── taskmanager.h
│   └── user.h
├── Python/
│   ├── main.py
│   ├── session.py
│   ├── task.py
│   ├── taskmanager.py
│   └── user.py
├── tasks.txt
└── users.txt
```

---

## 🚀 How to Run

### 🐍 Python Version

1. Navigate to the Python directory:
   ```bash
   cd Python
   ```

2. Run the app:
   ```bash
   python3 main.py
   ```

---

### 🧾 C++ Version

1. Navigate to the C++ directory:
   ```bash
   cd C++
   ```

2. Compile the application:
   ```bash
   g++ main.cpp -o collaborative_todo -std=c++11 -pthread
   ```

3. Run the executable:
   ```bash
   ./collaborative_todo
   ```

---

## 👥 Default User

If no `users.txt` exists, a default admin account will be created:
- **Username**: `admin`
- **Password**: `admin`

---

## 🛠️ Tech Stack

- **Languages**: Python 3, C++11
- **Concurrency**: `threading` (Python), `std::mutex` (C++)
- **Persistence**: File-based

---
