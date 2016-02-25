
#include <QTimer>
#include <OCoreApplication_p.h>
#include <OLog.h>


//-------------------------------------------------------------------------
//	O67::OCoreApplicationPrivate	class
//-------------------------------------------------------------------------
O67::OCoreApplicationPrivate::OCoreApplicationPrivate( O67::OCoreApplication* pParent )
	: q_ptr( pParent )
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
//	Q_Q( QCoreApplication );
}


O67::OCoreApplicationPrivate::~OCoreApplicationPrivate()
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
//	Q_Q( QCoreApplication );
}


//-------------------------------------------------------------------------
//	O67::OCoreApplication	class
//-------------------------------------------------------------------------
void
O67::OCoreApplication::posixSignal( int SigNo )
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
	QMutexLocker	Locker( this->mutex() );

	Log.debug( __FILE__, __LINE__, "Posix Signal", SigNo );
}


void
O67::OCoreApplication::quit()
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
	QMutexLocker	Locker( this->mutex() );
//	Q_D( OCoreApplication );

	Log.info( __FILE__, __LINE__, tr("--- quitting event processing ---") );

	QCoreApplication::quit();
}


int
O67::OCoreApplication::exec()
{
	O67::OLog	Log( NULL, __FILE__, __LINE__, __FUNCTION__ );
//	QMutexLocker	Locker( this->mutex() );
//	Q_D( OCoreApplication );
	int		RetCode;

	QTimer::singleShot( 1000, O67::OCoreApplication::instance(), SLOT( run() ) );

	Log.info( __FILE__, __LINE__, tr("--- starting event processing ---") );

	RetCode	= QCoreApplication::exec();

	Log.info( __FILE__, __LINE__, tr("--- stopped event processing ---") );

	return	RetCode;
}


///	This method fixes the inconsistency in behaviour between QCoreApplication
///	and QApplication - it does not work reliably with QCoreApplication
QStringList
O67::OCoreApplication::arguments()
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
	QMutexLocker	Locker( this->mutex() );
	Q_D( OCoreApplication );

	return	d->_ArgList;
}


O67::OCoreApplication::OCoreApplication( O67::OCoreApplicationPrivate& dd, int Argc, char** Argv )
	: QCoreApplication( Argc, Argv )
	, O67::OMutex( this, __FILE__, __LINE__ )
	, d_ptr( &dd )
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
	QMutexLocker	Locker( this->mutex() );
	Q_D( OCoreApplication );

	for( int i = 0; i < Argc; ++i )
	{
		d->_ArgList.append( QString::fromLocal8Bit( Argv[ i ] ) );

		Log.debug( __FILE__, __LINE__, tr("Argv[%1]").arg( i ), d->_ArgList.at( i ) );
	}

}


O67::OCoreApplication::~OCoreApplication()
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
	QMutexLocker	Locker( this->mutex() );
}
