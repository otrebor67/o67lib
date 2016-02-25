
#include <OA_p.h>
#include <OLog.h>


//-------------------------------------------------------------------------
//	O67::OAPrivate	class
//-------------------------------------------------------------------------
O67::OAPrivate::OAPrivate( O67::OA* pParent )
	: q_ptr( pParent )
	, _DummyA()
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
//	Q_Q( OA );
}


O67::OAPrivate::~OAPrivate()
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
//	Q_Q( OA );
}


//-------------------------------------------------------------------------
//	O67::OA	class
//-------------------------------------------------------------------------
void
O67::OA::setDummyA( const QString& DummyA )
{
	O67::OLog	Log( NULL, __FILE__, __LINE__, __FUNCTION__ );
	QMutexLocker	Locker( this->mutex() );
	Q_D( OA );

	d->_DummyA	= DummyA;
}


QString
O67::OA::dummyA() const
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
	QMutexLocker	Locker( this->mutex() );
	Q_D( const OA );

	return	d->_DummyA;
}


O67::OA::OA( O67::OAPrivate& dd, QObject* pParent )
	: QObject( pParent )
	, O67::OMutex( this, __FILE__, __LINE__ )
	, d_ptr( &dd )
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
	QMutexLocker	Locker( this->mutex() );
	Q_D( OA );

	this->setObjectName( __FUNCTION__ );
}


O67::OA::OA( QObject* pParent )
	: QObject( pParent )
	, O67::OMutex( this, __FILE__, __LINE__ )
	, d_ptr( new	O67::OAPrivate( this ) )
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
	QMutexLocker	Locker( this->mutex() );
//	Q_D( OA );

	this->setObjectName( __FUNCTION__ );
}


O67::OA::~OA()
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
	QMutexLocker	Locker( this->mutex() );
//	Q_D( OA );
}
