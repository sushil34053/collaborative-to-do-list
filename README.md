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


##  🧠💾 Memory Management and Obejct Lifecycle

### 🐍 Python Version
--Memormy Management 
- Automatic Memory Management: Python uses automatic memory management, meaning developers don’t need to manually allocate or free memory. This is handled by the Python interpreter through a system called reference counting and garbage collection.
- Reference Counting: Every object in Python has an associated reference count. This is the number of references pointing to the object. When the reference count drops to zero (i.e., no references to the object are left), Python automatically deallocates the memory for that object. For example:

  - a = [1, 2, 3]  # The list object is created and 'a' points to it.
b = a  # Now 'b' also points to the same list, reference count = 2.
del a  # Reference count drops to 1.
del b  # Reference count drops to 0, memory freed.

- At this point, Python removes the list from memory.

Garbage Collection: To handle cases where objects reference each other in a cycle (like a circular reference), Python uses a garbage collector that can clean up unused objects, even if they have non-zero reference counts due to cyclical references. Python’s garbage collector runs periodically to collect these unreachable objects and free their memory.

--Object Lifecycle in Python
Creation: Objects are created when they are instantiated, and memory is allocated for them. For example:
- obj = MyClass()  # An instance of MyClass is created and memory is allocated for it.
Usage: Once created, the object can be used within the program. This involves manipulating its properties or calling methods.

Destruction: When an object is no longer referenced (i.e., when the reference count drops to zero or it is explicitly deleted), Python automatically calls the `__del__()` method if it is defined (although this is not always guaranteed). This is when the object's memory is freed, and the lifecycle ends.
