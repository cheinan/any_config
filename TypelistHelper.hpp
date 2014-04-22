#ifndef ANY_PROPERTY_TYPELIST_HELPER_HPP__
#define ANY_PROPERTY_TYPELIST_HELPER_HPP__

/*
 * File Description:
 *   Helper function that creates a vector of typeids out of its template arguments.
 */

#include <vector>
#include <typeinfo>

#include "loki_type_info.hpp"


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


#endif  //  ANY_PROPERTY_TYPELIST_HELPER_HPP__
