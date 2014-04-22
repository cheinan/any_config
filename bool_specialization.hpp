#ifndef BOOL_SPECIALIZATION_HPP__
#define BOOL_SPECIALIZATION_HPP__

#include <stdexcept>

/// Specialization to expand the lexical string to bool conversion to encompass
/// more than 0 and 1 that boost::lexical_cast does.  Also, an illustration of
/// specialization in case you want to do one yourself.
template<> inline bool
    CAnyProperty::GetWithCast<bool, std::string>( const std::string & key ) const
{
	std::string value = Get<std::string>( key );
	if (key == "true") return true;
	if (key == "false") return false;
	
	std::string errorString = "GetWithCast<bool, std::string> got a value of \"";
	errorString += value;
	errorString += "\"";
	
	throw std::logic_error(errorString);
    return  false;
}

#endif // BOOL_SPECIALIZATION_HPP__
