

#include <OB_p.h>			//	PRIVATE CLASS IMPORT!!!
#include <OLog.h>


//-------------------------------------------------------------------------
//	O67::OBPrivate	class
//-------------------------------------------------------------------------
O67::OBPrivate::OBPrivate( OB* pParent )
	: O67::OAPrivate( pParent )
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
//	Q_Q( OB );
}


O67::OBPrivate::~OBPrivate()
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
//	Q_Q( OB );
}


//-------------------------------------------------------------------------
//	O67::OB	class
//-------------------------------------------------------------------------
void
O67::OB::setDummyB( int DummyB )
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
	QMutexLocker	Locker( this->mutex() );
	Q_D( OB );

	d->_DummyB	= DummyB;
}


int
O67::OB::dummyB() const
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
	QMutexLocker	Locker( this->mutex() );
	Q_D( const OB );

	return	d->_DummyB;
}


O67::OB::OB( OBPrivate& dd, QObject* pParent )
	: O67::OA( dd, pParent )
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
	QMutexLocker	Locker( this->mutex() );
//	Q_D( OB );

	this->setObjectName( __FUNCTION__ );
}


O67::OB::OB( QObject* pParent )
	: O67::OA( *( new O67::OBPrivate( this ) ), pParent )
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
	QMutexLocker	Locker( this->mutex() );
//	Q_D( OB );

	this->setObjectName( __FUNCTION__ );
}


///	class destructor
O67::OB::~OB()
{
	O67::OLog	Log( this, __FILE__, __LINE__, __FUNCTION__ );
	QMutexLocker	Locker( this->mutex() );
//	Q_D( OB );
}
