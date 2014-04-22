/*  $Id: gpipe_property.cpp 325572 2011-07-27 01:07:44Z dicuccio $
 * ===========================================================================
 *
 *                            PUBLIC DOMAIN NOTICE
 *               National Center for Biotechnology Information
 *
 *  This software/database is a "United States Government Work" under the
 *  terms of the United States Copyright Act.  It was written as part of
 *  the author's official duties as a United States Government employee and
 *  thus cannot be copyrighted.  This software/database is freely available
 *  to the public for use. The National Library of Medicine and the U.S.
 *  Government have not placed any restriction on its use or reproduction.
 *
 *  Although all reasonable efforts have been taken to ensure the accuracy
 *  and reliability of the software and data, the NLM and the U.S.
 *  Government do not and cannot warrant the performance or results that
 *  may be obtained by using this software or data. The NLM and the U.S.
 *  Government disclaim all warranties, express or implied, including
 *  warranties of performance, merchantability or fitness for any particular
 *  purpose.
 *
 *  Please cite the author in any work or product based on this material.
 *
 * ===========================================================================
 *
 * Authors:  Cheinan Marks
 *
 * File Description:
 *   Implementation of the CGPipeProperty interface.  The correct backend
 *   is looked up and the query is passed onto it.
 *
 */

#include <functional>
#include <algorithm>
#include <typeinfo>
#include <string>

#include <ncbi_pch.hpp>
#include <corelib/ncbienv.hpp>
#include <corelib/ncbistr.hpp>

#include <internal/gpipe/common/boost_any.hpp>
#include <internal/gpipe/common/for_each_if.hpp>
#include <internal/gpipe/common/loki_type_info.hpp>
#include <internal/gpipe/common/gpipe_property_exception.hpp>
#include <internal/gpipe/common/gpipe_property.hpp>


BEGIN_NCBI_SCOPE

/// Predicate functor that attempts to fetch a value from a handler given a key.
/// Returns true if the value is found.
class   CQueryHandler : public std::unary_function<CGPipeProperty::THandlerPtr, bool>
{
public:
    CQueryHandler( const std::string & key ) : m_Key( key ) {}
    boost::any  GetValue() const { return m_Value; }

    /// Execute the handler function and look for a return value.
    bool    operator() ( CGPipeProperty::THandlerPtr handler_ptr )
    {
        _ASSERT(m_Value.empty()); 
        m_Value = handler_ptr->Get( m_Key );
        
        return  ! m_Value.empty();
    }
    
private:
    std::string m_Key;
    boost::any  m_Value;
};


void    CGPipeProperty::AddGetHandler( CGPipeProperty::THandlerPtr handler_ptr )
{
    std::vector<Loki::TypeInfo> handled_types = handler_ptr->GetHandledTypes();
    ITERATE( std::vector<Loki::TypeInfo>, type_iter, handled_types ) {
        TGetHandlerMap::mapped_type &  handler_list = m_GetHandlerMap[*type_iter];
        handler_list.push_back( handler_ptr );
    }
}


boost::any
CGPipeProperty::x_GetAny( const std::string & key,
                          const Loki::TypeInfo & value_type ) const
{
    if ( key.empty() ) {
        NCBI_THROW( CGPipePropertyException, eEmptyKey, "" );
    }
    
    TGetHandlerMap::const_iterator handler_list_iter = m_GetHandlerMap.find( value_type );
    if ( m_GetHandlerMap.end() == handler_list_iter ) {
        NCBI_THROW( CGPipePropertyException, eNoReadHandler,
                    value_type.name() );
    }
    
    const TGetHandlerMap::mapped_type &    handler_list = handler_list_iter->second;
    
    CQueryHandler   a_query_handler =
        for_each_if( handler_list.begin(), handler_list.end(), CQueryHandler( key ) );
    
    if ( a_query_handler.GetValue().empty() ) {
        NCBI_THROW( CGPipePropertyNoKeyException, eKeyNotFound, key );
    }
    
    boost::any a = a_query_handler.GetValue();
    return a;
}


void    CGPipeProperty::x_SetAny( const std::string & key,
                                    const boost::any & value )
{
    if ( key.empty() ) {
        NCBI_THROW( CGPipePropertyException, eEmptyKey, "" );
    }

    Loki::TypeInfo  value_type( value.type() );
    TSetHandlerMap::iterator handler_iter = m_SetHandlerMap.find( value_type );
    if ( handler_iter == m_SetHandlerMap.end() ) {
        NCBI_THROW( CGPipePropertyException, eNoWriteHandler, value_type.name() );
    }
    
    THandlerPtr handler_ptr = handler_iter->second;
    _ASSERT( handler_ptr );
    
    handler_ptr->Set( key, value );
}


END_NCBI_SCOPE
