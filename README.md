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

### 🔒 **Error Handling and Type Safety**


#### 🧱 C++ Approach

```cpp
#include <iostream>
#include <stdexcept>

int parseInt(const std::string& input) {
    try {
        return std::stoi(input);  // type-safe conversion
    } catch (const std::invalid_argument& e) {
        throw std::runtime_error("Invalid number: " + input);
    }
}
```

#### 🐍 Python Approach

```python
def parse_int(input_str):
    try:
        return int(input_str)  # dynamic conversion
    except ValueError:
        raise ValueError(f"Invalid number: {input_str}")
```

#### 🔍 Key Differences

| Aspect                | C++                                                | Python                                             |
|-----------------------|-----------------------------------------------------|----------------------------------------------------|
| **Type Safety**        | Strong static typing; type conversions must be explicit (`std::stoi`, `static_cast`) | Dynamically typed; type conversions are looser (`int()`, `enum(int)`) |
| **Error Handling**     | Uses `try`/`catch` blocks with exceptions like `std::runtime_error` | Uses `try`/`except` with exceptions like `ValueError` |
| **Error Messages**     | Explicit exception thrown with `throw` | Raised with `raise` keyword |
| **Compile-Time Checks**| Catches more issues at compile time (e.g., missing return types, bad casts) | Many issues are caught at runtime due to dynamic typing |
| **Memory Safety**      | Errors like buffer overflows are possible if not careful | Memory safety is handled by the interpreter, reducing risk |

> ⚖️ **Key Takeaway**: C++ enforces error handling and type safety at compile time, offering high control but requiring more code and caution. Python provides a simpler, more flexible model that prioritizes readability and development speed, while relying on runtime checks to handle errors.
