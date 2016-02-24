#ifndef	__OCoreApplicationPrivate__
#define	__OCoreApplicationPrivate__


#include <QString>
#include <OCoreApplication.h>


//---------------------------------------------------------------------------
namespace	O67
{

//---------------------------------------------------------------------------
struct	OCoreApplicationPrivate
{
	Q_DECLARE_PUBLIC( OCoreApplication );

	explicit OCoreApplicationPrivate( OCoreApplication* pParent );
	virtual	~OCoreApplicationPrivate();

	OCoreApplication*	const	q_ptr;
	QStringList			_ArgList;
};

};	//	namespace	O67

#endif	//	__OCoreApplicationPrivate__
