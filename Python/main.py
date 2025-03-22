import os
import time
from taskmanager import TaskManager
from task import Priority  # Import the Priority enum from task.py

def clear_screen():
    """Clear the terminal screen"""
    os.system('cls' if os.name == 'nt' else 'clear')

def display_main_menu():
    """Display the main menu"""
    clear_screen()
    print("===== COLLABORATIVE TO-DO LIST APPLICATION =====")
    print("1. Login")
    print("2. Register")
    print("3. Exit")
    
    return input("Enter your choice: ")

def display_task_menu():
    """Display the task management menu"""
    clear_screen()
    print("===== TASK MANAGEMENT =====")
    print("1. View Personal Tasks")
    print("2. View Shared Tasks")
    print("3. Add New Task")
    print("4. Update Task")
    print("5. Delete Task")
    print("6. Mark Task as Completed")
    print("7. Logout")
    
    return input("Enter your choice: ")

def get_priority_from_user():
    """Get priority selection from user"""
    print("Select priority:")
    print("1. Low")
    print("2. Medium")
    print("3. High")
    
    choice = input("Enter your choice (1-3): ")
    
    try:
        choice_num = int(choice)
        if choice_num == 1:
            return Priority.LOW
        elif choice_num == 2:
            return Priority.MEDIUM
        elif choice_num == 3:
            return Priority.HIGH
        else:
            return Priority.MEDIUM
    except:
        return Priority.MEDIUM

def handle_personal_tasks(task_manager, session_id):
    """Display personal tasks"""
    clear_screen()
    print("===== PERSONAL TASKS =====")
    tasks = task_manager.get_personal_tasks(session_id)
    
    if not tasks:
        print("No personal tasks found.")
    else:
        for task in tasks:
            task.display()
    
    input("Press Enter to continue...")

def handle_shared_tasks(task_manager, session_id):
    """Display shared tasks"""
    clear_screen()
    print("===== SHARED TASKS =====")
    tasks = task_manager.get_shared_tasks(session_id)
    
    if not tasks:
        print("No shared tasks found.")
    else:
        for task in tasks:
            task.display()
    
    input("Press Enter to continue...")

def handle_add_task(task_manager, session_id):
    """Add a new task"""
    clear_screen()
    print("===== ADD NEW TASK =====")
    
    title = input("Enter task title: ")
    category = input("Enter task category: ")
    
    username = task_manager.get_username_from_session(session_id)
    assigned_to = input(f"Enter user to assign task to (default: {username}): ")
    if not assigned_to:
        assigned_to = username
    
    priority = get_priority_from_user()
    
    is_shared = input("Is this a shared task? (y/n): ").lower() == 'y'

    if not is_shared:
        print("Defaulting to personal task.")    

    task_id = task_manager.add_task(title, category, assigned_to, priority, is_shared, session_id)
    
    if task_id != -1:
        print(f"Task added successfully with ID: {task_id}")
    else:
        print("Failed to add task.")
    
    input("Press Enter to continue...")

def handle_update_task(task_manager, session_id):
    """Update an existing task"""
    clear_screen()
    print("===== UPDATE TASK =====")
    
    try:
        task_id = int(input("Enter task ID to update: "))
    except:
        print("Invalid task ID.")
        input("Press Enter to continue...")
        return
    
    task = task_manager.get_task_by_id(task_id, session_id)
    if not task:
        print("Task not found or you don't have permission to update it.")
        input("Press Enter to continue...")
        return
    
    print(f"Current title: {task.title}")
    title = input("Enter new title (leave empty to keep current): ")
    if not title:
        title = task.title
    
    print(f"Current category: {task.category}")
    category = input("Enter new category (leave empty to keep current): ")
    if not category:
        category = task.category
    
    print(f"Current assigned user: {task.assigned_to}")
    assigned_to = input("Enter new assigned user (leave empty to keep current): ")
    if not assigned_to:
        assigned_to = task.assigned_to
    
    print(f"Current status: {'Completed' if task.completed else 'Pending'}")
    completed = input("Mark as completed? (y/n): ").lower() == 'y'
    
    print(f"Current priority: {task.priority}")
    priority = get_priority_from_user()
    
    print(f"Current sharing status: {'Shared' if task.is_shared else 'Personal'}")
    is_shared = input("Make this a shared task? (y/n): ").lower() == 'y'
    
    if task_manager.update_task(task_id, title, category, assigned_to, completed, priority, is_shared, session_id):
        print("Task updated successfully.")
    else:
        print("Failed to update task.")
    
    input("Press Enter to continue...")

def handle_delete_task(task_manager, session_id):
    """Delete a task"""
    clear_screen()
    print("===== DELETE TASK =====")
    
    try:
        task_id = int(input("Enter task ID to delete: "))
    except:
        print("Invalid task ID.")
        input("Press Enter to continue...")
        return
    
    if task_manager.delete_task(task_id, session_id):
        print("Task deleted successfully.")
    else:
        print("Failed to delete task. Task not found or you don't have permission.")
    
    input("Press Enter to continue...")

def handle_complete_task(task_manager, session_id):
    """Mark a task as completed"""
    clear_screen()
    print("===== MARK TASK AS COMPLETED =====")
    
    try:
        task_id = int(input("Enter task ID to mark as completed: "))
    except:
        print("Invalid task ID.")
        input("Press Enter to continue...")
        return
    
    task = task_manager.get_task_by_id(task_id, session_id)
    if not task:
        print("Task not found or you don't have permission to update it.")
        input("Press Enter to continue...")
        return
    
    if task_manager.update_task(task_id, task.title, task.category, task.assigned_to, 
                              True, task.priority, task.is_shared, session_id):
        print("Task marked as completed successfully.")
    else:
        print("Failed to update task.")
    
    input("Press Enter to continue...")

def task_management_menu(task_manager, session_id):
    """Handle task management operations"""
    logged_in = True
    
    while logged_in:
        choice = display_task_menu()
        
        if choice == '1':
            handle_personal_tasks(task_manager, session_id)
        elif choice == '2':
            handle_shared_tasks(task_manager, session_id)
        elif choice == '3':
            handle_add_task(task_manager, session_id)
        elif choice == '4':
            handle_update_task(task_manager, session_id)
        elif choice == '5':
            handle_delete_task(task_manager, session_id)
        elif choice == '6':
            handle_complete_task(task_manager, session_id)
        elif choice == '7':
            task_manager.logout(session_id)
            logged_in = False
        else:
            print("Invalid choice. Please try again.")
            time.sleep(1)

def main():
    """Main application function"""
    task_manager = TaskManager()
    running = True
    
    while running:
        choice = display_main_menu()
        
        if choice == '1':
            clear_screen()
            print("===== LOGIN =====")
            
            username = input("Username: ")
            password = input("Password: ")
            
            session_id = task_manager.login(username, password)
            if session_id != -1:
                print("Login successful!")
                time.sleep(1)
                task_management_menu(task_manager, session_id)
            else:
                print("Invalid username or password.")
                input("Press Enter to continue...")
        
        elif choice == '2':
            clear_screen()
            print("===== REGISTER =====")
            
            username = input("Username: ")
            password = input("Password: ")
            confirm_password = input("Confirm Password: ")
            
            if password != confirm_password:
                print("Passwords do not match.")
            elif task_manager.add_user(username, password):
                print("Registration successful! Please login.")
            else:
                print("Username already exists.")
            
            input("Press Enter to continue...")
        
        elif choice == '3':
            running = False
            print("Goodbye!")
        
        else:
            print("Invalid choice. Please try again.")
            time.sleep(1)

if __name__ == "__main__":
    main()