#ifndef	__OLogPrivate__
#define	__OLogPrivate__

#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <QObject>
#include <QSharedPointer>
#include <QThread>
#include <QPair>
#include <QStack>
#include <QRegExp>
#include <QStringList>
#include <QDateTime>
#include <QHash>
#include <OLog.h>
//#include <unistd.h>
//#ifdef	__sun__
//#include <iostream>
//#endif

#define	O67_OLOG_NONPRINT_CHAR		'.'


//-------------------------------------------------------------------------
namespace	O67
{

//-------------------------------------------------------------------------
struct	OLogInfo
{
	const void*	pOwner;
	QString		FileName;
	long		LineNo;
	QString		Method;
};


typedef	QPair<OLog*, OLogInfo>	OLogPair;
typedef	QStack<OLogPair>		OLogStack;


//-------------------------------------------------------------------------
//	OLogPrivate	class
//-------------------------------------------------------------------------
class   OLogPrivate
	: public QObject
{
	Q_OBJECT;
	Q_DISABLE_COPY( OLogPrivate );
	static	QMutex	_ClassMutex;		//	do not use RcMutex here as it leads to deadlocks - RcMutex uses OLog as well
public:
	virtual	~OLogPrivate();

	static	OLogPrivate*	instance();

	virtual	void		syslogOnly();

	virtual	void		log( const QString& FileName, long LineNo, OLog::Level LogLevel, int Code, const QString& Message, const QVariant& Value = QVariant() );

	virtual	void		push( OLog* pLog, const void* pOwner, const QString& FileName, long LineNo, const QString& Method );
	virtual	OLogInfo	pop( OLog* pLog );
	virtual	OLogInfo	top();

	virtual	OLog::Level	currentLevel() const;
	virtual	void		setMasterLevel( OLog::Level NewLevel );

	virtual	void		attachLoggerDevice( const QProcess* const pLogger );

protected:
	OLogPrivate();

	virtual	void		log( const void* pOwner, const QString& FileName, long LineNo, OLog::Level LogLevel, int Code, const QString& Message, const QVariant& Value = QVariant() );

	virtual	QString		makeLogMessage( const QString& Message, const QVariant& Val );
	virtual	QString		levelText( OLog::Level Level );
	virtual	OLog::Level	initMasterLevel();
	virtual	int		nestLevel() const;
	virtual	QString		nestedName() const;
	virtual	QString		pointerValue( const void* pPointer ) const;
	virtual	QString		indentSpace() const;
	virtual	quint64		currentThreadId()  const;

protected	Q_SLOTS:
	virtual	void		readProcessLogger();

private:
	static	OLogPrivate		_Self;
	QHash<quint64, OLogStack* >	_ThreadMap;
	quint64				_Seq;
	OLog::Level			_MasterLevel;
	QDateTime			_LastLog;
	bool				_SyslogOnly;
	quint64				_LineCounter;

#if	QT_VERSION	>=	0x050000
	static	void		messageHandler( QtMsgType Type, const QMessageLogContext& Context, const QString& Message );
#else
	static	void		messageHandler( QtMsgType Type, const char* pMessage );
#endif

	virtual	void		coreLogger( const void* pOwner, const QString& FileName, int FileLine, OLog::Level LogLevel, int Code, const QString& Indent, const QString& Method, const QString& Message );
};


};	//	namespace	O67

#endif		//	__OLogPrivate__
