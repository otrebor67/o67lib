
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
		OMutex_p.h	\
		OB.h	\
		OB_p.h	\
		OA.h	\
		OA_p.h

SOURCES		+=	\
		OConsoleApp.cpp	\
		OCoreApplication.cpp	\
		OLog.cpp	\
		OMutex.cpp	\
		OB.cpp	\
		OA.cpp
