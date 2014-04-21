/*  $Id: test_gpipe_attr.cpp 312684 2011-07-15 00:59:52Z dicuccio $
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
 *   Regression test application for the GPipe attribute class.
 *
 */

#include <ncbi_pch.hpp>

#include <string>

#include <corelib/ncbiapp.hpp>
#include <corelib/ncbienv.hpp>
#include <corelib/ncbiargs.hpp>
#include <corelib/ncbistr.hpp>
#include <corelib/ncbi_system.hpp>

#include <gpipe/common/gpipe_attr.hpp>
#include <gpipe/common/property_handlers/gpinit_query_functors.hpp>
#include <gpipe/common/property_handlers/gpipe_corner_cases.hpp>
#include <gpipe/common/gpipe_types.hpp>
#include <gpipe/common/prog_param_facade.hpp>


USING_NCBI_SCOPE;


/////////////////////////////////////////////////////////////////////////////
//  CGPAttrTest::


class CGPAttrTest : public CNcbiApplication
{
private:
    virtual void Init(void);
    virtual int  Run(void);
    virtual void Exit(void);
};


class CAttrHolder
{
public:
    explicit CAttrHolder( const std::string & entry_point ) :
        m_BuildAttr( entry_point, CGPipeAttribute::eUseBuildId, 14 ),
        m_BuildrunAttr( entry_point, CGPipeAttribute::eUseBuildrunId, 17 ),
        m_TaskrunAttr( entry_point, CGPipeAttribute::eUseTaskrunId, 242 )
    {}
    
    template<typename T>
    void    PrintValues( const std::string & key ) const;
    void    PrintPairVector( const std::string & key ) const;
    void    PrintProgramParams( const std::string & key ) const;
    void    PrintDefaultTest() const;

private:    
    CGPipeAttribute m_BuildAttr;
    CGPipeAttribute m_BuildrunAttr;
    CGPipeAttribute m_TaskrunAttr;
};


template<typename T>
void    CAttrHolder::PrintValues( const std::string & key ) const
{
    std::string synonym_key = CGPHSynonymPair::GetSynonym( key );

    try {
        LOG_POST( Info << key << " from build: "
                    << m_BuildAttr.Get<T>( key ) );
    } catch ( CGPipeAttributeException & e ) {
        LOG_POST( Error << key << " from build Exception: " << e.what() );
    }
    
    try {
        LOG_POST( Info << key << " from buildrun: "
                    << m_BuildrunAttr.Get<T>( key ) );
    } catch ( CGPipeAttributeException & e ) {
        LOG_POST( Error << key << " from buildrun Exception: " << e.what() );
    }

    try {
        LOG_POST( Info << key << " from taskrun: "
                    << m_TaskrunAttr.Get<T>( key ) );
    } catch ( CGPipeAttributeException & e ) {
        LOG_POST( Error << key << " from taskrun Exception: " << e.what() );
    }
    
    if ( ! synonym_key.empty() ) {
        try {
            LOG_POST( Info << synonym_key << " from build: "
                << m_BuildAttr.Get<T>( synonym_key ) );
        } catch ( CGPipeAttributeException & e ) {
            LOG_POST( Error << synonym_key << " from build Exception: " << e.what() );
        }
    
        try {
            LOG_POST( Info << synonym_key << " from buildrun: "
                << m_BuildrunAttr.Get<T>( synonym_key ) );
        } catch ( CGPipeAttributeException & e ) {
            LOG_POST( Error << synonym_key << " from buildrun Exception: " << e.what() );
        }
    
        try {
            LOG_POST( Info << synonym_key << " from taskrun: " 
                << m_TaskrunAttr.Get<T>( synonym_key ) );
        } catch ( CGPipeAttributeException & e ) {
            LOG_POST( Error << synonym_key << " from taskrun Exception: " << e.what() );
        }
    }
}


void    CAttrHolder::PrintPairVector( const std::string & key ) const
{
    typedef std::vector<std::pair<std::string, std::string> >   TPairVector;
    TPairVector pair_vector = m_TaskrunAttr.Get<TPairVector> ( key );
    LOG_POST( Info << key << " returns a vector of pairs of strings with "
                << pair_vector.size() << " elements." );
    ITERATE( TPairVector, iter, pair_vector ) {
        LOG_POST( Info << "    Role: " << iter->first << " Data Type: "
                    << iter->second );
    }
}


void    CAttrHolder::PrintProgramParams( const std::string & key ) const
{
    CGPProgramParameterFacade   prog_param
        = m_TaskrunAttr.Get<CGPProgramParameterFacade>( key );
    unsigned int    recurrence_count = prog_param.GetRecurrenceCount();
    LOG_POST( Info << "Recurrence count = " << recurrence_count );
    
    CGPProgramParameterFacade::TCommandLineVector   command_line
        = prog_param.GetCommandLine();
    for ( unsigned int i = 0; i < command_line.size(); i++ ) {
        std::cout << command_line[i] << " ";
    }
    std::cout << '\n';
    
    CGPProgramParameterFacade::TParameterMap   param_map
        = prog_param.GetParameterMap();
    CGPProgramParameterFacade::TParameterMap::const_iterator    param_map_iter
        = param_map.begin();
    while ( param_map_iter != param_map.end() ) {
        LOG_POST( Info << param_map_iter->first << ": " <<param_map_iter->second );
        ++param_map_iter;
    }
}


