/*  $Id: gph_program_params.cpp 310374 2011-06-30 13:27:36Z dicuccio $
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
 *   A gpipe attribute backend that fetches program parameters given a
 *   taskrun ID and progrma name.  Returns the
 */

#include <ncbi_pch.hpp>

#include <string>
#include <map>

#include <corelib/ncbiobj.hpp>

#include <gpipe/common/boost_any.hpp>
#include <gpipe/common/TypelistHelper.hpp>
#include <gpipe/common/gpipe_types.hpp>
#include <gpipe/common/gpipe_property.hpp>
#include <gpipe/common/gpipe_property_exception.hpp>
#include <gpipe/common/property_handlers/gpipe_corner_cases.hpp>
#include <gpipe/common/security.hpp>
#include <gpipe/common/prog_param_facade.hpp>
#include <gpipe/common/property_handlers/gpinit_query.hpp>
#include <gpipe/common/property_handlers/program_params.hpp>

BEGIN_NCBI_SCOPE


CGPPropertyHandlerProgramParam::CGPPropertyHandlerProgramParam( 
                            const std::string & server,
                            const std::string & database,
                            const std::string & username,
                            const std::string & encrypted_password_file,
                            TTaskrunId  taskrun_id )
    : m_TaskrunId( taskrun_id ), m_IsDatabaseConnected( false )
{
    CCertificateRepository  certificate_repository;
    std::string database_URI
        = CGPAttrHandlerGPInitQuery::ToURI(
            server,
            database,
            username,
            certificate_repository.GetPassword( encrypted_password_file ) );
    m_DatabaseConnection = CDatabase( database_URI );
}


boost::any  CGPPropertyHandlerProgramParam::Get( const std::string & program_name ) const
{
    boost::any  result;
    
    if ( ! m_IsDatabaseConnected ) {
        m_DatabaseConnection.Connect();
        m_IsDatabaseConnected = true;
    }

    CQuery  query = m_DatabaseConnection.NewQuery();

    unsigned char    recurrence_count = x_GetRecurrenceCount(program_name);
    LOG_POST( Info << static_cast<unsigned int>(recurrence_count) );
    std::vector<CGPProgramParameterFacade::TCommandLineVector>
        command_lines( recurrence_count );
    std::vector<CGPProgramParameterFacade::TParameterMap>
        param_maps( recurrence_count );
    CGPProgramParameterFacade::TParameterMap    a_param_map;

    query.SetParameter( "@taskrun_id", static_cast<Int4>( m_TaskrunId ) );
    query.SetParameter( "@prog_name", program_name );
    query.SetParameter( "@use_by", std::string() );
    for ( unsigned char i = 0; i < recurrence_count; i++ ) {
        query.SetParameter( "@prog_recur", i + 1 );
        
        query.SetParameter( "@2cols", 0 );
        query.ExecuteSP( "taskrun_get_paramlist" );
        CQuery::CRowIterator row_iterator = query.begin();
        while( row_iterator != query.end() ) {
            command_lines[i].push_back( row_iterator[1].AsString() );
            LOG_POST( Info << row_iterator[1].AsString() );
            ++row_iterator;
        }
        
        query.SetParameter( "@2cols", 1 );
        query.ExecuteSP( "taskrun_get_paramlist" );
        row_iterator = query.begin();
        while( row_iterator != query.end() ) {
            a_param_map[row_iterator["param_name"].AsString()] =
                row_iterator["val"].AsString();
            ++row_iterator;
        }
        param_maps[i] = a_param_map;
        a_param_map.clear();
    }

    CGPProgramParameterFacade   parameter_facade( program_name,
                                                    command_lines,
                                                    param_maps );
    result = parameter_facade;

    return  result;
}


unsigned char  CGPPropertyHandlerProgramParam::x_GetRecurrenceCount( const std::string & program_name ) const
{
    std::string recurrence_sql = "SELECT MAX( prog_recur ) "
        "FROM ParamSetValue v "
        "JOIN ProgramParamHist p ON p.hist_cd != 'D' AND v.param_id=p.param_id "
        "JOIN TaskRun s ON s.paramset_id=v.paramset_id "
        "WHERE s.taskrun_id=" + NStr::IntToString(m_TaskrunId) +
        " AND p.prog_name='" + program_name + "' ";

    CQuery  query = m_DatabaseConnection.NewQuery();
    query.SetSql( recurrence_sql );
    query.Execute();

    query.SingleSet();
    CQuery::CRowIterator row_iterator = query.begin();
    return row_iterator[1].AsInt4();
}

std::string CGPPropertyHandlerProgramParam::Name() const
{
    return  "Get Program Parameters";
}


std::vector<Loki::TypeInfo> CGPPropertyHandlerProgramParam::GetHandledTypes() const
{
    return  CreateTypeVector<CGPProgramParameterFacade>()();
}


END_NCBI_SCOPE
