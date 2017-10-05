#pragma once

#include "util.h"
#include <memory>

#define UTILEX_BASE			0
#define UTILEX_STRING		1
#define UTILEX_SYSTEM		2
#define UTILEX_WIN			3
#define UTILEX_SCARD		4

class CBaseException
{
protected:
	virtual char *ExceptionName();
	virtual std::unique_ptr<CBaseException> Clone() const;

	const CBaseException& operator=(const CBaseException&);

public:
	CBaseException();
	CBaseException(int line,char *fileName);
	CBaseException(const CBaseException& inner);
	CBaseException(const CBaseException& inner,int line,char *fileName);
	virtual ~CBaseException(void);
	std::unique_ptr<CBaseException> innerException;

	void DumpTree(String &dump);
	virtual void DumpError(String &dump);
	int type;
	int line;
	String fileName;
};

class CStringException : public CBaseException
{
public:
	CStringException();
	CStringException(int line,char *fileName);
	CStringException(const CBaseException& inner);
	CStringException(const CBaseException& inner,int line,char *fileName);
	CStringException(int line,char *fileName,const char *fmt,...);
	CStringException(const char *fmt,...);
	CStringException(const CBaseException& inner,const char *fmt,...);
	CStringException(const CBaseException& inner,int line,char *fileName,const char *fmt,...);

	virtual ~CStringException();
	void DumpError(String &dump) override;
protected:
	char *ExceptionName() override;
	std::unique_ptr<CBaseException> Clone() const override;
	String description;
};

class CWinException : public CStringException
{
protected:
	virtual char *ExceptionName();
	void Init(DWORD error);
	std::unique_ptr<CBaseException> Clone() const override;
public:
	DWORD errorCode;
	CWinException ();
	CWinException (int line,char *fileName);
	CWinException (DWORD errorCode);
	CWinException (int line,char *fileName,DWORD errorCode);
	CWinException (const CBaseException& inner);
	CWinException (const CBaseException& inner,int line,char *fileName);
	CWinException (const CBaseException& inner,DWORD errorCode);
	CWinException (const CBaseException& inner,int line,char *fileName,DWORD errorCode);
};

class CSCardException : public CStringException
{
protected:
	virtual char *ExceptionName();
	void Init(WORD error);
	std::unique_ptr<CBaseException> Clone() const override;
public:
	WORD errorCode;
	CSCardException ();
	CSCardException (int line,char *fileName);
	CSCardException (WORD errorCode);
	CSCardException (int line,char *fileName,WORD errorCode);
	CSCardException (const CBaseException& inner,WORD errorCode);
	CSCardException (const CBaseException& inner,int line,char *fileName,WORD errorCode);

};

class CSystemException : public CStringException
{
protected:
	virtual char *ExceptionName();
	void Init(unsigned int error,LPCONTEXT context);
	unsigned int errorCode;
	std::unique_ptr<CBaseException> Clone() const override;
public:
	CSystemException ();
	CSystemException (unsigned int errorCode,LPCONTEXT context);

	String stackWalk;
	virtual void DumpError(String & dump);
};
