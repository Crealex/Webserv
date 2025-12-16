#include "../../includes/colors.hpp"
#include "../../includes/includes.hpp"
#include "../../includes/requests/methodsClass.hpp"
#include "../../includes/requests/Request.hpp"
#include "../../includes/requests/Get.hpp"
#include "../../includes/requests/Post.hpp"
#include "../../includes/requests/Delete.hpp"
#include "../../includes/requests/ResponseError.hpp"
#include <cstring>
#include <exception>


int main(int argc, char **argv)
{
	Request	data;
	data._accept = "text/plain";
	data._body = "Je suis un test et j'ai conscience de ma condition de test, je suis ok avec ça";
	data._host = "../../www";
	data._location = "/testAlex/test.txt";
	data._method = "pipi";
	data._protocol = "HTTP/1.1";
	data._userAgent = "agent 007";
	data._ContentLength = 1234;
	data._ContentType = "text/txt";
	Post	post(data);
	Get		get(data);
	Delete	del(data);

	if (argc < 2)
	{
		std::cerr << RED << "Needed argument" << RESET << std::endl;
		return (1);
	}
	try 
	{

		if (!strcmp(argv[1], "post"))
			std::cout << BLUE << post.createResponse() << RESET << std::endl;
		else if (!strcmp(argv[1], "get"))
			std::cout << BLUE << get.createResponse() << RESET << std::endl;
		else if (!strcmp(argv[1], "delete"))
			std::cout << BLUE << del.createResponse() << RESET << std::endl;
		else
			std::cerr << RED << "Please choose between get, post and delete" << RESET << std::endl;
	}
	catch (ResponseError &e)
	{
		std::cerr << RED << e.createResponse() << RESET << std::endl;
	}
	catch (std::exception &e) 
	{
		std::cerr << RED << e.what() << RESET << std::endl;
	}

}
