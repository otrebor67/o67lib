
#include <OLog_p.h>


//-----------------------------------------------------------------------------------
QMutex			O67::OLogPrivate::_ClassMutex( QMutex::Recursive );
O67::OLogPrivate	O67::OLogPrivate::_Self;


//---------------------------------------------------------------------------------------------------------------------------
///	the message handler is Qt's way of allowing to catch all qDebug(), qWarning(), and QCritical() messages
///	we install one to catch all of those and to redirect them for consistency to our log facility
#if	QT_VERSION	>=	0x050000
void
O67::OLogPrivate::messageHandler( QtMsgType Type, const QMessageLogContext& Context, const QString& Message )
{
	QMutexLocker	Locker( &_ClassMutex );

	switch( Type )
	{
		case	QtDebugMsg:	_Self.log( NULL, Context.file, Context.line, OLog::Debug,	O67_OLOG_UNKNOWN, Message );	break; 
		case	QtWarningMsg:	_Self.log( NULL, Context.file, Context.line, OLog::Warning,	O67_OLOG_UNKNOWN, Message );	break;
		case	QtCriticalMsg:	_Self.log( NULL, Context.file, Context.line, OLog::Error,	O67_OLOG_UNKNOWN, Message );	break;
		case	QtFatalMsg:
		default:
		{
			_Self.log( NULL, Context.file, Context.line, OLog::Fatal, O67_OLOG_UNKNOWN, Message );
		}
	}
}
#else
void
O67::OLogPrivate::messageHandler( QtMsgType Type, const char* pMessage )
{
	QMutexLocker	Locker( &_ClassMutex );
	
	switch( Type )
	{
		//	as Qt does not provide us the file/line location of the message (in qt4) we take the ones we know
		case	QtDebugMsg:	_Self.log( NULL, __FILE__, __LINE__, OLog::Debug,	O67_OLOG_UNKNOWN, QString::fromLatin1( pMessage ) );	break; 
		case	QtWarningMsg:	_Self.log( NULL, __FILE__, __LINE__, OLog::Warning,	O67_OLOG_UNKNOWN, QString::fromLatin1( pMessage ) );	break;
		case	QtCriticalMsg:	_Self.log( NULL, __FILE__, __LINE__, OLog::Error,	O67_OLOG_UNKNOWN, QString::fromLatin1( pMessage ) );	break;
		case	QtFatalMsg:
		default:
		{
			_Self.log( NULL, __FILE__, __LINE__, OLog::Fatal, O67_OLOG_UNKNOWN, QString::fromLatin1( pMessage ) );
		}
	}
}
#endif


//	whenever a new OLog() class is allocated - usually at the entry point of a method/function - push is called and puts 
//	the relevant information onto a log stack. This will allow us to prvide the logger history and nestlevel this function or method is in.
//	In addition, it allows us to retreive information of the current code position when arbitrary messages are being logged.
//	\param	pLog		OLog object pointer
//	\param	pOwner		the pointer of the owning object that owns the OLog object - this allows to retreive object specific messages from the log
//	\param	FileName	the sourcecode filename where this is being logged - usually the cpp macro __FILE__
//	\param	LineNo		the sourcecode line where this is being logged - usually the cpp macro __LINE__
//	\param	Method		the sourcecode method where this is being logged - usually the cpp macro __FUNCTION__
//				do not use gnu/cpp's macro __PRETTY_FUNCTION__ as it is not portable and prints by far too much information which we already
//				have with the filenam/line combination
void
O67::OLogPrivate::push( OLog* pLog, const void* pOwner, const QString& FileName, long LineNo, const QString& Method )
{
	QMutexLocker	Locker( &_ClassMutex );
	OLogStack*	pStack	= NULL;
	OLogInfo	LogInfo;
	OLogPair	LogPair;

	if ( ! pLog )
		::abort();

	LogInfo.pOwner		= pOwner;
	LogInfo.FileName	= FileName;
	LogInfo.LineNo		= LineNo;
	LogInfo.Method		= Method;

	LogPair	= qMakePair( pLog, LogInfo );

	if ( _ThreadMap.contains( this->currentThreadId() ) )
		pStack	= _ThreadMap.value( this->currentThreadId() );
	else
		pStack	= new OLogStack();

	pStack->push( LogPair );

	_ThreadMap.insert( this->currentThreadId(), pStack );

	this->log( LogInfo.pOwner, LogInfo.FileName, LogInfo.LineNo, OLog::Trace, O67_OLOG_UNKNOWN, O67_OLOG_TAG_START );
}


