#ifndef PROPERTY_EXCEPTION_HPP__
#define PROPERTY_EXCEPTION_HPP__

#include <string>

/*
 * This code is based on public domain code from the National Library of
 * Medicine at the US National Institutes of Health
 *
 * Author:	 Cheinan Marks
 *
 * File Description:
 *	 Properties exception class.
 */


class CPropertyException
{
public:
	enum EErrCode {
		eEmptyKey
		, eNoReadHandler
		, eNoWriteHandler
		, eBadReadType
		, eBadWriteType
		, eFileOpenFailed
		, eNoGet
		, eNoSet
		, eDBConnect
		, eDBStatement
		, eDBNotUnique
		, eNoEntryPointInEnv
		, eBadEntryPoint
		, eHomeless
	};
	
	CPropertyException( unsigned errorCode ) : m_errorCode(errorCode) {}

	virtual std::string GetErrCodeString() const
	{	
		switch (GetErrCode()) {
			case eEmptyKey: return "The key provided was empty.";
			case eNoReadHandler: return "No handler was installed to read this type";
			case eNoWriteHandler: return "No handler was installed to write this type";
			case eBadReadType: return "A handler found a type it couldn't write.";
			case eBadWriteType: return "A handler found a type it couldn't write.";
			case eFileOpenFailed: return "A file could not be opened.";
			case eNoGet: return "This handler does not implement Get.";
			case eNoSet: return "This handler does not implement Set.";
			case eDBConnect: return "The handler wasn't able to connect to a database.";
			case eDBStatement: return "A database statement failed.";
			case eDBNotUnique: return "A database query didn't return exactly one response.";
			case eNoEntryPointInEnv: return "You must set the entry point environment variable, "
											"GP_ep, to point at a configuration file.";
			case eBadEntryPoint: return "The configuration file couldn't be opened.";
			case eHomeless: return "GP_HOME must be set in the OS environment.";
			default: return "Unknown property exception with error code "
								+ std::to_string(GetErrCode());
		}	
	}

	unsigned GetErrCode() const { return m_errorCode; }
	
private:
	unsigned m_errorCode;	
};


class CPropertyNoKeyException : public CPropertyException
{
public:
	enum EErrCode {
		eKeyNotFound
	};

	CPropertyNoKeyException( unsigned errorCode ) : CPropertyException(errorCode) {}

	virtual std::string GetErrCodeString() const
	{	
		switch (GetErrCode()) {
			case eKeyNotFound: return "The key was not found in any handler.";
			default:	 return CPropertyException::GetErrCodeString();
		}
	}
};

#endif	// PROPERTY_EXCEPTION_HPP__
