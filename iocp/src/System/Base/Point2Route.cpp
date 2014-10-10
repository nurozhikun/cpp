// Point2Segment.cpp: implementation of the CPoint2Route class.
//
//////////////////////////////////////////////////////////////////////
#include "Point2Route.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const double CPoint2Route::s_pi = 3.141593;

// CPoint2Route::CPoint2Route(long nPointMax, const POINT *points /* = NULL */, long nNowCount /* = 0 */)
// {
//     m_ptList = new POINT[nPointMax];
//     m_nPointMax = nPointMax;
//     m_nPointNow = 0;
//     if( NULL != points )
//     {
//         m_nPointNow = lMin(nPointMax, nNowCount);
//         memcpy(m_ptList, points, sizeof(POINT)*m_nPointNow);
//     }
// }
// CPoint2Route::CPoint2Route(LineCoordinates *stations)
// {
// 	m_nPointNow=stations->getNum1()+stations->getNum2();
//     m_ptList = new POINT[m_nPointNow];
// 	int index=0;
// 	Coordinate station;
// 	for(int i=0;i<stations->getNum2();i++)
// 	{
// 		station=stations->GetCoordinate2(i);
// 		m_ptList[index].x=station.X*100000;
// 		m_ptList[index].y=station.Y*100000;
// 		index++;
// 	}
// 	for(int i=0;i<stations->getNum1();i++)
// 	{
// 		station=stations->GetCoordinate1(i);
// 		m_ptList[index].x=station.X*100000;
// 		m_ptList[index].y=station.Y*100000;
// 		index++;
// 	}
// }

CPoint2Route::CPoint2Route(long nPointMax, long nNodeMax)
{
    m_ptList    = new POINT[nPointMax];
    m_pNodeList = new NODE_INFO[nNodeMax];
    m_nPointMax = nPointMax;
    m_nPointNow = 0;
    m_nNodeMax  = nNodeMax;
    m_nNodeNow  = 0;
    m_nLastFixedIdx = -1;
}

CPoint2Route::~CPoint2Route()
{
    if( NULL != m_ptList )
    {
        delete[] m_ptList;
        m_ptList = NULL;
    }
    if( NULL != m_pNodeList )
    {
        delete[] m_pNodeList;
        m_pNodeList = NULL;
    }
}

bool CPoint2Route::AddPoint(long x, long y)
{
    if( NULL == m_ptList || m_nPointNow >= m_nPointMax )
    {
        return false;
    }
    else
    {
        m_ptList[m_nPointNow].x = x;
        m_ptList[m_nPointNow].y = y;
        ++m_nPointNow;
        return true;
    }
}

bool CPoint2Route::AddNodeInfo(long nIdx, long ndis)
{
    if( NULL == m_pNodeList || m_nNodeNow >= m_nNodeMax || nIdx < 0 )
    {
        return false;
    }
    else
    {
        if( m_nNodeNow > 0 && nIdx <= m_pNodeList[m_nNodeNow-1].nIndex )
        {
            return false;
        }
        m_pNodeList[m_nNodeNow].nIndex      = nIdx;
        m_pNodeList[m_nNodeNow].nDistance   = ndis;
        ++m_nNodeNow;
        return true;
    }
}

long CPoint2Route::Fit2Line(long nX, long nY, long& nFixedDis, POINT& ptFixed, 
                              long nLastFixedIdx /* = -1 */, long nAngle /* = -1 */)
{
    if( NULL == m_ptList )
    {
        return -1;
    }
    POINT ptLast;
    if( -1 == nLastFixedIdx )
    {
        nLastFixedIdx   = 0;
        ptLast          = m_ptList[0];
    }
    else
    {
        ptLast          = ptFixed;
    }
    if( 0 == nFixedDis )
    {
        nFixedDis = DEFAULT_MAX_DISTANCE;
    }
    long nNewIdx    = -1;
    long nLastDis   = nFixedDis;
    for(long i = nLastFixedIdx+1; i < m_nPointNow; ++i )
    {
        nFixedDis = nLastDis;
        long nFixedX;
        long nFixedY;
        if( !Point2Segment(nX, nY, ptLast.x, ptLast.y, m_ptList[i].x, m_ptList[i].y
                , nFixedX, nFixedY, nFixedDis, nAngle) )
        {
            ptLast = m_ptList[i];
            continue;
        }
        if( nFixedDis <= nLastDis )
        {
            nLastDis = nFixedDis;
            nNewIdx  = i-1;
            ptFixed.x  = nFixedX;
            ptFixed.y  = nFixedY;
        }
        ptLast = m_ptList[i];
    }
    m_nLastFixedIdx = nNewIdx;
    m_ptFixed       = ptFixed;
    return nNewIdx;
}