// CrossDebugOutZK.cpp: implementation of the CCrossDebugOutZK class.
//
//////////////////////////////////////////////////////////////////////

#include "CrossDebugOutZK.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrossDebugOutZK::CCrossDebugOutZK()
{

}

CCrossDebugOutZK::~CCrossDebugOutZK()
{

}

nuVOID CCrossDebugOutZK::DebugOutStringA(const nuCHAR* pcs)
{
    printf("%s\n", pcs);
}

nuVOID CCrossDebugOutZK::DebugOutInteger(nuLONG nValue)
{
    printf("%ld\n", nValue);
}