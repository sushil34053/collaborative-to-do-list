// taskmanager.h
#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "task.h"
#include "user.h"
#include "session.h"

#include <vector>
#include <map>
#include <mutex>
#include <fstream>
#include <iostream>
#include <memory>

class TaskManager {
private:
    std::vector<Task> tasks;
    std::vector<User> users;
    std::map<int, Session> activeSessions;
    int nextTaskId;
    int nextSessionId;
    std::mutex taskMutex;
    std::mutex userMutex;
    std::mutex sessionMutex;
    
    std::string tasksFilePath;
    std::string usersFilePath;

public:
    TaskManager(const std::string& tasksFile = "tasks.txt", 
                const std::string& usersFile = "users.txt")
        : nextTaskId(1), nextSessionId(1), 
          tasksFilePath(tasksFile), usersFilePath(usersFile) {
        loadUsers();
        loadTasks();
    }

    ~TaskManager() {
        saveTasks();
        saveUsers();
    }

    // User management
    bool addUser(const std::string& username, const std::string& password) {
        std::lock_guard<std::mutex> lock(userMutex);
        for (const auto& user : users) {
            if (user.getUsername() == username) {
                return false; // User already exists
            }
        }
        users.emplace_back(username, password);
        saveUsers();
        return true;
    }

    int login(const std::string& username, const std::string& password) {
        std::lock_guard<std::mutex> userLock(userMutex);
        for (const auto& user : users) {
            if (user.getUsername() == username && user.authenticate(password)) {
                std::lock_guard<std::mutex> sessionLock(sessionMutex);
                int sessionId = nextSessionId++;
                activeSessions.emplace(sessionId, Session(sessionId, username));
                return sessionId;
            }
        }
        return -1; // Login failed
    }

    bool logout(int sessionId) {
        std::lock_guard<std::mutex> lock(sessionMutex);
        auto it = activeSessions.find(sessionId);
        if (it != activeSessions.end()) {
            activeSessions.erase(it);
            return true;
        }
        return false;
    }

    std::string getUsernameFromSession(int sessionId) {
        std::lock_guard<std::mutex> lock(sessionMutex);
        auto it = activeSessions.find(sessionId);
        if (it != activeSessions.end()) {
            return it->second.getUsername();
        }
        return "";
    }

    // Task management
    int addTask(const std::string& title, const std::string& category, 
               const std::string& assignedTo, Priority priority, bool isShared, int sessionId) {
        std::string username = getUsernameFromSession(sessionId);
        if (username.empty()) {
            return -1; // Invalid session
        }

        std::lock_guard<std::mutex> lock(taskMutex);
        int taskId = nextTaskId++;
        tasks.emplace_back(taskId, title, category, assignedTo, priority, isShared);
        saveTasks();
        return taskId;
    }

    bool updateTask(int taskId, const std::string& title, const std::string& category, 
                   const std::string& assignedTo, bool completed, Priority priority, 
                   bool isShared, int sessionId) {
        std::string username = getUsernameFromSession(sessionId);
        if (username.empty()) {
            return false; // Invalid session
        }

        std::lock_guard<std::mutex> lock(taskMutex);
        for (auto& task : tasks) {
            if (task.getId() == taskId) {
                // Check if user has permission to update this task
                if (task.getAssignedTo() == username || task.getIsShared()) {
                    task.setTitle(title);
                    task.setCategory(category);
                    task.setAssignedTo(assignedTo);
                    task.setCompleted(completed);
                    task.setPriority(priority);
                    task.setShared(isShared);
                    saveTasks();
                    return true;
                }
                return false; // No permission
            }
        }
        return false; // Task not found
    }

    bool deleteTask(int taskId, int sessionId) {
        std::string username = getUsernameFromSession(sessionId);
        if (username.empty()) {
            return false; // Invalid session
        }

        std::lock_guard<std::mutex> lock(taskMutex);
        for (auto it = tasks.begin(); it != tasks.end(); ++it) {
            if (it->getId() == taskId) {
                // Check if user has permission to delete this task
                if (it->getAssignedTo() == username || it->getIsShared()) {
                    tasks.erase(it);
                    saveTasks();
                    return true;
                }
                return false; // No permission
            }
        }
        return false; // Task not found
    }

    std::vector<Task> getPersonalTasks(int sessionId) {
        std::vector<Task> result;
        std::string username = getUsernameFromSession(sessionId);
        if (username.empty()) {
            return result; // Invalid session
        }

        std::lock_guard<std::mutex> lock(taskMutex);
        for (const auto& task : tasks) {
            if (task.getAssignedTo() == username && !task.getIsShared()) {
                result.push_back(task);
            }
        }
        return result;
    }

    std::vector<Task> getSharedTasks(int sessionId) {
        std::vector<Task> result;
        std::string username = getUsernameFromSession(sessionId);
        if (username.empty()) {
            return result; // Invalid session
        }

        std::lock_guard<std::mutex> lock(taskMutex);
        for (const auto& task : tasks) {
            if (task.getIsShared()) {
                result.push_back(task);
            }
        }
        return result;
    }

    Task* getTaskById(int taskId, int sessionId) {
        std::string username = getUsernameFromSession(sessionId);
        if (username.empty()) {
            return nullptr; // Invalid session
        }

        std::lock_guard<std::mutex> lock(taskMutex);
        for (auto& task : tasks) {
            if (task.getId() == taskId) {
                // Check if user has permission to view this task
                if (task.getAssignedTo() == username || task.getIsShared()) {
                    return &task;
                }
            }
        }
        return nullptr; // Task not found or no permission
    }

    // File I/O
    void loadTasks() {
        std::lock_guard<std::mutex> lock(taskMutex);
        tasks.clear();
        std::ifstream file(tasksFilePath);
        if (!file.is_open()) {
            return; // File doesn't exist yet
        }

        std::string line;
        while (std::getline(file, line)) {
            try {
                Task task = Task::deserialize(line);
                tasks.push_back(task);
                if (task.getId() >= nextTaskId) {
                    nextTaskId = task.getId() + 1;
                }
            } catch (const std::exception& e) {
                std::cerr << "Error loading task: " << e.what() << std::endl;
            }
        }
        file.close();
    }

    void saveTasks() {
        std::ofstream file(tasksFilePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open tasks file for writing" << std::endl;
            return;
        }

        for (const auto& task : tasks) {
            file << task.serialize() << std::endl;
        }
        file.close();
    }

    void loadUsers() {
        std::lock_guard<std::mutex> lock(userMutex);
        users.clear();
        std::ifstream file(usersFilePath);
        if (!file.is_open()) {
            // Add a default admin user if the file doesn't exist
            users.emplace_back("admin", "admin");
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            try {
                User user = User::deserialize(line);
                users.push_back(user);
            } catch (const std::exception& e) {
                std::cerr << "Error loading user: " << e.what() << std::endl;
            }
        }
        file.close();
    }

    void saveUsers() {
        std::ofstream file(usersFilePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open users file for writing" << std::endl;
            return;
        }

        for (const auto& user : users) {
            file << user.serialize() << std::endl;
        }
        file.close();
    }
};

#endif // TASKMANAGER_H