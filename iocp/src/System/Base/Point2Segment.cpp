// Point2Segment.cpp: implementation of the CPoint2Segment class.
//
//////////////////////////////////////////////////////////////////////
#include "Point2Segment.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const double CPoint2Segment::s_pi = 3.141593;

CPoint2Segment::CPoint2Segment(long nPointMax, const Point *points /* = NULL */, long nNowCount /* = 0 */)
{
    m_ptList = new Point[nPointMax];
    m_nPointMax = nPointMax;
    m_nPointNow = 0;
    if( NULL != points )
    {
        m_nPointNow = lMin(nPointMax, nNowCount);
        memcpy(m_ptList, points, sizeof(Point)*m_nPointNow);
    }
}

CPoint2Segment::~CPoint2Segment()
{
    if( NULL != m_ptList )
    {
        delete[] m_ptList;
        m_ptList = NULL;
    }
}

bool CPoint2Segment::AddPoint(long x, long y)
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

long CPoint2Segment::Fit2LineEx(long nX, long nY, bool& bDeflect, long& nFixedDis, Point& ptFixed, long nLastFixedIdx, long nAngle)
{
    if( NULL == m_ptList )
    {
        bDeflect = false;
        return -1;
    }
    Point ptLast;
    if( -1 == nLastFixedIdx )
    {
        nLastFixedIdx   = 0;
        ptLast = m_ptList[0];
    }
    else
    {
        ptLast = ptFixed;
    }
    if( 0 == nFixedDis )
    {
        nFixedDis = DEFAULT_MAX_DISTANCE;
    }
    long nNewIdx    = -1;
    long nLastDis   = nFixedDis;
    long nLeft, nRight, nTop, nBottom;
    bDeflect = true;
    for(long i = nLastFixedIdx+1; i < m_nPointNow; ++i )
    {
        nLeft      = LMIN(ptLast.x, m_ptList[i].x) - nFixedDis;
        nRight     = LMAX(ptLast.x, m_ptList[i].x) + nFixedDis;
        nTop       = LMIN(ptLast.y, m_ptList[i].y) - nFixedDis;
        nBottom    = LMAX(ptLast.y, m_ptList[i].y) + nFixedDis;
        if( !POINT_IN_RECT(nX, nY, nLeft, nTop, nRight, nBottom) )
        {
            ptLast = m_ptList[i];
            continue;
        }

        bDeflect = false; //Æ«ÀëÖ»°´¾àÀëËã

        if( nAngle >= 0 )
        {
            long dx = m_ptList[i].x - ptLast.x;
            long dy = m_ptList[i].y - ptLast.y;
            long nSegAngle = (long)(atan2((float)dy, (float)dx) * 180 / s_pi);
            if( nSegAngle < 0 )
            {
                nSegAngle += 360;
            }
            long nDifAngle = (nAngle - nSegAngle) % 360;
            nDifAngle = LABS(nDifAngle);
            if( nDifAngle > DEFAULT_MAX_ANGLE_DIFF && nDifAngle < 360 - DEFAULT_MAX_ANGLE_DIFF )
            {
                ptLast = m_ptList[i];
                continue;
            }
        }
        nFixedDis = nLastDis;
        long nFixedX;
        long nFixedY;
        if( !Point2Segment(nX, nY, ptLast.x, ptLast.y, m_ptList[i].x, m_ptList[i].y
            , nFixedX, nFixedY, nFixedDis) )
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
    return nNewIdx;
}


long CPoint2Segment::Fit2Line(long nX, long nY, long& nFixedDis, Point& ptFixed, 
                              long nLastFixedIdx /* = -1 */, long nAngle /* = -1 */)
{
    if( NULL == m_ptList )
    {
        return -1;
    }
    Point ptLast;
    if( -1 == nLastFixedIdx )
    {
        nLastFixedIdx   = 0;
        ptLast = m_ptList[0];
    }
    else
    {
        ptLast = ptFixed;
    }
    if( 0 == nFixedDis )
    {
        nFixedDis = DEFAULT_MAX_DISTANCE;
    }
    long nNewIdx    = -1;
    long nLastDis   = nFixedDis;
    long nLeft, nRight, nTop, nBottom;
    for(long i = nLastFixedIdx+1; i < m_nPointNow; ++i )
    {
        nLeft      = lMin(ptLast.x, m_ptList[i].x) - nFixedDis;
        nRight     = lMax(ptLast.x, m_ptList[i].x) + nFixedDis;
        nTop       = lMin(ptLast.y, m_ptList[i].y) - nFixedDis;
        nBottom    = lMax(ptLast.y, m_ptList[i].y) + nFixedDis;
        if( !PointInRect(nX, nY, nLeft, nTop, nRight, nBottom) )
        {
            ptLast = m_ptList[i];
            continue;
        }
        if( nAngle >= 0 )
        {
            long dx = m_ptList[i].x - ptLast.x;
            long dy = m_ptList[i].y - ptLast.y;
            long nSegAngle = (long)(atan2((float)dy, (float)dx) * 180 / s_pi);
            if( nSegAngle < 0 )
            {
                nSegAngle += 360;
            }
            long nDifAngle = (nAngle - nSegAngle) % 360;
            nDifAngle = lAbs(nDifAngle);
            if( nDifAngle > DEFAULT_MAX_ANGLE_DIFF && nDifAngle < 360 - DEFAULT_MAX_ANGLE_DIFF )
            {
                ptLast = m_ptList[i];
                continue;
            }
        }
        nFixedDis = nLastDis;
        long nFixedX;
        long nFixedY;
        if( !Point2Segment(nX, nY, ptLast.x, ptLast.y, m_ptList[i].x, m_ptList[i].y
                , nFixedX, nFixedY, nFixedDis) )
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
    return nNewIdx;
}