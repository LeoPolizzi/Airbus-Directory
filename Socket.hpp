#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include <string>

#ifdef _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#else

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#endif

class Socket {
private:
	int sock;
	sockaddr_in addr;
public:
	Socket(const std::string &ip, int port);
	~Socket();

	bool send(const std::string& data);
};
