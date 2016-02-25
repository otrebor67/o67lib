#ifndef	__OBPrivate__
#define	__OBPrivate__


#include <OB.h>
#include <OA_p.h>


//---------------------------------------------------------------------------
namespace	O67
{

//---------------------------------------------------------------------------
struct	OBPrivate
	: public OAPrivate
{
	Q_DECLARE_PUBLIC( OB );

	explicit OBPrivate( OB* pParent );
	virtual	~OBPrivate();

	int	_DummyB;
};

};	//	namespace	O67

#endif	//	__OBPrivate__