///	pops a log info struct from the logger stack
///	this is usually the case when an OLog object goes out of scope (its destructor is called)
///	\param	pLog	the logger object where this info bengs to
///	\return		returns the OLogInfo struct that has been taken (popped) from the stack
O67::OLogInfo
O67::OLogPrivate::pop( OLog* pLog )
{
	QMutexLocker	Locker( &_ClassMutex );
	OLogStack*	pStack	= NULL;
	OLogInfo	LogInfo;
	OLogPair	LogPair;

	LogInfo	= this->top();

	//	log that an object goes out of scope
	this->log( LogInfo.pOwner, LogInfo.FileName, LogInfo.LineNo, OLog::Trace, O67_OLOG_UNKNOWN, O67_OLOG_TAG_END );

	if ( _ThreadMap.contains( this->currentThreadId() ) )
		pStack	= _ThreadMap.value( this->currentThreadId() );

	//	this will crash if we try to pop from a thread we do not know
	//	this is exactly what we want as there is seriously something wrong with
	//	the programmers handling of this class
	LogPair	= pStack->pop();

	if ( LogPair.first == pLog )
		LogInfo	= LogPair.second;

	return	LogInfo;
}


///	takes the top OLogInfo object from the stack and leave it there
///	\return		returns the OLogInfo struct that has been referenced from the top of the calling stack
O67::OLogInfo
O67::OLogPrivate::top()
{
	QMutexLocker	Locker( &_ClassMutex );
	OLogStack*	pStack	= NULL;
	OLogInfo	LogInfo;

	if ( _ThreadMap.contains( this->currentThreadId() ) )
		pStack	= _ThreadMap.value( this->currentThreadId() );
	else
	{
		this->log( NULL, __FILE__, __LINE__, OLog::Fatal, O67_OLOG_UNKNOWN, O67_OLOG_TAG_END );
		::abort();	// we won't reach this, but to make things clear that this should not ever happen
	}

	LogInfo	= pStack->top().second;

	return	LogInfo;
}


///	returns the current code nestlevel - as far as OLog objects are concerned
///	this cannot be true when other subroutines (e.g from third party libraries) are called which do not use
///	the OLog logger facility
///	\return		number of nestlevel of the call stack
int
O67::OLogPrivate::nestLevel() const
{
	if ( _LineCounter > 0 )
		QMutexLocker	Locker( const_cast<QMutex*>( &_ClassMutex ) );

	int		NestLevel	= 0;
	OLogStack*	pStack	= NULL;

	if ( _ThreadMap.contains( this->currentThreadId() ) )
		pStack	= _ThreadMap.value( this->currentThreadId() );

	if ( pStack )
		NestLevel	= pStack->count();

	//	safety measure ;-)
	if ( NestLevel < 0 )
		NestLevel	= 0;

	return	NestLevel;
}


///	retreive the nested name of the current method
///	\return		the name of the current method we are in
QString
O67::OLogPrivate::nestedName() const
{
	QMutexLocker	Locker( const_cast<QMutex*>( &_ClassMutex ) );
	QString		NestedName;
	OLogStack*	pStack	= NULL;
	
	if ( _ThreadMap.contains( this->currentThreadId() ) )
	{
		OLogPair	LogPair;

		pStack		= _ThreadMap.value( this->currentThreadId() );
		
//		//	activate this code sequence if you want the full call-path displayed in the logs
//		for ( int i = 0; i < pStack->count(); ++i )
//		{
//			LogPair	= pStack->at( i );
//
//			if ( ! NestedName.isEmpty() )
//				NestedName	+= O67_OLOG_METHOD_SEPARATOR;
//
//			NestedName	+= LogPair.second.Method;
//		}

		//	activate this code sequence if you want only the last method displayed in the log
		//	this is by far less i/o intense as the same information as above can be gathered by looking at the log sequence in trace mode
		LogPair	= pStack->at( pStack->count() - 1 );

		if ( ! NestedName.isEmpty() )
			NestedName	+= O67_OLOG_METHOD_SEPARATOR;

		NestedName	+= LogPair.second.Method;
	}
	else
		NestedName	= "-unknown-"; 

	return	NestedName;
}


