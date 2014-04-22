#ifndef ANY_PROPERTY_HPP__
#define ANY_PROPERTY_HPP__

/*
 * File Description:
 *   The client-facing interface for the gpipe properties.  The interface is
 *   implemented by the gpipe_property library.  Thus code glues together the
 *   user interface with the backend storage implementations.
 *
 *   This interface is a generic interface that can access the environment, ini
 *   files or database.  For a gpipe-specific class to access these in a gpipe
 *   enviornment, see CGPipeAttribute which is implemented with this class.
 *
 */

#include <string>
#include <vector>
#include <map>
#include <typeinfo>

#include "boost_any.hpp"
#include "loki_type_info.hpp"

#include <boost/lexical_cast.hpp>

#include "any_property_exception.hpp"
#include "base.hpp"


class CGPipeProperty
{
public:
    typedef     std::shared_ptr<CHandlerBase>    THandlerPtr;
    
    template<typename T> T  Get( const std::string & key ) const
    {
        return  boost::any_cast<T>( x_GetAny( key, typeid( T ) ) );
    }
    
    template<typename T> T  GetWithDefault( const std::string & key, const T & default_value ) const
    {
        T   value;
        
        try {
            value = Get<T>( key );
        }
        catch   ( CPropertyNoKeyException & e ) {
            value = default_value;
        }
        
        return  value;
    }
    
    /// Template deduction is not practical with literal strings.  This
    /// overload allows the client to specifiy literal string defaults without
    /// an explicit template specification.
    std::string  GetWithDefault( const std::string & key, const char * a_c_string ) const
    {
        return  GetWithDefault<std::string>( key, a_c_string );
    }
    
    
    /// Get an attribute that's stored as one type and cast it to another.
    /// E.g.  GetWithCast<double, std::string> from the environment will
    /// attempt fetch and environment string and attempt to cast it to a
    /// double.
    //TODO: We need ini file and env backend handlers that can handle doubles,
    //bools, ints etc.  GetWithCast is not sufficient because the client will
    //have to make separate calls to get an int (e.g. build ID) from the env
    //first and then from the DB.
    template<typename TCast, typename THandlerLookup>
    TCast  GetWithCast( const std::string & key ) const
    {
        return  boost::lexical_cast<TCast>( Get<THandlerLookup>( key ) );
    }

    template<typename T> void   Set( const std::string & key, const T & value )
    {
        x_SetAny( key, value );
    }
    
    /// An overload to allow template type deduction for literal strings.
    void   Set( const std::string & key, const char * a_c_string )
    {
        Set<std::string>( key, a_c_string );
    }
    
    /// Append a read-handler to the list of handlers for the types it handles.
    /// The added handler will be queried after the alrady existing handlers
    /// are queried.
    void    AddGetHandler( THandlerPtr handler_ptr );
    
    /// Set the handler that will be used to write an attribute for a given
    /// type.  The handler can be one of the get-handlers or it can be
    /// distinct.
    template<typename T> void    SetSetHandler( THandlerPtr handler_ptr )
    {
        m_SetHandlerMap[Loki::TypeInfo( typeid( T ) )] = handler_ptr;
    }
    
private:
    boost::any  x_GetAny( const std::string & key, const Loki::TypeInfo & value_type ) const;
    void        x_SetAny( const std::string & key, const boost::any & value );
    
    typedef std::map<Loki::TypeInfo, std::vector<THandlerPtr> >    TGetHandlerMap;
    TGetHandlerMap  m_GetHandlerMap;
    
    typedef std::map<Loki::TypeInfo, THandlerPtr>    TSetHandlerMap;
    TSetHandlerMap  m_SetHandlerMap;
};


#endif  //  ANY_PROPERTY_HPP__
