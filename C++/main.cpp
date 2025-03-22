// main.cpp
#include "taskmanager.h"
#include <iostream>
#include <string>
#include <limits>
#include <thread>
#include <chrono>

void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

void displayMainMenu() {
    clearScreen();
    std::cout << "===== COLLABORATIVE TO-DO LIST APPLICATION =====\n";
    std::cout << "1. Login\n";
    std::cout << "2. Register\n";
    std::cout << "3. Exit\n";
    std::cout << "Enter your choice: ";
}

void displayTaskMenu() {
    clearScreen();
    std::cout << "===== TASK MANAGEMENT =====\n";
    std::cout << "1. View Personal Tasks\n";
    std::cout << "2. View Shared Tasks\n";
    std::cout << "3. Add New Task\n";
    std::cout << "4. Update Task\n";
    std::cout << "5. Delete Task\n";
    std::cout << "6. Mark Task as Completed\n";
    std::cout << "7. Logout\n";
    std::cout << "Enter your choice: ";
}

Priority getPriorityFromUser() {
    int choice;
    std::cout << "Select priority:\n";
    std::cout << "1. Low\n";
    std::cout << "2. Medium\n";
    std::cout << "3. High\n";
    std::cout << "Enter your choice (1-3): ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    switch (choice) {
        case 1: return Priority::LOW;
        case 2: return Priority::MEDIUM;
        case 3: return Priority::HIGH;
        default: return Priority::MEDIUM;
    }
}

