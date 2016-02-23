#ifndef	__OMutex__
#define	__OMutex__

#include <QMutex>
#include <QString>

namespace	O67
{

//----------------------------------------------------------------------
class	OMutexPrivate;
//----------------------------------------------------------------------
class	OMutex
	: protected QMutex
{
	Q_DECLARE_PRIVATE( OMutex );
	Q_DISABLE_COPY( OMutex );
public:
	explicit OMutex( void* pOwner, const QString& FileName, int FileLine );
	virtual	~OMutex();

	virtual	void		lock();
	virtual	bool		tryLock();
	virtual	bool		tryLock( int TimeOut );
	virtual	void		unlock();

protected:
	OMutexPrivate*	const	d_ptr;

	virtual	OMutex*	mutex() const;

private:
};


};	//	namespace	O67


#endif	// __OMutex_H__
