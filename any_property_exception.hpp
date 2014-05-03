#ifndef ANY_PROPERTY_EXCEPTION_HPP__
#define ANY_PROPERTY_EXCEPTION_HPP__

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


class CAnyPropertyException
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
	};
	
	CAnyPropertyException( unsigned errorCode, std::string extraInfo = "")
		: m_errorCode(errorCode), m_extraInfo(extraInfo) {}

	virtual std::string GetErrCodeString() const
	{
		std::string errorString;
		switch (GetErrCode()) {
			case eEmptyKey: errorString = "The key provided was empty";
			case eNoReadHandler: errorString = "No handler was installed to read this type";
			case eNoWriteHandler: errorString = "No handler was installed to write this type";
			case eBadReadType: errorString = "A handler found a type it couldn't write";
			case eBadWriteType: errorString = "A handler found a type it couldn't write";
			case eFileOpenFailed: errorString = "A file could not be opened";
			case eNoGet: errorString = "This handler does not implement Get";
			case eNoSet: errorString = "This handler does not implement Set";
			case eDBConnect: errorString = "The handler wasn't able to connect to a database";
			case eDBStatement: errorString = "A database statement faile.";
			case eDBNotUnique: errorString = "A database query didn't return exactly one response";
			default: errorString = "Unknown property exception with error code "
								+ std::to_string(GetErrCode());
			return errorString + ": " + m_extraInfo;
		}	
	}

	unsigned GetErrCode() const { return m_errorCode; }
	
private:
	unsigned m_errorCode;
	
protected:
	std::string m_extraInfo;
};


class CAnyPropertyNoKeyException : public CAnyPropertyException
{
public:
	enum EErrCode {
		eKeyNotFound
	};

	CAnyPropertyNoKeyException( unsigned errorCode, std::string keyName )
		: CAnyPropertyException(errorCode, keyName)
		, m_keyName(keyName) {}

	virtual std::string GetErrCodeString() const
	{	
		switch (GetErrCode()) {
			case eKeyNotFound: return "The key was not found in any handler: " + m_extraInfo;
			default:	 return CAnyPropertyException::GetErrCodeString();
		}
	}
	
	std::string GetKeyName() const { return m_keyName; }
	
private:
	std::string m_keyName;
};

#endif	// ANY_PROPERTY_EXCEPTION_HPP__
