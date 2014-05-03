#ifndef ANY_PROPERTY_HPP__
#define ANY_PROPERTY_HPP__

/*
 * File Description:
 *   The client-facing interface for the gpipe properties.  The interface is
 *   implemented by the gpipe_property library.  Thus code glues together the
 *   user interface with the backend storage implementations.
 *
 */

#include <string>
#include <vector>
#include <map>
#include <typeinfo>

#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>

#include "loki_type_info.hpp"

#include "any_property_exception.hpp"
#include "for_each_if.hpp"
#include "base.hpp"


class CAnyProperty
{
public:
    typedef     std::shared_ptr<CAnyHandlerBase>    THandlerPtr;
    
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
        catch   ( CAnyPropertyNoKeyException & e ) {
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


/// Predicate functor that attempts to fetch a value from a handler given a key.
/// Returns true if the value is found.
class   CQueryHandler : public std::unary_function<CAnyProperty::THandlerPtr, bool>
{
public:
    CQueryHandler( const std::string & key ) : m_Key( key ) {}
    boost::any  GetValue() const { return m_Value; }

    /// Execute the handler function and look for a return value.
    bool    operator() ( CAnyProperty::THandlerPtr handler_ptr )
    {
        assert(m_Value.empty()); 
        m_Value = handler_ptr->Get( m_Key );
        
        return  ! m_Value.empty();
    }
    
private:
    std::string m_Key;
    boost::any  m_Value;
};


inline void    CAnyProperty::AddGetHandler( CAnyProperty::THandlerPtr handler_ptr )
{
    std::vector<Loki::TypeInfo> handled_types = handler_ptr->GetHandledTypes();
    for( auto type_iter : handled_types ) {
        TGetHandlerMap::mapped_type &  handler_list = m_GetHandlerMap[type_iter];
        handler_list.push_back( handler_ptr );
    }
}


inline boost::any
CAnyProperty::x_GetAny( const std::string & key,
                          const Loki::TypeInfo & value_type ) const
{
    if ( key.empty() ) {
        throw CAnyPropertyException(CAnyPropertyException::eEmptyKey);
    }
    
    TGetHandlerMap::const_iterator handler_list_iter = m_GetHandlerMap.find( value_type );
    if ( m_GetHandlerMap.end() == handler_list_iter ) {
        throw CAnyPropertyException( CAnyPropertyException::eNoReadHandler,
                    value_type.name() );
    }
    
    const TGetHandlerMap::mapped_type &    handler_list = handler_list_iter->second;
    
    CQueryHandler   a_query_handler =
        for_each_if( handler_list.begin(), handler_list.end(), CQueryHandler( key ) );
    
    if ( a_query_handler.GetValue().empty() ) {
        throw CAnyPropertyNoKeyException( CAnyPropertyNoKeyException::eKeyNotFound, key );
    }
    
    boost::any a = a_query_handler.GetValue();
    return a;
}


inline void    CAnyProperty::x_SetAny( const std::string & key,
                                    const boost::any & value )
{
    if ( key.empty() ) {
        throw CAnyPropertyException( CAnyPropertyException::eEmptyKey);
    }

    Loki::TypeInfo  value_type( value.type() );
    TSetHandlerMap::iterator handler_iter = m_SetHandlerMap.find( value_type );
    if ( handler_iter == m_SetHandlerMap.end() ) {
        throw CAnyPropertyException( CAnyPropertyException::eNoWriteHandler, value_type.name() );
    }
    
    THandlerPtr handler_ptr = handler_iter->second;
    assert( handler_ptr );
    
    handler_ptr->Set( key, value );
}


#endif  //  ANY_PROPERTY_HPP__
