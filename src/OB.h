
#ifndef	__OB__
#define	__OB__

#include <QObject>
#include <OA.h>


//-----------------------------------------------------------------------------
namespace	O67
{

//-----------------------------------------------------------------------------
class	OBPrivate;


//-----------------------------------------------------------------------------
class	OB
	: public OA
{
	Q_OBJECT;
	Q_DISABLE_COPY( OB );
	Q_DECLARE_PRIVATE( OB );
public:
	explicit	OB( QObject* pParent = NULL );
	virtual		~OB();

	virtual	int	dummyB() const;
	virtual	void	setDummyB( int DummyB );

public	Q_SLOTS:
	
protected:
	OB( OBPrivate& dd, QObject* pParent );

protected	Q_SLOTS:

private:
};


};	//	namespace	O67


#endif	// __OB_H__
