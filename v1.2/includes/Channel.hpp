#pragma once
// this might be better as a struct, i dont see the need for methods here yet

class channel {
	private:
		Unique ID
		Admin
		// map of Members by name or some other id to role such as admin
		// channel modes booleans.
		Subject
		// topic
		// global modes set 
			// channel modes invite only = false
			// channle modes something else = false
	public:

}


// Represents an IRC channel
class Channel {
	private:
		std::string name;
		std::string topic;
		std::set<User*> users; // Set of users in the channel (using pointers to User objects)
		std::set<User*> operators; // Set of channel operators
		// Add other channel properties like modes (key, invite-only, moderated, etc.)
	
	public:
		// Constructor
		Channel(const std::string& channelName) : name(channelName), topic("") {}
	
		// Get the channel name
		const std::string& getName() const {
			return name;
		}
	
		// Get the channel topic
		const std::string& getTopic() const {
			return topic;
		}
	
		// Set the channel topic
		void setTopic(const std::string& newTopic) {
			topic = newTopic;
		}
	
		// Add a user to the channel
		// Returns true if the user was added, false if already in the channel
		bool addUser(User* user) {
			// std::set::insert returns a pair: iterator to the element and a boolean indicating insertion
			auto result = users.insert(user);
			return result.second; // Return true if insertion happened (user was not already there)
		}
	
		// Remove a user from the channel
		// Returns true if the user was removed, false if not found
		bool removeUser(User* user) {
			// std::set::erase returns the number of elements removed (0 or 1 for a set)
			size_t removed_count = users.erase(user);
	
			// Also remove from operators if they were an operator
			operators.erase(user);
	
			return removed_count > 0;
		}
	
		// Check if a user is in the channel
		bool isUserInChannel(User* user) const {
			return users.count(user) > 0;
		}
	
		// Get the list of users in the channel
		const std::set<User*>& getUsers() const {
			return users;
		}
	
		// Add a user to the operator list
		// Returns true if the user became an operator, false if already one or not in channel
		bool addOperator(User* user) {
			if (isUserInChannel(user)) {
				auto result = operators.insert(user);
				return result.second;
			}
			return false; // User must be in the channel to be an operator
		}
	
		// Remove a user from the operator list
		// Returns true if the user was no longer an operator, false if not found as operator
		bool removeOperator(User* user) {
			return operators.erase(user) > 0;
		}
	
		// Check if a user is a channel operator
		bool isOperator(User* user) const {
			return operators.count(user) > 0;
		}
	
		// Method to broadcast a message to all users in the channel (excluding sender for typical chat)
		// In a real server, this would involve sending data over sockets.
		// For this example, we'll just print a message.
		void broadcastMessage(const std::string& message, User* sender = nullptr) const {
			std::cout << "Channel [" << name << "] Broadcast: " << message << std::endl;
			// In a real server:
			// for (User* user : users) {
			//     if (user != sender) {
			//         // Send message to user->socket
			//     }
			// }
		}
	
		// Handle channel modes (basic example)
		// This is a simplified representation. Real IRC modes are more complex.
		void setMode(const std::string& mode, User* user) {
			// Example: set topic protection (+t)
			if (mode == "+t") {
				// Logic to prevent non-operators from changing topic
				std::cout << name << " mode +t set." << std::endl;
			}
			// Add more mode handling logic here
		}
	
		void removeMode(const std::string& mode, User* user) {
			 // Example: remove topic protection (-t)
			if (mode == "-t") {
				// Logic to allow anyone to change topic
				std::cout << name << " mode -t set." << std::endl;
			}
			// Add more mode handling logic here
		}
	
		// Destructor (optional, but good practice if managing resources)
		~Channel() {
			// No dynamic memory allocated within the Channel class itself in this example,
			// but if you added such resources, you would clean them up here.
			// Note: This destructor does NOT delete the User objects themselves,
			// as they are managed elsewhere (e.g., in the server's list of connected users).
		}
	};
	