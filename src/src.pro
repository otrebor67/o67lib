
TEMPLATE	= lib

TARGET		= ../lib/o76lib 

DEPENDPATH	+= .
INCLUDEPATH	+= .

# Input
HEADERS		+=	\
		OLog.h	\
		OLog_p.h	\
		OMutex.h	\
		OMutex_p.h

SOURCES		+=	\
		OLog.cpp	\
		OMutex.cpp
