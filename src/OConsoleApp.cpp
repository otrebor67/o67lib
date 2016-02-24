

#include <OConsoleApp_p.h>			//	PRIVATE CLASS IMPORT!!!
#include <OLog.h>


//-------------------------------------------------------------------------
//	O67::OConsoleAppPrivate	class
//-------------------------------------------------------------------------
O67::OConsoleAppPrivate::OConsoleAppPrivate( OConsoleApp* pParent )
	: O67::OCoreApplicationPrivate( pParent )
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
//	QMutexLocker	Locker( this->mutex() );
//	Q_Q( OConsoleApp );
}


O67::OConsoleAppPrivate::~OConsoleAppPrivate()
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
//	QMutexLocker	Locker( this->mutex() );
//	Q_Q( OConsoleApp );
}


//-------------------------------------------------------------------------
//	O67::OConsoleApp	class
//-------------------------------------------------------------------------
void
O67::OConsoleApp::posixSignal( int SigNo )
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
	QMutexLocker	Locker( this->mutex() );
//	Q_D( OConsoleApp );

	Log.debug( __FILE__, __LINE__, "POSIX Signal received", SigNo );

	switch( SigNo )
	{
//		case	SIGHUP:		Log.debug( __FILE__, __LINE__, "Signal", "SIGHUP" );	break;
//		case	SIGINT:		Log.debug( __FILE__, __LINE__, "Signal", "SIGINT" );	break;
//		case	SIGQUIT:	Log.debug( __FILE__, __LINE__, "Signal", "SIGQUIT" );	break;
//		case	SIGILL:		Log.debug( __FILE__, __LINE__, "Signal", "SIGILL" );	break;
//		case	SIGTRAP:	Log.debug( __FILE__, __LINE__, "Signal", "SIGTRAP" );	break;
//		case	SIGABRT:	Log.debug( __FILE__, __LINE__, "Signal", "SIGABRT" );	break;
////		case	SIGIOT:		Log.debug( __FILE__, __LINE__, "Signal", "SIGIOT" );	break;		//	duplicate signal
//		case	SIGBUS:		Log.debug( __FILE__, __LINE__, "Signal", "SIGBUS" );	break;
//		case	SIGFPE:		Log.debug( __FILE__, __LINE__, "Signal", "SIGFPE" );	break;
//		case	SIGKILL:	Log.debug( __FILE__, __LINE__, "Signal", "SIGKILL" );	break;
//		case	SIGUSR1:	Log.debug( __FILE__, __LINE__, "Signal", "SIGUSR1" );	break;
//		case	SIGSEGV:	Log.debug( __FILE__, __LINE__, "Signal", "SIGSEGV" );	break;
//		case	SIGUSR2:	Log.debug( __FILE__, __LINE__, "Signal", "SIGUSR2" );	break;
//		case	SIGPIPE:	Log.debug( __FILE__, __LINE__, "Signal", "SIGPIPE" );	break;
//		case	SIGALRM:	Log.debug( __FILE__, __LINE__, "Signal", "SIGALRM" );	break;
//		case	SIGTERM:	Log.debug( __FILE__, __LINE__, "Signal", "SIGTERM" );	break;
//		case	SIGSTKFLT:	Log.debug( __FILE__, __LINE__, "Signal", "SIGSTKFLT" );	break;
//		case	SIGCHLD:	Log.debug( __FILE__, __LINE__, "Signal", "SIGCHLD" );	break;
//		case	SIGCONT:	Log.debug( __FILE__, __LINE__, "Signal", "SIGCONT" );	break;
//		case	SIGSTOP:	Log.debug( __FILE__, __LINE__, "Signal", "SIGSTOP" );	break;
//		case	SIGTSTP:	Log.debug( __FILE__, __LINE__, "Signal", "SIGTSTP" );	break;
//		case	SIGTTIN:	Log.debug( __FILE__, __LINE__, "Signal", "SIGTTIN" );	break;
//		case	SIGTTOU:	Log.debug( __FILE__, __LINE__, "Signal", "SIGTTOU" );	break;
//		case	SIGURG:		Log.debug( __FILE__, __LINE__, "Signal", "SIGURG" );	break;
//		case	SIGXCPU:	Log.debug( __FILE__, __LINE__, "Signal", "SIGXCPU" );	break;
//		case	SIGXFSZ:	Log.debug( __FILE__, __LINE__, "Signal", "SIGXFSZ" );	break;
//		case	SIGVTALRM:	Log.debug( __FILE__, __LINE__, "Signal", "SIGVTALRM" );	break;
//		case	SIGPROF:	Log.debug( __FILE__, __LINE__, "Signal", "SIGPROF" );	break;
//		case	SIGWINCH:	Log.debug( __FILE__, __LINE__, "Signal", "SIGWINCH" );	break;
//		case	SIGPOLL:	Log.debug( __FILE__, __LINE__, "Signal", "SIGPOLL" );	break;
////		case	SIGIO:		Log.debug( __FILE__, __LINE__, "Signal", "SIGGIO" );	break;		//	duplicate signal
//		case	SIGPWR:		Log.debug( __FILE__, __LINE__, "Signal", "SIGPWR" );	break;
//		case	SIGSYS:		Log.debug( __FILE__, __LINE__, "Signal", "SIGSYS" );	break;
////		case	SIGUNUSED:	Log.debug( __FILE__, __LINE__, "Signal", "SIGUNUSED" );	break;		//	duplicate signal
//
		default:
			Log.debug( __FILE__, __LINE__, "Signal not known", SigNo );
	}
}


