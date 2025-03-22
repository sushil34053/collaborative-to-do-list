class User:
    def __init__(self, username, password):
        self.username = username
        self.password = password
    
    def authenticate(self, input_password):
        """Check if input password matches user password"""
        return self.password == input_password
    
    def serialize(self):
        """Convert user to a string for file storage"""
        return f"{self.username}|{self.password}"
    
    @classmethod
    def deserialize(cls, data):
        """Create a user from a serialized string"""
        parts = data.strip().split('|')
        if len(parts) < 2:
            raise ValueError("Invalid user data format")
        
        username = parts[0]
        password = parts[1]
        
        return cls(username, password)