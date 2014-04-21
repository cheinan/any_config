#ifndef GPIPE_COMMOM_GPIPE_PROPERTY_HPP__
#define GPIPE_COMMOM_GPIPE_PROPERTY_HPP__

/*  $Id: gpipe_property.hpp 340635 2011-10-11 19:51:33Z dicuccio $
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

#include <internal/gpipe/common/boost_any.hpp>
#include <internal/gpipe/common/loki_type_info.hpp>

#include <corelib/ncbiobj.hpp>
#include <boost/lexical_cast.hpp>

#include <internal/gpipe/common/gpipe_property_exception.hpp>
#include <internal/gpipe/common/property_handlers/base.hpp>


BEGIN_NCBI_SCOPE

class CGPipeProperty
{
public:
    typedef     CRef<CGPAttrHandlerBase>    THandlerPtr;
    
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
        catch   ( CGPipePropertyNoKeyException & e ) {
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


/// Specialization to expand the lexical string to bool conversion to encompass
/// more than 0 and 1 that boost::lexical_cast does.  Also, and illustration of
/// specialization in case you want to do one yourself.
template<> inline bool
    CGPipeProperty::GetWithCast<bool, std::string>( const std::string & key ) const
{
    return  NStr::StringToBool( Get<std::string>( key ) );
}

END_NCBI_SCOPE

#endif  //  GPIPE_COMMOM_GPIPE_ATTR_HPP__
