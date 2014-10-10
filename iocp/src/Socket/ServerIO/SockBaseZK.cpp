// SockBaseZK.cpp: implementation of the CSockBaseZK class.
//
//////////////////////////////////////////////////////////////////////

#include "SockBaseZK.h"
#include "SockServerZK.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//CIServerCallBack* CSockBaseZK::s_pCallback = nuNULL;
//CSockServerZK*  CSockBaseZK::s_pServer  = nuNULL;
CSockBaseZK::CSockBaseZK()
{
	s_pCallback = NULL;
	s_pServer = NULL;
    nuMemset(&m_sockAddr, 0, sizeof(m_sockAddr));
}

CSockBaseZK::~CSockBaseZK()
{

}

