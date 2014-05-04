#ifndef ANY_HANDLER_JSON_SPIRIT_HPP__
#define ANY_HANDLER_JSON_SPIRIT_HPP__

#include <fstream>

#include <boost/any.hpp>

#include <json_spirit/json_spirit_reader_template.h>

#include "base.hpp"


struct SPhoneNumber
{
	std::string type;
	std::string number;
	
	SPhoneNumber(const std::string& theType, const std::string theNumber) : type(theType), number(theNumber) {}
};


class CAnyHandlerJSON : public CAnyHandlerBase
{
public:
	CAnyHandlerJSON(const std::string& jsonFileName);
	
    virtual boost::any  Get( const std::string & key ) const;

    virtual std::string Name() const;
    virtual std::vector<Loki::TypeInfo> GetHandledTypes() const;

private:
	std::map<std::string, boost::any> m_values;
};


inline CAnyHandlerJSON::CAnyHandlerJSON(const std::string& jsonFileName)
{
    std::ifstream demoFileStream(jsonFileName.c_str());
    json_spirit::mValue jsonValue;
	json_spirit::read_stream(demoFileStream, jsonValue);
	auto topObject = jsonValue.get_obj();
	
	m_values["firstName"] = topObject["firstName"].get_value<std::string>();
	m_values["lastName"] = topObject["lastName"].get_value<std::string>();
	m_values["age"] = topObject["age"].get_value<int>();
	
	std::map<std::string, std::string> addressMap;
	auto addressObject = topObject["address"].get_obj();
	addressMap["streetAddress"] = addressObject["streetAddress"].get_value<std::string>();
	addressMap["city"] = addressObject["city"].get_value<std::string>();
	addressMap["state"] = addressObject["state"].get_value<std::string>();
	addressMap["postalCode"] = addressObject["postalCode"].get_value<std::string>();
	m_values["address"] = addressMap;
	
	std::vector<SPhoneNumber> phoneVector;
	auto phoneArray = topObject["phoneNumber"].get_array();
	phoneVector.reserve(phoneArray.size());
	for (auto phoneEntry : phoneArray) {
		phoneVector.emplace_back(phoneEntry.get_obj()["type"].get_value<std::string>(), phoneEntry.get_obj()["number"].get_value<std::string>());
	}
	m_values["phone"] = phoneVector;
}


inline boost::any  CAnyHandlerJSON::Get( const std::string & key ) const
{
    return  m_values.at(key);
}


inline std::string CAnyHandlerJSON::Name() const
{
    return  "json_spirit handler";
}


inline std::vector<Loki::TypeInfo> CAnyHandlerJSON::GetHandledTypes() const
{
    return  CreateTypeVector<std::string, int, std::map<std::string, std::string>, std::vector<SPhoneNumber>>()();
}


#endif // ANY_HANDLER_JSON_SPIRIT_HPP__
