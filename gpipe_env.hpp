#ifndef GPIPE_COMMOM_GPIPE_ATTR_HANDLER_GPIPE_ENV_HPP__
#define GPIPE_COMMOM_GPIPE_ATTR_HANDLER_GPIPE_ENV_HPP__

/*  $Id: gpipe_env.hpp 290454 2011-05-19 19:49:24Z marksc2 $
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
 *   This handler is the same as env.hpp except we specialize it to add the
 *   gpipe synonyms formerly stored in Perl.  E.g. sql.password is a
 *   synonym for GP_SQL_PSWD in the registry and environment.
 */

#include <string>
#include <map>

#include <corelib/ncbiobj.hpp>
#include <corelib/ncbienv.hpp>

#include <gpipe/common/property_handlers/env.hpp>
#include <gpipe/common/boost_any.hpp>
#include "base.hpp"


BEGIN_NCBI_SCOPE

class CGPAttrHandlerGPipeEnv : public CGPAttrHandlerEnv
{
public:
    virtual boost::any  Get( const std::string & key ) const;
    virtual void        Set( const std::string & key, const boost::any & value );

    virtual std::string Name() const;
    virtual std::vector<Loki::TypeInfo> GetHandledTypes() const;
    
    /// Reset the environment as needed by gpipe applications.
    /// Wipes the environment clean and sets up the variables
    /// gpipe needs.  Used by gp_sh and bootstrapping.
    void    ResetEnvironment( const std::string & entry_point );

    /// Attempts to adjust the environment variables to avoid conflicts
    /// with gpipe.  Otherwise does not change the envirnoment.
    /// Used by gpipe applications that assume bootstrapping has occurred.
    /// The function assumes a CGPipe
    void    StandardizeEnviornment();
    
    explicit CGPAttrHandlerGPipeEnv( const std::string & entry_point )
                : m_EntryPoint( entry_point )
                {}
    
private:
    void    x_SetCoreEnv();
    void    x_SetSgeEnv();
    void    x_SetCommonEnv();
    
    std::string m_EntryPoint;
    
    static const std::vector<std::string>   sc_WhiteList;
    static std::vector<std::string>    InitWhiteList();

    /// Attempts to infer the value for GP_HOME from context.
    std::string  x_GetGPipeHome();
};


END_NCBI_SCOPE

#endif  //  GPIPE_COMMOM_GPIPE_ATTR_HANDLER_GPIPE_ENV_HPP__
