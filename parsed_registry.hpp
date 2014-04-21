#ifndef GPIPE_COMMOM_GPIPE_ATTR_HANDLER_PARSED_REGISTRY_HPP__
#define GPIPE_COMMOM_GPIPE_ATTR_HANDLER_PARSED_REGISTRY_HPP__

/*  $Id: parsed_registry.hpp 340636 2011-10-11 19:52:16Z dicuccio $
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
 *   A gpipe attribute backend that fronts for CNcbiRegistry with the addition
 *   of a parser that decodes the section name that is separated from the key
 *   by a delimiter.  The parser is greedy, treating the last instance of the
 *   delimeter as the actual delimeter.
 */

#include <string>

#include <corelib/ncbiobj.hpp>
#include <corelib/ncbireg.hpp>

#include <gpipe/common/boost_any.hpp>
#include <gpipe/common/loki_type_info.hpp>
#include <gpipe/common/gpipe_property_exception.hpp>
#include <gpipe/common/property_handlers/base.hpp>


BEGIN_NCBI_SCOPE

class CGPAttrHandlerParsedRegistry : public CGPAttrHandlerBase
{
public:
    virtual boost::any  Get( const std::string & key ) const;
    virtual void        Set( const std::string & key, const boost::any & value );

    virtual std::string Name() const;
    virtual std::vector<Loki::TypeInfo> GetHandledTypes() const;
    
    /// The optional delimiter separates the section and key name.  Do not
    /// escape the delimiter beyond what the C++ compiler requires.  The
    /// delimiter will be automagically escaped for use in a regexp.  E.g. use
    /// "." and not "\." or "\\."
    CGPAttrHandlerParsedRegistry( const std::string & registry_path,
                                  const std::string & delim = "." );

protected:
    typedef std::pair<std::string, std::string> TSectionNKey;
    
    TSectionNKey    ParseSection( const std::string & key ) const;
    
    std::string         m_Delim;
    CRef<CNcbiRegistry> m_Registry;
};


END_NCBI_SCOPE

#endif  //  GPIPE_COMMOM_GPIPE_ATTR_HANDLER_PARSED_REGISTRY_HPP__