///	maps log leves to a matching text for the user
///	\return		useful text mark that matches the log level
QString
O67::OLogPrivate::levelText( OLog::Level Level )
{
	if ( _LineCounter > 0 )
		QMutexLocker	Locker( &_ClassMutex );

	QString		LevelTxt;

	switch( Level )
	{
		case	OLog::Fatal:	LevelTxt	= O67_OLOG_CLASS_TXT_FATAL;	break;
		case	OLog::Error:	LevelTxt	= O67_OLOG_CLASS_TXT_ERROR;	break;
		case	OLog::Warning:	LevelTxt	= O67_OLOG_CLASS_TXT_WARNING;	break;
		case	OLog::Info:	LevelTxt	= O67_OLOG_CLASS_TXT_INFO;		break;
		case	OLog::Debug:	LevelTxt	= O67_OLOG_CLASS_TXT_DEBUG;	break;
		case	OLog::Trace:	LevelTxt	= O67_OLOG_CLASS_TXT_TRACE;	break;

		default:
					LevelTxt	= O67_OLOG_CLASS_TXT_DEBUG;	break;
	}

	return	LevelTxt;
}


///	returns the current log level that has been set
///	return		current log level
O67::OLog::Level
O67::OLogPrivate::currentLevel() const
{
	QMutexLocker	Locker( const_cast<QMutex*>( &_ClassMutex ) );

	return	_MasterLevel;
}


///	set the current master log level
///	\param	the new log level
void
O67::OLogPrivate::setMasterLevel( OLog::Level NewLevel )
{
	QMutexLocker	Locker( &_ClassMutex );

	_MasterLevel	= NewLevel;
}


///	initializes the master log level on startup
///	this will be taken from an environment variable named O67_OLOG_LEVEL 
///	if not existing, the default log level is set
///	\return		initialized log level
O67::OLog::Level
O67::OLogPrivate::initMasterLevel()
{
	QMutexLocker	Locker( &_ClassMutex );
	QString		EnvValue( getenv( O67_OLOG_ENV_LOGLEVEL ) );
	int		Level	= OLog::Default;
	bool		Ok;

	if ( EnvValue.isEmpty() )
		EnvValue.setNum( Level );

	Level	= EnvValue.toInt( &Ok );
	if ( ! Ok )
	{
		qFatal( QString("Invalid log level %1 specified").arg( EnvValue ).toLatin1() );
	}

	if ( Level > OLog::Trace )
		Level	= OLog::Trace;

	if ( Level < OLog::Error )
		Level	= OLog::Error;

	return	static_cast<OLog::Level>( Level );
}


///	combines the message and the value into one message suitable for logging
///	return	the combined message and value string
QString
O67::OLogPrivate::makeLogMessage( const QString& Message, const QVariant& Val )
{
	if ( _LineCounter > 0 )
		QMutexLocker	Locker( &_ClassMutex );

	QString		Info;

	if ( Val.isValid() )
	{
		Info	=	QString( "%1:%2%3%4" )
				.arg( Message )
				.arg( O67_OLOG_DATA_SEPARATOR )
				.arg( Val.toString() )
				.arg( O67_OLOG_DATA_SEPARATOR );
	}
	else
	{
		Info	= 	QString( "%1" ).arg( Message );
	}
	
	return	Info;
}


///	returns the current technical id of the thread we are in casted in a platform neutral way
///	the purpose of this id is not to be if technical uniqueness but only to have an id to
///	distinguish threads from each other in the call stack
///	\return		the thread id in 64bit integer
quint64
O67::OLogPrivate::currentThreadId() const
{
	QMutexLocker	Locker( &_ClassMutex );
	Qt::HANDLE	THandle	= QThread::currentThreadId(); 

#if	QT_VERSION	>=	0x050000
	#if	( QT_POINTER_SIZE == 4 )
		return	reinterpret_cast<quint32>( THandle );
	#else
		return	reinterpret_cast<quint64>( THandle );
	#endif
#else
	return	THandle;
#endif
}


///	transforms a pointer into a human readable hex string 
///	return	the pointer as human readable hex string with constant width, right aligned
QString
O67::OLogPrivate::pointerValue( const void* pPointer ) const
{
//	QMutexLocker	Locker( &_ClassMutex );
	QString		PointerValue;

#if	( QT_POINTER_SIZE == 4 )
	PointerValue.sprintf( "%10p", pPointer );
#else
	PointerValue.sprintf( "%18p", pPointer );
#endif

	return	PointerValue;
}


