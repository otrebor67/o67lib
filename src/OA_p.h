#ifndef	__OAPrivate__
#define	__OAPrivate__


#include <QString>
#include <OA.h>


//---------------------------------------------------------------------------
namespace	O67
{

//---------------------------------------------------------------------------
struct	OAPrivate
{
	Q_DECLARE_PUBLIC( OA );

	explicit OAPrivate( OA* pParent );
	virtual	~OAPrivate();

	OA*	const	q_ptr;
	QString		_DummyA;
};

};	//	namespace	O67

#endif	//	__OAPrivate__
