#include <iostream>
#include <string>

#include "any_property.hpp"
#include "bool_specialization.hpp"
#include "memory.hpp"
#include "env.hpp"

int main()
{
	std::shared_ptr<CAnyPropertyHandlerMemory<std::string>> memoryHandler(new CAnyPropertyHandlerMemory<std::string>);
	std::shared_ptr<CAnyHandlerEnv> envHandler(new CAnyHandlerEnv);
	
	CAnyProperty properties;
	properties.AddGetHandler(memoryHandler);
	properties.SetSetHandler<std::string>(memoryHandler);
	
	properties.Set("some key", "some value");
	std::cout << properties.Get<std::string>("some key") << std::endl;
	
	properties.AddGetHandler(envHandler);
	properties.SetSetHandler<std::string>(envHandler);
	
	try {
		std::cout << properties.Get<std::string>("ENVKEY") << std::endl;
	}
	catch (CAnyPropertyNoKeyException & e) {
		std::cerr << "Key \"" << e.GetKeyName() << "\" was not found in the environment!" << std::endl;
	}
	
	try {
		properties.Set("ENVKEY", "some value");
	}
	catch (CAnyPropertyException& e)
	{
		std::cerr << "CAnyPropertyException: " << e.GetErrCodeString() << std::endl;
	}

	try {
		std::cout << properties.Get<std::string>("ENVKEY") << std::endl;
	}
	catch (CAnyPropertyNoKeyException & e) {
		std::cerr << "Key \"" << e.GetKeyName() << "\" was not found in the environment!" << std::endl;
	}

	return 0;
}
