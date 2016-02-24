
TEMPLATE	= lib

TARGET		= ../lib/o76lib 

DEPENDPATH	+= .
INCLUDEPATH	+= .

# Input
HEADERS		+=	\
		OConsoleApp.h	\
		OConsoleApp_p.h	\
		OCoreApplication.h	\
		OCoreApplication_p.h	\
		OLog.h	\
		OLog_p.h	\
		OMutex.h	\
		OMutex_p.h

SOURCES		+=	\
		OConsoleApp.cpp	\
		OCoreApplication.cpp	\
		OLog.cpp	\
		OMutex.cpp