//void
//O67::OConsoleApp::shutdown()
//{
//	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
//	QMutexLocker	Locker( this->mutex() );
//	Q_D( OConsoleApp );
//
////	this->watchUnixSignal( SIGINT,	false );
////	this->watchUnixSignal( SIGUSR1,	false );
////	this->watchUnixSignal( SIGUSR2,	false );
//
//	this->quit();
//}


O67::OConsoleApp::OConsoleApp( int Argc, char** Argv )
	: O67::OCoreApplication( *( new O67::OConsoleAppPrivate( this ) ), Argc, Argv )
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
	QMutexLocker	Locker( this->mutex() );
//	Q_D( OConsoleApp );

#if	QT_VERSION	>=	0x050000
//	O67_OLOG_ASSERT_FATAL( connect( this, SIGNAL( unixSignal(int) ),	this,	SLOT( posixSignal(int) ) ) );
#else
	O67_OLOG_ASSERT_FATAL( connect( this, SIGNAL( unixSignal(int) ),	this,	SLOT( posixSignal(int) ) ) );
#endif

//	this->watchUnixSignal( SIGHUP,	true );
//	this->watchUnixSignal( SIGINT,	true );
//	this->watchUnixSignal( SIGQUIT,	true );
//	this->watchUnixSignal( SIGILL,	true );
//	this->watchUnixSignal( SIGTRAP,	true );
//	this->watchUnixSignal( SIGABRT,	true );
////	this->watchUnixSignal( SIGIOT,	true );			//	duplicate signal
//	this->watchUnixSignal( SIGBUS,	true );
//	this->watchUnixSignal( SIGFPE,	true );
//	this->watchUnixSignal( SIGKILL,	true );
//	this->watchUnixSignal( SIGUSR1,	true );
//	this->watchUnixSignal( SIGSEGV,	true );
//	this->watchUnixSignal( SIGUSR2,	true );
//	this->watchUnixSignal( SIGPIPE,	true );
//	this->watchUnixSignal( SIGALRM,	true );
//	this->watchUnixSignal( SIGTERM,	true );
//	this->watchUnixSignal( SIGSTKFLT,	true );
//	this->watchUnixSignal( SIGCHLD,	true );
//	this->watchUnixSignal( SIGCONT,	true );
//	this->watchUnixSignal( SIGSTOP,	true );
//	this->watchUnixSignal( SIGTSTP,	true );
//	this->watchUnixSignal( SIGTTIN,	true );
//	this->watchUnixSignal( SIGTTOU,	true );
//	this->watchUnixSignal( SIGURG,	true );
//	this->watchUnixSignal( SIGXCPU,	true );
//	this->watchUnixSignal( SIGXFSZ,	true );
//	this->watchUnixSignal( SIGVTALRM,	true );
//	this->watchUnixSignal( SIGPROF,	true );
//	this->watchUnixSignal( SIGWINCH,	true );
//	this->watchUnixSignal( SIGPOLL,	true );
////	this->watchUnixSignal( SIGIO,	true );			//	duplicate signal
//	this->watchUnixSignal( SIGPWR,	true );
//	this->watchUnixSignal( SIGSYS,	true );
////	this->watchUnixSignal( SIGUNUSED,	true );		//	duplicate signal
}


O67::OConsoleApp::~OConsoleApp()
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
	QMutexLocker	Locker( this->mutex() );
//	Q_D( OConsoleApp );

//	this->shutdown();
}
