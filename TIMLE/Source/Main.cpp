#include "../Include/Application.hpp"

#include <stdexcept>
#include <iostream>


int main()
{
	try
	{
		Application app;
		app.run();
	}
	catch (std::exception& e)
	{
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
