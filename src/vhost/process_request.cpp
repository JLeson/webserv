#include "VHost.hpp"

void	VHost::_process_get_method(const t_httprequest &request, t_httpresponse &response) const
{
	if (!file_exists(this->_parse_resource(request.resource)))
	{
		response.status = HTTP_NOT_FOUND;
		response.body = this->_get_err_page(HTTP_NOT_FOUND);
	}
	else
	{
		response.status = HTTP_OK;
		response.body = ftobyte(this->_parse_resource(request.resource));
	}
	response.head.insert({"Content-Type", "text/html"});
}

void	VHost::_process_post_method(const t_httprequest &request, t_httpresponse &response) const
{
	if (request.head.find("Expect") != request.head.end())
	{
		if (request.head.at("Expect") == "100-continue")
		{
			response.status = HTTP_CONTINUE;
			return ;
		}
	}
	std::ofstream	new_file(this->_parse_resource(request.resource));

	for (const std::byte &byte : request.body)
		new_file << (char)byte;
	response.status = HTTP_NO_CONTENT;
}

void	VHost::_process_delete_method(const t_httprequest &request, t_httpresponse &response) const
{
	int	status = std::remove(this->_parse_resource(request.resource).c_str());	
	
	if (status != 0)
		response.status = HTTP_NOT_FOUND;
	if (status == 0)
		response.status = HTTP_NO_CONTENT;
}