void handlePersonalTasks(TaskManager& taskManager, int sessionId) {
    clearScreen();
    std::cout << "===== PERSONAL TASKS =====\n";
    auto tasks = taskManager.getPersonalTasks(sessionId);
    
    if (tasks.empty()) {
        std::cout << "No personal tasks found.\n";
    } else {
        for (const auto& task : tasks) {
            task.display();
        }
    }
    
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

void handleSharedTasks(TaskManager& taskManager, int sessionId) {
    clearScreen();
    std::cout << "===== SHARED TASKS =====\n";
    auto tasks = taskManager.getSharedTasks(sessionId);
    
    if (tasks.empty()) {
        std::cout << "No shared tasks found.\n";
    } else {
        for (const auto& task : tasks) {
            task.display();
        }
    }
    
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

void handleAddTask(TaskManager& taskManager, int sessionId) {
    clearScreen();
    std::cout << "===== ADD NEW TASK =====\n";
    
    std::string title, category, assignedTo;
    char sharedChoice;
    
    std::cout << "Enter task title: ";
    std::getline(std::cin, title);
    
    std::cout << "Enter task category: ";
    std::getline(std::cin, category);
    
    std::string username = taskManager.getUsernameFromSession(sessionId);
    std::cout << "Enter user to assign task to (default: " << username << "): ";
    std::getline(std::cin, assignedTo);
    if (assignedTo.empty()) {
        assignedTo = username;
    }
    
    Priority priority = getPriorityFromUser();
    
    std::cout << "Is this a shared task? (y/n): ";
    std::cin >> sharedChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    bool isShared = (sharedChoice == 'y' || sharedChoice == 'Y');
    if (!isShared) {
        std::cout << "Defaulting to personal task.\n";
    }
    
    int taskId = taskManager.addTask(title, category, assignedTo, priority, isShared, sessionId);
    
    if (taskId != -1) {
        std::cout << "Task added successfully with ID: " << taskId << std::endl;
    } else {
        std::cout << "Failed to add task.\n";
    }
    
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

void handleUpdateTask(TaskManager& taskManager, int sessionId) {
    clearScreen();
    std::cout << "===== UPDATE TASK =====\n";
    
    int taskId;
    std::cout << "Enter task ID to update: ";
    std::cin >> taskId;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    Task* task = taskManager.getTaskById(taskId, sessionId);
    if (!task) {
        std::cout << "Task not found or you don't have permission to update it.\n";
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return;
    }
    
    std::string title, category, assignedTo;
    char completedChoice, sharedChoice;
    
    std::cout << "Current title: " << task->getTitle() << std::endl;
    std::cout << "Enter new title (leave empty to keep current): ";
    std::getline(std::cin, title);
    if (title.empty()) title = task->getTitle();
    
    std::cout << "Current category: " << task->getCategory() << std::endl;
    std::cout << "Enter new category (leave empty to keep current): ";
    std::getline(std::cin, category);
    if (category.empty()) category = task->getCategory();
    
    std::cout << "Current assigned user: " << task->getAssignedTo() << std::endl;
    std::cout << "Enter new assigned user (leave empty to keep current): ";
    std::getline(std::cin, assignedTo);
    if (assignedTo.empty()) assignedTo = task->getAssignedTo();
    
    std::cout << "Current status: " << (task->isCompleted() ? "Completed" : "Pending") << std::endl;
    std::cout << "Mark as completed? (y/n): ";
    std::cin >> completedChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    bool completed = (completedChoice == 'y' || completedChoice == 'Y');
    
    std::cout << "Current priority: " << task->getPriorityString() << std::endl;
    Priority priority = getPriorityFromUser();
    
    std::cout << "Current sharing status: " << (task->getIsShared() ? "Shared" : "Personal") << std::endl;
    std::cout << "Make this a shared task? (y/n): ";
    std::cin >> sharedChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    bool isShared = (sharedChoice == 'y' || sharedChoice == 'Y');
    
    if (taskManager.updateTask(taskId, title, category, assignedTo, completed, priority, isShared, sessionId)) {
        std::cout << "Task updated successfully.\n";
    } else {
        std::cout << "Failed to update task.\n";
    }
    
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

void handleDeleteTask(TaskManager& taskManager, int sessionId) {
    clearScreen();
    std::cout << "===== DELETE TASK =====\n";
    
    int taskId;
    std::cout << "Enter task ID to delete: ";
    std::cin >> taskId;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (taskManager.deleteTask(taskId, sessionId)) {
        std::cout << "Task deleted successfully.\n";
    } else {
        std::cout << "Failed to delete task. Task not found or you don't have permission.\n";
    }
    
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

void handleCompleteTask(TaskManager& taskManager, int sessionId) {
    clearScreen();
    std::cout << "===== MARK TASK AS COMPLETED =====\n";
    
    int taskId;
    std::cout << "Enter task ID to mark as completed: ";
    std::cin >> taskId;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    Task* task = taskManager.getTaskById(taskId, sessionId);
    if (!task) {
        std::cout << "Task not found or you don't have permission to update it.\n";
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return;
    }
    
    if (taskManager.updateTask(taskId, task->getTitle(), task->getCategory(), 
                              task->getAssignedTo(), true, task->getPriority(), 
                              task->getIsShared(), sessionId)) {
        std::cout << "Task marked as completed successfully.\n";
    } else {
        std::cout << "Failed to update task.\n";
    }
    
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

void taskManagementMenu(TaskManager& taskManager, int sessionId) {
    bool loggedIn = true;
    
    while (loggedIn) {
        displayTaskMenu();
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                handlePersonalTasks(taskManager, sessionId);
                break;
            case 2:
                handleSharedTasks(taskManager, sessionId);
                break;
            case 3:
                handleAddTask(taskManager, sessionId);
                break;
            case 4:
                handleUpdateTask(taskManager, sessionId);
                break;
            case 5:
                handleDeleteTask(taskManager, sessionId);
                break;
            case 6:
                handleCompleteTask(taskManager, sessionId);
                break;
            case 7:
                taskManager.logout(sessionId);
                loggedIn = false;
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

int main() {
    TaskManager taskManager;
    bool running = true;
    
    while (running) {
        displayMainMenu();
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                clearScreen();
                std::cout << "===== LOGIN =====\n";
                std::string username, password;
                
                std::cout << "Username: ";
                std::getline(std::cin, username);
                
                std::cout << "Password: ";
                std::getline(std::cin, password);
                
                int sessionId = taskManager.login(username, password);
                if (sessionId != -1) {
                    std::cout << "Login successful!\n";
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    taskManagementMenu(taskManager, sessionId);
                } else {
                    std::cout << "Invalid username or password.\n";
                    std::cout << "Press Enter to continue...";
                    std::cin.get();
                }
                break;
            }
            case 2: {
                clearScreen();
                std::cout << "===== REGISTER =====\n";
                std::string username, password, confirmPassword;
                
                std::cout << "Username: ";
                std::getline(std::cin, username);
                
                std::cout << "Password: ";
                std::getline(std::cin, password);
                
                std::cout << "Confirm Password: ";
                std::getline(std::cin, confirmPassword);
                
                if (password != confirmPassword) {
                    std::cout << "Passwords do not match.\n";
                } else if (taskManager.addUser(username, password)) {
                    std::cout << "Registration successful! Please login.\n";
                } else {
                    std::cout << "Username already exists.\n";
                }
                
                std::cout << "Press Enter to continue...";
                std::cin.get();
                break;
            }
            case 3:
                running = false;
                std::cout << "Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    
    return 0;
}