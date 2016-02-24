
#ifndef	__OConsoleApp_H__
#define	__OConsoleApp_H__

#include <OCoreApplication.h>


//-----------------------------------------------------------------------------
namespace	O67
{

//-----------------------------------------------------------------------------
class	OConsoleAppPrivate;


//-----------------------------------------------------------------------------
///	Base class for a console (non-gui) application
///	inherits OCoreApplication. enhances the base class with a handler for posix signals
class	OConsoleApp
	: public OCoreApplication
{
	Q_OBJECT;
	Q_DISABLE_COPY( OConsoleApp );
	Q_DECLARE_PRIVATE( OConsoleApp );
public:
	OConsoleApp( int Argc, char** Argv );
	virtual	~OConsoleApp();

public	Q_SLOTS:
	virtual	void	posixSignal( int SigNo );
	
protected:
	OConsoleApp( OConsoleAppPrivate& dd, int Argc, char** Argv );

protected	Q_SLOTS:

private:
};


};	//	namespace	O67


#endif	// __OConsoleApp_H__
