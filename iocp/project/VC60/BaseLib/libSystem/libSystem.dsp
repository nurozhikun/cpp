# Microsoft Developer Studio Project File - Name="libSystem" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libSystem - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libSystem.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libSystem.mak" CFG="libSystem - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libSystem - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libSystem - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libSystem - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libSystem - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../../../src/inc/crossH" /I "../../../../src/inc/crossh/windows" /I "../../../../src/inc/logH" /I "../../../../src/inc/systemh" /I "../../../../src/system/base" /I "../../../../src/system/windows" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\library\winxp\libSystem.lib"

!ENDIF 

# Begin Target

# Name "libSystem - Win32 Release"
# Name "libSystem - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\src\System\Base\BaseCodeListZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Base\BaseFileZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Base\BaseFunctionsZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Base\BaseLogZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Base\BaseThreadZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Base\BaseTTSZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Windows\CrossDebugOutZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Windows\CrossFileZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Windows\crossFunctionsZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Windows\CrossGpsZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Windows\CrossIOCompletionZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Windows\CrossLoadDllZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Windows\CrossSharedMemoryZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Windows\CrossSocketZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Windows\CrossTaskZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Windows\CrossThreadZK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Base\Point2Route.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\src\Inc\systemH\BaseCodeListZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Inc\systemH\BaseFileZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Inc\systemH\BaseFunctionsZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Inc\systemH\BaseLogZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Inc\systemH\BaseThreadZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Inc\systemH\BaseTTSZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Inc\systemH\CrossDebugOutZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Inc\systemH\CrossFileZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Inc\systemH\crossFunctionsZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Inc\systemH\CrossGpsZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Inc\systemH\CrossIOCompletionZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Inc\systemH\CrossLoadDllZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Inc\systemH\CrossSharedMemoryZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Inc\systemH\CrossSocketZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Inc\systemH\CrossTaskZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Inc\systemH\CrossThreadZK.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Inc\systemH\Point2Route.h
# End Source File
# End Group
# End Target
# End Project
