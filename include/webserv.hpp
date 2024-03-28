#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define	OK		0
#define	ERR_NO_CONFIG	2
#define	ERR_NOP_SOCK	3
#define	ERR_NO_CONN	4

#define	SOCK_READ_SIZE	1024
#define	SOCK_QUEUE_SIZE	10
#define	POLL_TIMEOUT	100

enum HTTPMethod {
	HTTP_UNKNOWN,
	HTTP_GET,
	HTTP_POST,
	HTTP_DELETE
};

typedef struct	s_httprequest
{
	HTTPMethod				method;
	std::string				resource;
	std::string				version;
	std::map<std::string, std::string>	head;
	std::vector<std::byte>			body;
}		t_httprequest;

typedef struct	s_ip
{
	uint8_t	a;
	uint8_t	b;
	uint8_t	c;
	uint8_t	d;
}		t_ip;

//	socket.cpp

pollfd	open_listen_socket(const uint32_t &ip, const uint16_t &port);
pollfd	open_connection_socket(int socket_fd);

//	request.cpp

t_httprequest				parse_request(const std::string &request);
std::map<std::string, std::string>	parse_head(const std::string &head);

//	UTIL

int		error(const std::string &msg, const int &err_code);
uint32_t	ip(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