///	produces a string that is as long as the current call stack nest level
///	\return	string with length like call stack nest level
QString
O67::OLogPrivate::indentSpace() const
{
	QMutexLocker	Locker( &_ClassMutex );
	QString		IndentSpace;

	while ( IndentSpace.count() < this->nestLevel() )
		IndentSpace	+= O67_OLOG_NONPRINT_CHAR; 

	return	IndentSpace;
}


///	logger helper method
void
O67::OLogPrivate::log( const QString& FileName, long LineNo, OLog::Level LogLevel, int Code, const QString& Message, const QVariant& Value )
{
	QMutexLocker	Locker( &_ClassMutex );
	OLogInfo	Info	= this->top();

	this->log( Info.pOwner, FileName, LineNo, LogLevel, Code, Message, Value );
}


///	logger helper method
///	if the log level is OLog::Fatal the call will not return, but exit with an exit code of EXIT_FAILURE
void
O67::OLogPrivate::log( const void* pOwner, const QString& FileName, long LineNo, OLog::Level LogLevel, int Code, const QString& Message, const QVariant& Value )
{
	if ( _LineCounter > 0 )
	{
		//	only lock this if we are beyond constructor phase: 0 means we are being called by the constructor - don't lock as it deadlocks
		QMutexLocker	Locker( &_ClassMutex );
	}

	this->coreLogger( pOwner, FileName, LineNo, LogLevel, Code, this->indentSpace(), this->nestedName(), this->makeLogMessage( Message, Value ) );

	if ( LogLevel <= OLog::Fatal )
	{
		::exit( EXIT_FAILURE );		//	allow stdout/stderr buffers to be flushed
//		::abort();
	}
}


///	call this method if you do not want log messages to als go to stderr but only to syslog instead
void
O67::OLogPrivate::syslogOnly()
{
	QMutexLocker	Locker( &_ClassMutex );

	_SyslogOnly	= true;
}


///	obtian the global singleton instance of the core logger: OLogPrivate instance
///	\return	pointer to the core logger instance
O67::OLogPrivate*
O67::OLogPrivate::instance()
{
	//	singleton pattern with double checked locking
	//	see here for more inforamtion: http://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf
//	if ( ! _Self )
//	{
		QMutexLocker	Locker( &_ClassMutex );

//		if ( ! _Self )
//			_Self	= new	OLogPrivate();
//	}

	return	&_Self;
}


///	read the logger output form a sub-process and forward it to stderr
///	no re-formatting is performed and the data is forwarded as-is
void
O67::OLogPrivate::readProcessLogger()
{
	QMutexLocker	Locker( &_ClassMutex );
	QProcess*	pProcess	= qobject_cast<QProcess*>( this->sender() );

	if ( ! pProcess )
		return;

	QString	Buffer( pProcess->readAllStandardError() ); 

	//	we just print this as we get it to stderr, as we assume the data has already been pre-formatted as we need it
	fprintf( stderr, "%s", Buffer.toLatin1().data() );
}


///	attach a sub-process to the same logger device as this process has
void
O67::OLogPrivate::attachLoggerDevice( const QProcess* const pLogger )
{
	QMutexLocker	Locker( &_ClassMutex );

	if ( ! pLogger  )
		return;

	if ( ! 	this->connect(	pLogger,	SIGNAL(	readyReadStandardError()	),
				this,		SLOT(	readProcessLogger()		)	)	)
	{
		::exit( EXIT_FAILURE );
		::abort();
	}
}


