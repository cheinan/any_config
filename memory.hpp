#ifndef ANY_PROPERTY_HANDLER_MEMORY_HPP__
#define ANY_PROPERTY_HANDLER_MEMORY_HPP__

/*
 * File Description:
 *   An any property backend that stores its data in memory.  The data is
 *   volatile and will disappear when the handler is destroyed.
 */

#include <string>
#include <map>

#include <boost/any.hpp>

#include "TypelistHelper.hpp"
#include "base.hpp"


template <typename TValue>
class CAnyPropertyHandlerMemory : public CAnyHandlerBase
{
public:
    virtual boost::any  Get( const std::string & key ) const
    {
        boost::any  value;
        
        typename std::map<std::string, TValue>::const_iterator   it = m_Map.find( key );
        if ( it !=  m_Map.end() ) {
            value = it->second;
        }
        
        return  value;
    }

    virtual void        Set( const std::string & key, const boost::any & value )
    {
        m_Map[key] = boost::any_cast<TValue> ( value );
    }

    virtual std::string Name() const { return  "memory handler"; }
    virtual std::vector<Loki::TypeInfo> GetHandledTypes() const
    {
        return  CreateTypeVector<TValue>()();
    }

private:
    std::map<std::string, TValue>    m_Map;
};


#endif  //  ANY_PROPERTY_HANDLER_MEMORY_HPP__
