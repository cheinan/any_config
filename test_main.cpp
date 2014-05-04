#include <iostream>
#include <string>

#include "any_property.hpp"
#include "bool_specialization.hpp"
#include "memory.hpp"
#include "env.hpp"
#include "json.hpp"

int main()
{
	std::shared_ptr<CAnyPropertyHandlerMemory<std::string>> memoryHandler(new CAnyPropertyHandlerMemory<std::string>);
	std::shared_ptr<CAnyHandlerEnv> envHandler(new CAnyHandlerEnv);
	std::shared_ptr<CAnyHandlerJSON> jsonHandler(new CAnyHandlerJSON("demo.json"));
	
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
	
	properties.AddGetHandler(jsonHandler);
	
	std::cout << properties.Get<std::string>("firstName") << std::endl;
	std::cout << properties.Get<std::string>("lastName") << std::endl;
	
	std::map<std::string, std::string> addressMap = properties.Get<std::map<std::string, std::string>>("address");
	for (auto addressLine : addressMap) {
		std::cout << addressLine.first << ": " << addressLine.second << '\n';
	}
	
	std::vector<SPhoneNumber> phones = properties.Get<std::vector<SPhoneNumber>>("phone");
	for (auto phone : phones) {
		std::cout << phone.type << ": " << phone.number << '\n';
	}

	return 0;
}
