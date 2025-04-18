

// https://modern.ircdocs.horse/#message-format

class message{
	private
		std::string from (array)
		std::string to (array)
		std::string subject
		std::string content

	public
		send_message
		recieve_message
		parse_message
}