#ifndef	__OMutexPrivate__
#define	__RCMutexPrivate__


#include <QString>
#include <OMutex.h>


//---------------------------------------------------------------------------
namespace	O67
{

//---------------------------------------------------------------------------
struct	OMutexPrivate
{
	Q_DECLARE_PUBLIC( OMutex );

	explicit OMutexPrivate( OMutex* pParent, void* pOwner, const QString& FileName, int _FileLine );
	virtual	~OMutexPrivate();

	OMutex*	const	q_ptr;
	void*		_pOwner;
	QString		_FileName;
	int		_FileLine;
};

};	//	namespace	O67

#endif	//	__OMutexPrivate__
