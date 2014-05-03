#ifndef ATTR_HANDLER_BASE_HPP__
#define ATTR_HANDLER_BASE_HPP__

/*
 * Authors:  Cheinan Marks
 *
 * File Description:
 *   Base class for the backend handlers.  Backend handlers must derive from
 *   this class and implement either Set, Get or both.  If the derived handler
 *   does not implement a method and it is called, a link error will result.
 */

#include <string>
#include <vector>

#include <boost/any.hpp>

#include "loki_type_info.hpp"
#include "any_property_exception.hpp"


class CAnyHandlerBase
{
public:
    CAnyHandlerBase() {}
    virtual ~CAnyHandlerBase() {}
    
    virtual boost::any Get( const std::string & /*key*/ ) const
    {
        throw CAnyPropertyException(CAnyPropertyException::eNoGet);
        return boost::any();
    }
    
    
    virtual void Set( const std::string & key, const boost::any & /*value*/ )
    {
        throw CAnyPropertyException(CAnyPropertyException::eNoSet);
    }
    
    /// Override with a method returning the name of the handler for error
    /// reporting.
    virtual std::string Name() const = 0;
    
    /// The overridden method must create and return a vector of all the types
    /// that the handler supports.  The helper function, CreateTypeVector, in
    /// TypelistHelper.hpp is very handy here.
    virtual std::vector<Loki::TypeInfo> GetHandledTypes() const = 0;

private:
    CAnyHandlerBase(const CAnyHandlerBase&);
    CAnyHandlerBase& operator=(const CAnyHandlerBase&);
};


#endif  //  ATTR_HANDLER_BASE_HPP__
