/*  $Id: gph_env.cpp 254760 2011-02-16 22:16:08Z marksc2 $
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
 *   A gpipe attribute backend that stores its data in the environment.
 *
 *   This class handles ints, longs, doubles, and bools.
 */

#include <string>

#include "boost_any.hpp"
#include "gpipe_property_exception.hpp"
#include "TypelistHelper.hpp
#include "property_handlers/env.hpp"


boost::any  CGPAttrHandlerEnv::Get( const std::string & key ) const
{
    boost::any  value;
    
    const std::string & value_string = m_Env.Get( key );
    if ( &kEmptyStr != &value_string ) {
        value = value_string;
    }
    
    return  value;
}


void        CGPAttrHandlerEnv::Set( const std::string & key, const boost::any & value )
{
    m_Env.Set( key, boost::any_cast<std::string> ( value ) );
}


std::string CGPAttrHandlerEnv::Name() const
{
    return  "environment handler";
}


std::vector<Loki::TypeInfo> CGPAttrHandlerEnv::GetHandledTypes() const
{
    return  CreateTypeVector<std::string>()();
}
