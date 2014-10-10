# Microsoft Developer Studio Project File - Name="ServerIO" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ServerIO - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ServerIO.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ServerIO.mak" CFG="ServerIO - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ServerIO - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ServerIO - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ServerIO - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SERVERIO_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SERVERIO_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "ServerIO - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SERVERIO_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../../../src/Inc/crossH" /I "../../../../src/Inc/crossH/windows" /I "../../../../src/inc/logh" /I "../../../../src/inc/systemH" /I "../../../../src/inc/socketH" /I "../.../../../src/socket/ServerIO" /I "../../../../src/socket/Protocol" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SERVERIO_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libSystem.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../../../../runner/ServerManager/ServerIO.dll" /pdbtype:sept /libpath:"../../../../library/winxp"

!ENDIF 

# Begin Target

# Name "ServerIO - Win32 Release"
# Name "ServerIO - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\src\Socket\ServerIO\ClientOverlappedZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Socket\ServerIO\libServerIO.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Socket\ServerIO\OverlappedVesselZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Socket\ServerIO\OverlappedZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Socket\Protocol\ProtocolBaseZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Socket\ServerIO\ServerProtocolZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Socket\ServerIO\SockBaseZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Socket\ServerIO\SockClientMgrZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Socket\ServerIO\SockClientZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Socket\ServerIO\SocketServerIO.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Socket\ServerIO\SocketThreadZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Socket\ServerIO\SockServerZK.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\src\Socket\ServerIO\ClientOverlappedZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Inc\socketH\ISocketServerIO.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Inc\socketH\LoadServerIO.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Socket\ServerIO\OverlappedVesselZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Socket\ServerIO\OverlappedZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Socket\Protocol\ProtocolBaseZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Socket\ServerIO\ServerProtocolZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Socket\ServerIO\SockBaseZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Socket\ServerIO\SockClientMgrZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Socket\ServerIO\SockClientZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Socket\ServerIO\SocketServerIO.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Socket\ServerIO\SocketThreadZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Socket\ServerIO\SockServerZK.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
