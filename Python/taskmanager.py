import threading
import os
from task import Task
from user import User
from session import Session

class TaskManager:
    def __init__(self, tasks_file="tasks.txt", users_file="users.txt"):
        self.tasks = []
        self.users = []
        self.active_sessions = {}
        self.next_task_id = 1
        self.next_session_id = 1
        
        self.task_lock = threading.RLock()
        self.user_lock = threading.RLock()
        self.session_lock = threading.RLock()
        
        self.tasks_file_path = tasks_file
        self.users_file_path = users_file
        
        self.load_users()
        self.load_tasks()
    
    def __del__(self):
        """Save data when object is destroyed"""
        self.save_tasks()
        self.save_users()
    
    # User management
    def add_user(self, username, password):
        """Add a new user"""
        with self.user_lock:
            for user in self.users:
                if user.username == username:
                    return False  # User already exists
            
            self.users.append(User(username, password))
            self.save_users()
            return True
    
    def login(self, username, password):
        """Authenticate user and create session"""
        with self.user_lock:
            for user in self.users:
                if user.username == username and user.authenticate(password):
                    with self.session_lock:
                        session_id = self.next_session_id
                        self.next_session_id += 1
                        self.active_sessions[session_id] = Session(session_id, username)
                        return session_id
        
        return -1  # Login failed
    
    def logout(self, session_id):
        """Remove user session"""
        with self.session_lock:
            if session_id in self.active_sessions:
                del self.active_sessions[session_id]
                return True
            return False
    
    def get_username_from_session(self, session_id):
        """Get username associated with session"""
        with self.session_lock:
            if session_id in self.active_sessions:
                return self.active_sessions[session_id].get_username()
            return ""
    
    # Task management
    def add_task(self, title, category, assigned_to, priority, is_shared, session_id):
        """Add a new task"""
        username = self.get_username_from_session(session_id)
        if not username:
            return -1  # Invalid session
        
        with self.task_lock:
            task_id = self.next_task_id
            self.next_task_id += 1
            self.tasks.append(Task(task_id, title, category, assigned_to, priority, is_shared))
            self.save_tasks()
            return task_id
    
    def update_task(self, task_id, title, category, assigned_to, completed, priority, is_shared, session_id):
        """Update an existing task"""
        username = self.get_username_from_session(session_id)
        if not username:
            return False  # Invalid session
        
        with self.task_lock:
            for task in self.tasks:
                if task.id == task_id:
                    # Check if user has permission to update this task
                    if task.assigned_to == username or task.is_shared:
                        task.title = title
                        task.category = category
                        task.assigned_to = assigned_to
                        task.completed = completed
                        task.priority = priority
                        task.is_shared = is_shared
                        self.save_tasks()
                        return True
                    return False  # No permission
            return False  # Task not found
    
    def delete_task(self, task_id, session_id):
        """Delete a task"""
        username = self.get_username_from_session(session_id)
        if not username:
            return False  # Invalid session
        
        with self.task_lock:
            for i, task in enumerate(self.tasks):
                if task.id == task_id:
                    # Check if user has permission to delete this task
                    if task.assigned_to == username or task.is_shared:
                        del self.tasks[i]
                        self.save_tasks()
                        return True
                    return False  # No permission
            return False  # Task not found
    
    def get_personal_tasks(self, session_id):
        """Get all personal tasks for the current user"""
        username = self.get_username_from_session(session_id)
        if not username:
            return []  # Invalid session
        
        with self.task_lock:
            return [task for task in self.tasks 
                   if task.assigned_to == username and not task.is_shared]
    
    def get_shared_tasks(self, session_id):
        """Get all shared tasks"""
        username = self.get_username_from_session(session_id)
        if not username:
            return []  # Invalid session
        
        with self.task_lock:
            return [task for task in self.tasks if task.is_shared]
    
    def get_task_by_id(self, task_id, session_id):
        """Get a specific task by ID"""
        username = self.get_username_from_session(session_id)
        if not username:
            return None  # Invalid session
        
        with self.task_lock:
            for task in self.tasks:
                if task.id == task_id:
                    # Check if user has permission to view this task
                    if task.assigned_to == username or task.is_shared:
                        return task
            return None  # Task not found or no permission
    
    # File I/O
    def load_tasks(self):
        """Load tasks from file"""
        with self.task_lock:
            self.tasks = []
            if not os.path.exists(self.tasks_file_path):
                return  # File doesn't exist yet
            
            try:
                with open(self.tasks_file_path, 'r') as file:
                    for line in file:
                        try:
                            task = Task.deserialize(line)
                            self.tasks.append(task)
                            if task.id >= self.next_task_id:
                                self.next_task_id = task.id + 1
                        except Exception as e:
                            print(f"Error loading task: {e}")
            except Exception as e:
                print(f"Error opening tasks file: {e}")
    
    def save_tasks(self):
        """Save tasks to file"""
        try:
            with open(self.tasks_file_path, 'w') as file:
                for task in self.tasks:
                    file.write(f"{task.serialize()}\n")
        except Exception as e:
            print(f"Error saving tasks: {e}")
    
    def load_users(self):
        """Load users from file"""
        with self.user_lock:
            self.users = []
            if not os.path.exists(self.users_file_path):
                # Add a default admin user if the file doesn't exist
                self.users.append(User("admin", "admin"))
                return
            
            try:
                with open(self.users_file_path, 'r') as file:
                    for line in file:
                        try:
                            user = User.deserialize(line)
                            self.users.append(user)
                        except Exception as e:
                            print(f"Error loading user: {e}")
            except Exception as e:
                print(f"Error opening users file: {e}")
    
    def save_users(self):
        """Save users to file"""
        try:
            with open(self.users_file_path, 'w') as file:
                for user in self.users:
                    file.write(f"{user.serialize()}\n")
        except Exception as e:
            print(f"Error saving users: {e}")