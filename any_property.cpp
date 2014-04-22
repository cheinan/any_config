/*
 * File Description:
 *   Implementation of the CAnyProperty interface.  The correct backend
 *   is looked up and the query is passed onto it.
 *
 */

#include <functional>
#include <algorithm>
#include <typeinfo>
#include <string>
#include <cassert>

#include <boost/any.hpp>

#include "for_each_if.hpp"
#include "loki_type_info.hpp"
#include "any_property_exception.hpp"
#include "any_property.hpp"


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


void    CAnyProperty::AddGetHandler( CAnyProperty::THandlerPtr handler_ptr )
{
    std::vector<Loki::TypeInfo> handled_types = handler_ptr->GetHandledTypes();
    for( auto type_iter : handled_types ) {
        TGetHandlerMap::mapped_type &  handler_list = m_GetHandlerMap[type_iter];
        handler_list.push_back( handler_ptr );
    }
}


boost::any
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


void    CAnyProperty::x_SetAny( const std::string & key,
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
