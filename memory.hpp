#ifndef GPIPE_COMMOM_GPIPE_ATTR_HANDLER_MEMORY_HPP__
#define GPIPE_COMMOM_GPIPE_ATTR_HANDLER_MEMORY_HPP__

/*  $Id: memory.hpp 278667 2011-04-22 17:13:52Z dicuccio $
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
 *   A gpipe attribute backend that stores its data in memory.  The data is
 *   volatile and will disappear when the handler is destroyed.
 */

#include <string>
#include <map>

#include <corelib/ncbiobj.hpp>

#include <gpipe/common/boost_any.hpp>
#include <gpipe/common/TypelistHelper.hpp>
#include "base.hpp"


BEGIN_NCBI_SCOPE

template <typename TValue>
class CGPAttrHandlerMemory : public CGPAttrHandlerBase
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


END_NCBI_SCOPE

#endif  //  GPIPE_COMMOM_GPIPE_ATTR_HANDLER_MEMORY_HPP__
