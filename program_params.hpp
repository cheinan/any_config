#ifndef GPIPE_COMMOM_GPIPE_ATTR_HANDLER_PROGRAM_PARAMETERS_HPP__
#define GPIPE_COMMOM_GPIPE_ATTR_HANDLER_PROGRAM_PARAMETERS_HPP__

/*  $Id: program_params.hpp 295284 2011-06-01 17:49:56Z marksc2 $
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
 *   A gpipe attribute backend that gets program parameters given a taskrun
 *   and program name.
 */

#include <string>
#include <memory>
#include <map>

#include <corelib/ncbiobj.hpp>

#include <dbapi/simple/sdbapi.hpp>

#include <gpipe/common/boost_any.hpp>
#include <gpipe/common/property_handlers/base.hpp>
#include <gpipe/common/gpipe_property.hpp>
#include <gpipe/common/gpipe_types.hpp>
#include <gpipe/common/property_handlers/gpinit_query_functors.hpp>


BEGIN_NCBI_SCOPE

class CGPPropertyHandlerProgramParam : public CGPAttrHandlerBase
{
public:
    CGPPropertyHandlerProgramParam( 
        const std::string & server,
        const std::string & database,
        const std::string & username,
        const std::string & password,
        TTaskrunId  taskrun_id = 0 );

    /// Get returns different types, depending on the recurrence
    /// count of the program.  If the program has a recurrence of
    /// 1 (as the vast majority of programs do), this function
    /// returns a vector of strings.  If the recurrence is more
    /// than 1, it returns a vector of vectors of strigs, each
    /// element of which contains the parameters for the corresponding
    /// recurrence of the program.
    virtual boost::any  Get( const std::string & program_name ) const;

    virtual std::string Name() const;
    virtual std::vector<Loki::TypeInfo> GetHandledTypes() const;

private:
    TTaskrunId  m_TaskrunId;
    
    mutable CDatabase   m_DatabaseConnection;
    mutable bool        m_IsDatabaseConnected;
    
    unsigned char   x_GetRecurrenceCount( const std::string & program_name ) const;
};


END_NCBI_SCOPE

#endif  //  GPIPE_COMMOM_GPIPE_ATTR_HANDLER_PROGRAM_PARAMETERS_HPP__
