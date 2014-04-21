/*  $Id: gph_parsed_registry.cpp 340636 2011-10-11 19:52:16Z dicuccio $
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
 *   A gpipe attribute backend that stores its data as files using the key as
 *   the file name.  This class is intended for testing and probably should
 *   not be used in production code.
 *
 *   This class handles ints, longs, doubles, and bools.
 */

#include <ncbi_pch.hpp>

#include <string>
#include <utility>

#include <corelib/ncbiobj.hpp>

#include <gpipe/common/boost_any.hpp>
#include <gpipe/common/TypelistHelper.hpp>
#include <gpipe/common/property_handlers/parsed_registry.hpp>


BEGIN_NCBI_SCOPE


CGPAttrHandlerParsedRegistry::CGPAttrHandlerParsedRegistry
(const std::string & registry_path,
 const std::string & delim )
    : m_Delim( delim )
{
    CNcbiIfstream registry_stream(registry_path.c_str());
    if ( ! registry_stream.is_open() ) {
        NCBI_THROW( CGPipePropertyException, eFileOpenFailed, registry_path );
    }
    m_Registry.Reset(new CNcbiRegistry(registry_stream));
}


boost::any  CGPAttrHandlerParsedRegistry::Get( const std::string & key ) const
{
    boost::any  value;
    
    TSectionNKey    section_plus_key = ParseSection( key );
    
    if ( m_Registry->HasEntry( section_plus_key.first, section_plus_key.second ) ) {
        value = m_Registry->Get( section_plus_key.first, section_plus_key.second );
    }
    
    return  value;
}


void    CGPAttrHandlerParsedRegistry::Set( const std::string & key, const boost::any & value )
{
    TSectionNKey    section_plus_key = ParseSection( key );
    m_Registry->Set( section_plus_key.first, section_plus_key.second,
                    boost::any_cast<std::string>( value ) );
}


std::string CGPAttrHandlerParsedRegistry::Name() const
{
    return  "parsed key CNcbiRegistry handler";
}


std::vector<Loki::TypeInfo> CGPAttrHandlerParsedRegistry::GetHandledTypes() const
{
    return  CreateTypeVector<std::string>()();
}


CGPAttrHandlerParsedRegistry::TSectionNKey
    CGPAttrHandlerParsedRegistry::ParseSection( const std::string & key ) const
{
    std::string::size_type  final_delimiter_index = key.find_last_of( m_Delim );
    std::string section_name = key.substr( 0, final_delimiter_index );
    std::string section_key = key.substr( final_delimiter_index + 1 );
    
    return  std::make_pair( section_name, section_key );
}


END_NCBI_SCOPE