///	core logger which writes the information out to stderr and to syslog if criterias match
///	igores the call if the current log level says to ignore some messages
void
O67::OLogPrivate::coreLogger( const void* pOwner, const QString& FileName, int FileLine, OLog::Level LogLevel, int Code, const QString& Indent, const QString& Method, const QString& Message )
{
	QMutexLocker	Locker( &_ClassMutex );

	if ( LogLevel > this->currentLevel() )
		return;

	QString		LogMessage;
	QDateTime	Dt( QDateTime::currentDateTime() );
	QString		TimeStamp( Dt.toString( "yyyyMMdd-hhmmsszzz" ) );
	int		SyslogLevel	= LOG_ERR;
	QString		NameOfFile	= FileName;
	int		Pos		= FileName.lastIndexOf( '/' );

	if ( Pos >= 0 )
		NameOfFile	= FileName.right( FileName.length() - Pos - 1 );

	if ( _LastLog != Dt )
		_Seq	= 0;

	LogMessage	= QString( "%1%2:T%3:%4:%5:%6:%7:%8[%9]:%10:%11" )
				.arg( TimeStamp )
				.arg( _Seq, 3, 10, QChar( '0' ) )
				.arg( this->currentThreadId(), 0, 36 )			//	writes something like this: T1dq27wzg3k
				.arg( this->levelText( LogLevel ) )
				.arg( this->pointerValue( pOwner ) )
				.arg( Code )
				.arg( Indent )
				.arg( NameOfFile )
				.arg( FileLine )
				.arg( Method )
				.arg( Message );

	switch( LogLevel )
	{
		case	OLog::Fatal:	SyslogLevel	= LOG_CRIT;	break;
		case	OLog::Error:	SyslogLevel	= LOG_ERR;	break;
		case	OLog::Warning:	SyslogLevel	= LOG_WARNING;	break;
		case	OLog::Info:	SyslogLevel	= LOG_NOTICE;	break;
		case	OLog::Debug:	SyslogLevel	= LOG_INFO;	break;
		case	OLog::Trace:	SyslogLevel	= LOG_DEBUG;	break;

		default:		SyslogLevel	= LOG_ERR;	break;
	}

	if ( _SyslogOnly )
		::syslog( SyslogLevel, "%s", LogMessage.toLatin1().data() );
	else
	{
		//	if we log to stderr then only send critical and error messages to the syslog
		if (	SyslogLevel	== LOG_CRIT	||
			SyslogLevel	== LOG_ERR	)
		{
			::syslog( SyslogLevel, "%s", LogMessage.toLatin1().data() );
		}

		fprintf( stderr, "%s\n", LogMessage.toLatin1().data() );
	}

	++_Seq;
	_LastLog	= Dt;
}


O67::OLogPrivate::OLogPrivate()
	: QObject()
	, _Seq( 0 )
	, _MasterLevel( this->initMasterLevel() )
	, _SyslogOnly( false )
	, _LineCounter( 0 )
{
	this->coreLogger( this, __FILE__, __LINE__, OLog::Trace, O67_OLOG_UNKNOWN, QString(), __FUNCTION__, O67_OLOG_TAG_START );

	QMutexLocker	Locker( &_ClassMutex );

//#ifndef	__sun__
//	::openlog( NULL, LOG_PERROR | LOG_PID, LOG_ERR | LOG_USER );
//#else
//	::openlog( NULL, LOG_CONS | LOG_PID, LOG_ERR | LOG_USER );
//#endif

	//	let us take control of where all those log message go
#if	QT_VERSION	>=	0x050000
	qInstallMessageHandler( OLogPrivate::messageHandler );
#else
	qInstallMsgHandler( OLogPrivate::messageHandler );
#endif

	this->coreLogger( this, __FILE__, __LINE__, OLog::Trace, O67_OLOG_UNKNOWN, QString(), __FUNCTION__, O67_OLOG_TAG_END );
}


O67::OLogPrivate::~OLogPrivate()
{
	this->coreLogger( this, __FILE__, __LINE__, OLog::Trace, O67_OLOG_UNKNOWN, QString(), __FUNCTION__, O67_OLOG_TAG_START );

	QMutexLocker	Locker( &_ClassMutex );

//	::closelog();

	//	restore the default message handler
#if	QT_VERSION	>=	0x050000
	qInstallMessageHandler( 0 );
#else
	qInstallMsgHandler( 0 );
#endif

	this->coreLogger( this, __FILE__, __LINE__, OLog::Trace, O67_OLOG_UNKNOWN, QString(), __FUNCTION__, O67_OLOG_TAG_END );
}


//-------------------------------------------------------------------------
//	OLog	class
//-------------------------------------------------------------------------
void
O67::OLog::log( const QString& FileName, long LineNo, Level ErrClass, int ErrNo, const QString& Message, const QVariant& Value )
{
//	QMutexLocker	Locker( &_ClassMutex );

	OLogPrivate::instance()->log( FileName, LineNo, ErrClass, ErrNo, Message, Value );
}


void
O67::OLog::trace( const QString& FileName, long LineNo, const QString& Message, const QVariant& Value )
{
//	QMutexLocker	Locker( &_ClassMutex );

	OLogPrivate::instance()->log( FileName, LineNo, OLog::Trace, O67_OLOG_UNKNOWN, Message, Value );
}


