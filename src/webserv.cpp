#include "webserv.hpp"
#include "VHost.hpp"
#include "Server.hpp"

int	main(int argc, char **argv)
{
	std::vector<t_conf>	configs;
	t_conf			config;
	Server			server;
	int			init_status;
	std::string		buffer;
	t_httprequest		request;
	t_httpresponse		response;
	t_route			post_route;

	argv = argv;
	if (argc <= 1)
		return (error("no configuration file provided", ERR_NO_CONFIG));
	config.ip = 0;
	config.port = 9999;
	config.default_route.index = "/index.html";
	config.default_route.autoindex = false;
	config.default_route.root = "/home/jburleson/documents/codam/webserv_website";
	config.default_route.allowed_methods.insert(HTTP_GET);
	config.max_body_size = 1024;
	post_route.alias = "/post";
	post_route.root = "/home/jburleson/documents/codam/webserv_post";
	post_route.allowed_methods.insert(HTTP_POST);
	config.routes.push_back(post_route);
	post_route.alias = "/del";
	post_route.allowed_methods.insert(HTTP_DELETE);
	config.routes.push_back(post_route);
	configs.push_back(config);
	config.server_names.push_back("localhost");
	config.default_route.autoindex = true;
	config.default_route.index = "";
	config.port = 9999;
	configs.push_back(config);
	init_status = server.init(configs);
	if (init_status != OK)
		return (error("port conflict in configuration", init_status));
	while (true)
	{
		server.poll_events();
		for (const t_socket &socket : server.get_sockets())
		{
			if (socket.type == LISTEN && has_socket_recv(socket))
				server.add_connection(socket);
			else if (socket.type == CONNECTION && server.has_response(socket) && can_socket_send(socket))
				server.send_queued_response(socket);
			else if (socket.type == CONNECTION && has_socket_recv(socket))
			{
				buffer = read_file(socket.poll_fd.fd);
				if (buffer.empty())
				{
					server.close_socket(socket);
					continue ;
				}
				request = parse_request(buffer);
				request.client = socket.poll_fd;
				response = server.process_request(request, socket);
				server.add_response(socket, response);
			}
		}
	}
	return (OK);
}

