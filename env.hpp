#ifndef ANY_HANDLER_ENV_HPP__
#define ANY_HANDLER_ENV_HPP__

 /*
 * File Description:
 *   An any attribute backend that get its values from the environment, the
 *   key being the environment variable name.  Be aware that values written
 *   to the environment will not be exported to the calling shell.
 */

#include <cstdlib>
#include <cstring>
#include <cerrno>

#include <string>

#include <boost/any.hpp>

#include "base.hpp"


class CAnyHandlerEnv : public CAnyHandlerBase
{
public:
    virtual boost::any  Get( const std::string & key ) const;
    virtual void        Set( const std::string & key, const boost::any & value );

    virtual std::string Name() const;
    virtual std::vector<Loki::TypeInfo> GetHandledTypes() const;
};


inline boost::any  CAnyHandlerEnv::Get( const std::string & key ) const
{
    boost::any  value;
    
    char* env_value = ::getenv(key.c_str());
	if (env_value) {
		value = std::string(env_value);
	}
	    
    return  value;
}


inline void CAnyHandlerEnv::Set( const std::string & key, const boost::any & value )
{
	std::string env_value(key + "=" + boost::any_cast<std::string> (value));
    int putenvReturn = ::putenv(const_cast<char*>(env_value.c_str()));
	int savedErrno = errno;
    if (putenvReturn) {
		throw CAnyPropertyException(CAnyPropertyException::ePutenvFailed, ::strerror(savedErrno));
    }
}


inline std::string CAnyHandlerEnv::Name() const
{
    return  "environment handler";
}


inline std::vector<Loki::TypeInfo> CAnyHandlerEnv::GetHandledTypes() const
{
    return  CreateTypeVector<std::string>()();
}


#endif  //  ANY_HANDLER_ENV_HPP__
