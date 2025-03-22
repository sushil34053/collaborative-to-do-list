// session.h
#ifndef SESSION_H
#define SESSION_H

#include <string>
#include <ctime>

class Session {
private:
    int sessionId;
    std::string username;
    time_t loginTime;

public:
    Session(int sessionId, const std::string& username)
        : sessionId(sessionId), username(username) {
        loginTime = time(nullptr);
    }

    int getSessionId() const { return sessionId; }
    std::string getUsername() const { return username; }
    time_t getLoginTime() const { return loginTime; }
};

#endif // SESSION_H