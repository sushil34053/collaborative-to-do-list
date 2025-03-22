// user.h
#ifndef USER_H
#define USER_H

#include <string>

class User {
private:
    std::string username;
    std::string password;

public:
    User(const std::string& username, const std::string& password)
        : username(username), password(password) {}

    std::string getUsername() const { return username; }
    bool authenticate(const std::string& inputPassword) const {
        return password == inputPassword;
    }

    // Serialize user to string for file storage
    std::string serialize() const {
        return username + "|" + password;
    }

    // Deserialize from string
    static User deserialize(const std::string& data) {
        size_t pos = data.find("|");
        if (pos == std::string::npos) {
            throw std::runtime_error("Invalid user data format");
        }
        
        std::string username = data.substr(0, pos);
        std::string password = data.substr(pos + 1);
        
        return User(username, password);
    }
};

#endif // USER_H