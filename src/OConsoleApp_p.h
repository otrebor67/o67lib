#ifndef	__OConsoleAppPrivate__
#define	__OConsoleAppPrivate__


#include <QString>
#include <OConsoleApp.h>
#include <OCoreApplication_p.h>


//---------------------------------------------------------------------------
namespace	O67
{

//---------------------------------------------------------------------------
struct	OConsoleAppPrivate
	: public OCoreApplicationPrivate
{
	Q_DECLARE_PUBLIC( OConsoleApp );

	explicit OConsoleAppPrivate( OConsoleApp* pParent );
	virtual	~OConsoleAppPrivate();
};

};	//	namespace	O67

#endif	//	__OConsoleAppPrivate__
