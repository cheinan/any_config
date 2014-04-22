#include <iostream>
#include <string>

#include "any_property.hpp"
#include "bool_specialization.hpp"
#include "memory.hpp"

int main()
{
	std::shared_ptr<CAnyPropertyHandlerMemory<std::string>> memoryHandler(new CAnyPropertyHandlerMemory<std::string>);
	
	CAnyProperty properties;
	properties.AddGetHandler(memoryHandler);
	properties.SetSetHandler<std::string>(memoryHandler);
	
	properties.Set("some key", "some value");
	std::cout << properties.Get<std::string>("some key") << std::endl;
	
	return 0;
}
