#ifndef	__OCoreApplication__
#define	__OCoreApplication__

#include <signal.h>
#include <QCoreApplication>
#include <QStringList>
#include <OMutex.h>


namespace	O67
{

//-----------------------------------------------------------------------------
class	OCoreApplicationPrivate;


//-----------------------------------------------------------------------------
///	Core application base class
class	OCoreApplication
	: public QCoreApplication
	, protected O67::OMutex
{
	Q_OBJECT;
	Q_DISABLE_COPY( OCoreApplication );
	Q_DECLARE_PRIVATE( OCoreApplication );
public:
	virtual	~OCoreApplication();

	static	int		exec();

public	Q_SLOTS:
	virtual	void		quit();

protected:
				OCoreApplication();								//	disable plain constructor
		explicit	OCoreApplication( OCoreApplicationPrivate& dd, int Argc, char** Argv );		//	allow inheritance classes to instatiate with their on private subclass

	OCoreApplicationPrivate* const	d_ptr;

	virtual	void		posixSignal( int SigNo );

	virtual	QStringList	arguments();

protected	Q_SLOTS:
	virtual	void	run()	= 0;

private:
};


};	//	namsepace	O67


#endif	// __OCoreApplication__
