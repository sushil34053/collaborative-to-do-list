import time

class Session:
    def __init__(self, session_id, username):
        self.session_id = session_id
        self.username = username
        self.login_time = int(time.time())
    
    def get_username(self):
        return self.username