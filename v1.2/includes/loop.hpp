
// one possible way to see if we need to make stupid messages just for netcat, since we dont apply our own when 
// irssi utalizes its own.
/*void detectClientType(const std::string& initialMessage) {
    if (initialMessage.find("CAP LS") != std::string::npos) {
        std::cout << "Client appears to be Irssi or another full-featured IRC client." << std::endl;
    } else {
        std::cout << "Client is likely Netcat or a simpler connection." << std::endl;
    }
}*/
