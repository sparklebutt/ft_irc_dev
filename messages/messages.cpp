void parse_buffer(buffer)
{
	Class Message(id); 
		
	vector = std::getline(buffer);

	id::getArgs(vector);
	id::getConent(vector);
}
