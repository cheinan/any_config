#ifndef GPIPE_COMMOM_GPIPE_ATTR_TYPELIST_HELPER_HPP__
#define GPIPE_COMMOM_GPIPE_ATTR_TYPELIST_HELPER_HPP__

/*  $Id: TypelistHelper.hpp 174437 2009-10-27 20:15:00Z marksc2 $
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
 *   Helper function that creates a vector of typeids out of its template arguments.
 */

#include <vector>
#include <typeinfo>

#include <corelib/ncbiobj.hpp>

#include <internal/gpipe/common/loki_type_info.hpp>


BEGIN_NCBI_SCOPE

typedef std::vector<Loki::TypeInfo>   TTypeVector;

namespace {

class   CNullType {};

template <typename T> void   PushTypeIfNotNull( TTypeVector & type_list, T )
{
    if (typeid( CNullType ) != typeid( T ) ) {
        type_list.push_back( typeid( T ) );
    }
}

}   //  Blank namespace


/// Usage:  TTypeVector v = CreateTypeVector<int, double, std::string>()();    //  etc.
/// You can specify up to 12 types that will be entered in the vector.  If you need
/// more, then extend the function.
template <typename T1,
            typename T2 = CNullType,
            typename T3 = CNullType,
            typename T4 = CNullType,
            typename T5 = CNullType,
            typename T6 = CNullType,
            typename T7 = CNullType,
            typename T8 = CNullType,
            typename T9 = CNullType,
            typename T10 = CNullType,
            typename T11 = CNullType,
            typename T12 = CNullType >
class CreateTypeVector
{
public:
    std::vector<Loki::TypeInfo>  operator() ()
    {
        std::vector<Loki::TypeInfo> type_list;
        type_list.push_back( typeid( T1 ) );
        
        PushTypeIfNotNull( type_list, T2() );
        PushTypeIfNotNull( type_list, T3() );
        PushTypeIfNotNull( type_list, T4() );
        PushTypeIfNotNull( type_list, T5() );
        PushTypeIfNotNull( type_list, T6() );
        PushTypeIfNotNull( type_list, T7() );
        PushTypeIfNotNull( type_list, T8() );
        PushTypeIfNotNull( type_list, T9() );
        PushTypeIfNotNull( type_list, T10() );
        PushTypeIfNotNull( type_list, T11() );
        PushTypeIfNotNull( type_list, T12() );
        
        return  type_list;
    }
};


END_NCBI_SCOPE

#endif  //  GPIPE_COMMOM_GPIPE_ATTR_TYPELIST_HELPER_HPP__