void
O67::OLog::debug( const QString& FileName, long LineNo, const QString& Message, const QVariant& Value )
{
//	QMutexLocker	Locker( &_ClassMutex );

	OLogPrivate::instance()->log( FileName, LineNo, OLog::Debug, O67_OLOG_UNKNOWN, Message, Value );
}


void
O67::OLog::debugPtr( const QString& FileName, long LineNo, const QString& Message, const void* pPointer )
{
//	QMutexLocker	Locker( &_ClassMutex );
	QString		PtrValue;

	PtrValue.sprintf( "(%10p)", pPointer );

	this->debug( FileName, LineNo, Message, PtrValue );
}


void
O67::OLog::info( const QString& FileName, long LineNo, const QString& Message, const QVariant& Value )
{
//	QMutexLocker	Locker( &_ClassMutex );

	OLogPrivate::instance()->log( FileName, LineNo, OLog::Info, O67_OLOG_UNKNOWN, Message, Value );
}


void
O67::OLog::warning( const QString& FileName, long LineNo, const QString& Message, const QVariant& Value )
{
//	QMutexLocker	Locker( &_ClassMutex );

	OLogPrivate::instance()->log( FileName, LineNo, OLog::Warning, O67_OLOG_UNKNOWN, Message, Value );
}


void
O67::OLog::error( const QString& FileName, long LineNo, const QString& Message, const QVariant& Value )
{
//	QMutexLocker	Locker( &_ClassMutex );

	OLogPrivate::instance()->log( FileName, LineNo, OLog::Error, O67_OLOG_UNKNOWN, Message, Value );
}


void
O67::OLog::fatal( const QString& FileName, long LineNo, const QString& Message, const QVariant& Value )
{
//	QMutexLocker	Locker( &_ClassMutex );

	OLogPrivate::instance()->log( FileName, LineNo, OLog::Fatal, O67_OLOG_UNKNOWN, Message, Value );
}


void
O67::OLog::syslogOnly()
{
//	QMutexLocker	Locker( &_ClassMutex );

	OLogPrivate::instance()->syslogOnly();
}


void
O67::OLog::attachLoggerDevice( const QProcess* const pLogger )
{
//	QMutexLocker	Locker( &_ClassMutex );

	OLogPrivate::instance()->attachLoggerDevice( pLogger );
}


O67::OLog::OLog( const void* pOwner, const QString& FileName, long LineNo, const QString& Method )
//	: _ClassMutex( QMutex::Recursive )
//	: OLogPrivate::instance()( OLogPrivate::instance() )
{
//	QMutexLocker	Locker( &_ClassMutex );

	OLogPrivate::instance()->push( this, pOwner, FileName, LineNo, Method );
}


//!	First constructor for main() to initialize the log system
//!	\param	Argc		main's argc
//!	\param	Argv		mains's argv
//!	\param	FileName	Name of the source file: use the __FILE__ macro
//!	\param	LineNo		Line within the source file: use the __LINE__ macro
//!	\param	Method		function or method name: use the __FUNCTION__ or __PRETTY_FUNCTION__ macro
//!	\param	Flags		Option flags to be used \see OLog::Option
O67::OLog::OLog( int& Argc, char** Argv, const QString& FileName, long LineNo, const QString& Method )
//	: _ClassMutex( QMutex::Recursive )
//	: OLogPrivate::instance()( OLogPrivate::instance() )
{
//	QMutexLocker	Locker( &_ClassMutex );

	OLogPrivate::instance()->push( this, NULL, FileName, LineNo, Method );

	Q_UNUSED( Argc );
	Q_UNUSED( Argv );

//	ArgLevel	= this->argInit( Argc, Argv );

//	if ( ArgLevel > OLogPrivate::instance()->currentLevel() )
//		OLogPrivate::instance()->setMasterLevel( ArgLevel );

//	Func	= _Method + " " + O67_OLOG_TAG_START;
//	OLogPrivate::instance()->log( _pOwner, _FileName, _LineNo, OLog::Trace, O67_OLOG_UNKNOWN, Func );
//	OLogPrivate::instance()->log( this, FileName, LineNo, OLog::Trace, O67_OLOG_UNKNOWN, O67_OLOG_TAG_START );
}


O67::OLog::~OLog()
{
//	QMutexLocker	Locker( &_ClassMutex );

	OLogPrivate::instance()->pop( this );
}

