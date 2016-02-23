

#include <QtDebug>
#include <OMutex_p.h>
#include <OLog.h>


//------------------------------------------------------------------------------------------------
O67::OMutexPrivate::OMutexPrivate( OMutex* pParent, void* pOwner, const QString& FileName, int FileLine )
	: q_ptr( pParent )
	, _pOwner( pOwner )
	, _FileName( FileName )
	, _FileLine( FileLine )
{
//	Q_Q( OMutex );
	O67::OLog	Log( _pOwner, _FileName, _FileLine, __FUNCTION__ );
};


O67::OMutexPrivate::~OMutexPrivate()
{
//	Q_Q( OMutex );
	O67::OLog	Log( _pOwner, _FileName, _FileLine, __FUNCTION__ );
};


//------------------------------------------------------------------------------------------------
O67::OMutex*
O67::OMutex::mutex() const
{
	Q_D( const OMutex );
	O67::OLog	Log( d->_pOwner, d->_FileName, d->_FileLine, __FUNCTION__ );

	return	const_cast<OMutex*>( this );
}


void
O67::OMutex::lock()
{
	Q_D( OMutex );
	O67::OLog	Log( d->_pOwner, d->_FileName, d->_FileLine, __FUNCTION__ );

	QMutex::lock();
}


bool
O67::OMutex::tryLock()
{
	Q_D( OMutex );
	O67::OLog	Log( d->_pOwner, d->_FileName, d->_FileLine, __FUNCTION__ );

	return	QMutex::tryLock();
}


bool
O67::OMutex::tryLock( int TimeOut )
{
	Q_D( OMutex );
	O67::OLog	Log( d->_pOwner, d->_FileName, d->_FileLine, __FUNCTION__ );

	return	QMutex::tryLock( TimeOut );
}


void
O67::OMutex::unlock()
{
	Q_D( OMutex );
	O67::OLog	Log( d->_pOwner, d->_FileName, d->_FileLine, __FUNCTION__ );

	QMutex::unlock();
}


O67::OMutex::OMutex( void* pOwner, const QString& FileName, int FileLine )
	: QMutex( QMutex::Recursive )
	, d_ptr( new	OMutexPrivate( this, pOwner, FileName, FileLine ) )
{
	Q_D( OMutex );
	O67::OLog	Log( d->_pOwner, d->_FileName, d->_FileLine, __FUNCTION__ );
}


O67::OMutex::~OMutex()
{
	Q_D( OMutex );
	O67::OLog	Log( d->_pOwner, d->_FileName, d->_FileLine, __FUNCTION__ );
}

