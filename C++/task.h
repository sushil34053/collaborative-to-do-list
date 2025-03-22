// task.h
#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>
#include <iostream>
#include <ctime>

enum class Priority { LOW, MEDIUM, HIGH };

class Task {
private:
    int id;
    std::string title;
    std::string category;
    std::string assignedTo;
    bool completed;
    Priority priority;
    bool isShared;
    time_t createdAt;

public:
    Task(int id, const std::string& title, const std::string& category, 
         const std::string& assignedTo, Priority priority, bool isShared)
        : id(id), title(title), category(category), assignedTo(assignedTo),
          completed(false), priority(priority), isShared(isShared) {
        createdAt = time(nullptr);
    }

    // Getters
    int getId() const { return id; }
    std::string getTitle() const { return title; }
    std::string getCategory() const { return category; }
    std::string getAssignedTo() const { return assignedTo; }
    bool isCompleted() const { return completed; }
    Priority getPriority() const { return priority; }
    bool getIsShared() const { return isShared; }
    time_t getCreatedAt() const { return createdAt; }

    // Setters
    void setTitle(const std::string& newTitle) { title = newTitle; }
    void setCategory(const std::string& newCategory) { category = newCategory; }
    void setAssignedTo(const std::string& newAssignedTo) { assignedTo = newAssignedTo; }
    void setCompleted(bool newCompleted) { completed = newCompleted; }
    void setPriority(Priority newPriority) { priority = newPriority; }
    void setShared(bool newShared) { isShared = newShared; }

    // Helper method to display priority as string
    std::string getPriorityString() const {
        switch (priority) {
            case Priority::LOW: return "Low";
            case Priority::MEDIUM: return "Medium";
            case Priority::HIGH: return "High";
            default: return "Unknown";
        }
    }

    // Serialize task to string for file storage
    std::string serialize() const {
        return std::to_string(id) + "|" + 
               title + "|" + 
               category + "|" + 
               assignedTo + "|" + 
               (completed ? "1" : "0") + "|" + 
               std::to_string(static_cast<int>(priority)) + "|" + 
               (isShared ? "1" : "0") + "|" + 
               std::to_string(createdAt);
    }

    // Deserialize from string
    static Task deserialize(const std::string& data) {
        std::vector<std::string> parts;
        std::string part;
        size_t pos = 0;
        std::string delimiter = "|";
        std::string dataCopy = data;
        
        while ((pos = dataCopy.find(delimiter)) != std::string::npos) {
            part = dataCopy.substr(0, pos);
            parts.push_back(part);
            dataCopy.erase(0, pos + delimiter.length());
        }
        parts.push_back(dataCopy); // Add the last part
        
        if (parts.size() < 8) {
            throw std::runtime_error("Invalid task data format");
        }
        
        int id = std::stoi(parts[0]);
        std::string title = parts[1];
        std::string category = parts[2];
        std::string assignedTo = parts[3];
        bool completed = parts[4] == "1";
        Priority priority = static_cast<Priority>(std::stoi(parts[5]));
        bool isShared = parts[6] == "1";
        time_t createdAt = std::stol(parts[7]);
        
        Task task(id, title, category, assignedTo, priority, isShared);
        task.setCompleted(completed);
        // Set created time
        return task;
    }

    // Display task details
    void display() const {
        std::cout << "ID: " << id << "\n";
        std::cout << "Title: " << title << "\n";
        std::cout << "Category: " << category << "\n";
        std::cout << "Assigned To: " << assignedTo << "\n";
        std::cout << "Status: " << (completed ? "Completed" : "Pending") << "\n";
        std::cout << "Priority: " << getPriorityString() << "\n";
        std::cout << "Type: " << (isShared ? "Shared" : "Personal") << "\n";
        
        char timeBuffer[26];
        std::cout << "Created At: " << std::ctime(&createdAt);
        std::cout << "Created: " << timeBuffer;
        std::cout << "----------------------\n";
    }
};

#endif // TASK_H