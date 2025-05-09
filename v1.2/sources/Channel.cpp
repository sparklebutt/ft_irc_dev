#include <string>
#include <vector>
#include <set> // Using set for users to easily handle uniqueness and removal
#include <map> // Using map to store channels by name
#include <algorithm> // For std::find

// Forward declaration of a User class.
// In a real IRC server, you would have a User class representing a connected client.
// For this example, we'll just use strings for usernames.
// If you have a User class, you'd replace std::string with User* or a smart pointer.
class User {
public:
    std::string nickname;
    // Add other user properties like socket descriptor, modes, etc.

    // Constructor
    User(const std::string& nick) : nickname(nick) {}

    // Need comparison operators if using User objects in std::set or std::map keys
    bool operator<(const User& other) const {
        return nickname < other.nickname;
    }
    bool operator==(const User& other) const {
        return nickname == other.nickname;
    }
};


