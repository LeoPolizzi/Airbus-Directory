#include "Socket.hpp"

#ifdef _WIN32

Socket::Socket(const std::string& ip, int port) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error("WSAStartup failed");
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        throw std::runtime_error("Failed to create socket");
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) {
        closesocket(sock);
        WSACleanup();
        throw std::runtime_error("Invalid IP address");
    }
}

bool Socket::send(const std::string& data) {
    int sent = sendto(
        sock,
        data.c_str(),
        static_cast<int>(data.size()),
        0,
        reinterpret_cast<sockaddr*>(&addr),
        sizeof(addr)
    );

    return (sent == static_cast<int>(data.size()));
}

Socket::~Socket() {
    closesocket(sock);
    WSACleanup();
}

#else

Socket::Socket(const std::string& ip, int port) {
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		throw std::runtime_error("Failed to create socket");
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) {
		close(sock);
		throw std::runtime_error("Invalid IP address");
	}
}

bool Socket::send(const std::string& data) {
	int sent = sendto(
		sock,
		data.c_str(),
		static_cast<int>(data.size()),
		0,
		reinterpret_cast<sockaddr*>(&addr),
		sizeof(addr)
	);

	return (sent == (int)data.size());
}

Socket::~Socket() {
	close(sock);
}

#endif
