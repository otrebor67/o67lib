#ifndef	__OA__
#define	__OA__

#include <QObject>
#include <QString>
#include <OMutex.h>


namespace	O67
{

//-----------------------------------------------------------------------------
class	OAPrivate;


//-----------------------------------------------------------------------------
///	base application class template
class	OA
	: public QObject
	, protected O67::OMutex
{
	Q_OBJECT;
	Q_DISABLE_COPY( OA );
	Q_DECLARE_PRIVATE( OA );
public:
	explicit	OA( QObject* pParent = NULL );
	virtual		~OA();

	virtual	QString		dummyA() const;
	virtual	void		setDummyA( const QString& DummyA );

public	Q_SLOTS:

protected:
	explicit	OA( OAPrivate& dd, QObject* pParent );		//	allow inheritance classes to instatiate with their on private subclass

	OAPrivate* const	d_ptr;

protected	Q_SLOTS:

private:
};


};	//	namespace	O67


#endif	// __OA__
