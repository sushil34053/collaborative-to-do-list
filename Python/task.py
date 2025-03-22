import time
from enum import Enum

class Priority(Enum):
    LOW = 1
    MEDIUM = 2
    HIGH = 3
    
    def __str__(self):
        return self.name.capitalize()

class Task:
    def __init__(self, task_id, title, category, assigned_to, priority, is_shared):
        self.id = task_id
        self.title = title
        self.category = category
        self.assigned_to = assigned_to
        self.completed = False
        self.priority = priority
        self.is_shared = is_shared
        self.created_at = int(time.time())
        
    def serialize(self):
        """Convert task to a string for file storage"""
        return f"{self.id}|{self.title}|{self.category}|{self.assigned_to}|{1 if self.completed else 0}|{self.priority.value}|{1 if self.is_shared else 0}|{self.created_at}"
    
    @classmethod
    def deserialize(cls, data):
        """Create a task from a serialized string"""
        parts = data.strip().split('|')
        if len(parts) < 8:
            raise ValueError("Invalid task data format")
        
        task_id = int(parts[0])
        title = parts[1]
        category = parts[2]
        assigned_to = parts[3]
        completed = parts[4] == '1'
        priority = Priority(int(parts[5]))
        is_shared = parts[6] == '1'
        created_at = int(parts[7])
        
        task = cls(task_id, title, category, assigned_to, priority, is_shared)
        task.completed = completed
        task.created_at = created_at
        return task
    
    def display(self):
        """Display task details"""
        print(f"ID: {self.id}")
        print(f"Title: {self.title}")
        print(f"Category: {self.category}")
        print(f"Assigned To: {self.assigned_to}")
        print(f"Status: {'Completed' if self.completed else 'Pending'}")
        print(f"Priority: {self.priority}")
        print(f"Type: {'Shared' if self.is_shared else 'Personal'}")
        print(f"Created: {time.ctime(self.created_at)}")
        print("----------------------")