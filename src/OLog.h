
#ifndef	__OLog__
#define	__OLog__

#include <QSharedPointer>
#include <QProcess>
#include <QString>
#include <QMutex>
#include <QVariant>

//	see here why we need this:	http://doc.qt.io/qt-5/qmessagelogcontext.html
#ifndef	QT_MESSAGELOGCONTEXT
#define	QT_MESSAGELOGCONTEXT			1
#undef	QT_NO_MESSAGELOGCONTEXT
#endif


#define	O67_OLOG_DATA_SEPARATOR			"'"
#define	O67_OLOG_METHOD_SEPARATOR		":"


#define	O67_OLOG_CLASS_TXT_FATAL		"fatal"
#define	O67_OLOG_CLASS_TXT_ERROR		"error"
#define	O67_OLOG_CLASS_TXT_WARNING		"warn "
#define	O67_OLOG_CLASS_TXT_INFO			"info "
#define	O67_OLOG_CLASS_TXT_DEBUG		"debug"
#define	O67_OLOG_CLASS_TXT_TRACE		"trace"

#define	O67_OLOG_TAG_START			"-start-"
#define	O67_OLOG_TAG_END			"-end-"

#define	O67_OLOG_UNKNOWN			(-1)
//#define	O67_LOG_OK			0

#define	O67_OLOG_ENV_LOGLEVEL			"O67_LOG_LEVEL"

#define	O67_OLOG_CMD_ARGUMENT			"-v"


//----------------------------------------------------------------------------------
namespace	O67
{

//-----------------------------------------------------------------------------------
class	OLogPrivate;

//-----------------------------------------------------------------------------------
class	OLog
{
public:
	enum	Level
	{
		Fatal		= 0
		,Error		= 1
		,Default	= Error
		,Warning	= 2
		,Info		= 3
		,Debug		= 4
		,Trace		= 5
	};

	OLog( int& Argc, char** Argv, const QString& FileName, long LineNo, const QString& Method );
	OLog( const void* pOwner, const QString& FileName, long LineNo, const QString& Method );
	virtual	~OLog();

	virtual	void		log(	const QString& FileName, long LineNo, Level ErrClass, int ErrNo, const QString& Message = QString(), const QVariant& Value = QVariant() );
	virtual	void		trace(	const QString& FileName, long LineNo, const QString& Message = QString(), const QVariant& Value = QVariant() );
	virtual	void		debug(	const QString& FileName, long LineNo, const QString& Message = QString(), const QVariant& Value = QVariant() );
	virtual	void		debugPtr( const QString& FileName, long LineNo, const QString& Message, const void* pPointer );
	virtual	void		info(	const QString& FileName, long LineNo, const QString& Message = QString(), const QVariant& Value = QVariant() );
	virtual	void		warning(const QString& FileName, long LineNo, const QString& Message = QString(), const QVariant& Value = QVariant() );
	virtual	void		error(	const QString& FileName, long LineNo, const QString& Message = QString(), const QVariant& Value = QVariant() );
	virtual	void		fatal(	const QString& FileName, long LineNo, const QString& Message = QString(), const QVariant& Value = QVariant() );

	virtual	void		syslogOnly();

	virtual	void		attachLoggerDevice( const QProcess* const pLogger );

protected:

private:
};


//-----------------------------------------------------------------------------------
//	MACROS
//-----------------------------------------------------------------------------------
#if	QT_VERSION	>= 0x050000

#define	O67_OLOG_ASSERT_FATAL(expr)	\
		if (!(expr))	\
		{	\
			O67::OLog	Assert( NULL, __FILE__, __LINE__, __FUNCTION__ );	\
			\
			Assert.fatal( __FILE__, __LINE__, "Assertion", static_cast<bool>( (expr) ) );	\
		}

#else

#define	O67_OLOG_ASSERT_FATAL(expr)	\
		if (!(expr))	\
		{	\
			O67::OLog	Assert( NULL, __FILE__, __LINE__, __FUNCTION__ );	\
			\
			Assert.fatal( __FILE__, __LINE__, "Assertion", (expr) );	\
		}

#endif
		
};	//	namespace	O67

#endif	// __OLog_H__