void    CAttrHolder::PrintDefaultTest() const
{
    LOG_POST( Info << m_BuildAttr.GetWithDefault( "Does.Not.Exist", "Default" ) );
}


/////////////////////////////////////////////////////////////////////////////
//  Init test for all different types of arguments


void CGPAttrTest::Init(void)
{
    // Create command-line argument descriptions class
    auto_ptr<CArgDescriptions> arg_desc(new CArgDescriptions);

    // Specify USAGE context
    arg_desc->SetUsageContext(GetArguments().GetProgramBasename(),
                              "GPipe Attribute Prototype");

    arg_desc->AddKey( "entry", "GPipeEntryPoint",
                            "GPipe entry point.  If omitted, the environment"
                            "var GP_ep is used",
                            CArgDescriptions::eString );

    // Setup arg.descriptions for this application
    SetupArgDescriptions(arg_desc.release());
}



/////////////////////////////////////////////////////////////////////////////

int CGPAttrTest::Run(void)
{
    CArgs args = GetArgs();
    std::string entry_point = args["entry"].AsString();
    
    CAttrHolder attr_holder( entry_point );
    
    attr_holder.PrintValues<std::string>( "entry-point.name" );
    attr_holder.PrintValues<std::string>( "GP_MAGIC" );
    attr_holder.PrintValues<std::string>( "MACHTYPE" );
    attr_holder.PrintValues<std::string>( "PATH" );
    attr_holder.PrintValues<std::string>( "TMPDIR" );
    attr_holder.PrintValues<std::string>( "NCBI" );
    attr_holder.PrintValues<std::string>( "SYBASE" );
    attr_holder.PrintValues<std::string>( "PERL5LIB" );
    attr_holder.PrintValues<std::string>( "PYTHONPATH" );
    attr_holder.PrintValues<TGPipeId>( "build.id" );
    attr_holder.PrintValues<std::string>( "build.org.scientific_name" );
    attr_holder.PrintValues<std::string>( "build.org.gb_division" );
    attr_holder.PrintValues<std::string>( "build.org.gb_division_abbr" );
    attr_holder.PrintValues<TGPipeId>( "build.org.taxid" );
    attr_holder.PrintValues<std::string>( "build.ext_display_name" );
    attr_holder.PrintValues<std::string>( "build.paths.root" );
    attr_holder.PrintValues<std::string>( "build.paths.official_output" );
    attr_holder.PrintValues<TGPipeId>( "buildrun.id" );
    attr_holder.PrintValues<std::string>( "buildrun.paths.root" );
    attr_holder.PrintValues<std::string>( "buildrun.paths.sequence_cache" );
    attr_holder.PrintValues<std::string>( "buildrun.paths.gencoll_release" );
    attr_holder.PrintValues<std::string>( "task.type" );
    attr_holder.PrintValues<TGPipeId>( "taskrun.id" );
    attr_holder.PrintValues<std::string>( "taskrun.paths.root" );
    attr_holder.PrintValues<std::string>( "build.org.display_name" );
    attr_holder.PrintValues<std::string>( "build.org.species_taxid" );
    attr_holder.PrintValues<std::string>( "build.org.taxid_exact_fl" );
    attr_holder.PrintValues<std::string>( "build.org.common_name" );
    attr_holder.PrintValues<std::string>( "sql.server" );
    attr_holder.PrintValues<std::string>( "sql.user" );
    attr_holder.PrintValues<std::string>( "sql.password" );
    attr_holder.PrintValues<std::string>( "sql.gencoll_server" );
    attr_holder.PrintValues<std::string>( "sql.gencoll_user" );
    attr_holder.PrintValues<std::string>( "sql.gencoll_password" );
    attr_holder.PrintValues<TGPipeId>( "netschedule.monitor_sleep" );
    attr_holder.PrintValues<std::string>( "gpqueue.sge_queue" );
    attr_holder.PrintValues<std::string>( "gpqueue.exclusive" );
    attr_holder.PrintPairVector( "task.inputs" );
    attr_holder.PrintPairVector( "task.outputs" );
    attr_holder.PrintDefaultTest();
    attr_holder.PrintProgramParams( "align_filter" );
    
    return 0;
}


/////////////////////////////////////////////////////////////////////////////
//  Cleanup


void CGPAttrTest::Exit(void)
{
    SetDiagStream(0);
}


/////////////////////////////////////////////////////////////////////////////
//  MAIN


int main(int argc, const char* argv[])
{
    // Execute main application function
    return CGPAttrTest().AppMain(argc, argv);
}
