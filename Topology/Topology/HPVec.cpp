#include "StdAfx.h"
#include "HPVec.h"


bool DoubleEquals(const double &a, const double b, const double zero)
{
	return (fabs(a - b) < zero);
}

bool DoubleEquals(const double &a, const double b)
{
	return (fabs(a - b) < 1e-9);
}

//////////////////////////////////////////
// CHPVec2D
CHPVec2D::CHPVec2D() throw()
{ x = WX_UNKNOWN_COEF; y=WX_UNKNOWN_COEF; }
CHPVec2D::CHPVec2D(double initX, double initY) throw()
{ x = initX; y = initY; }
CHPVec2D::CHPVec2D(HPVEC2D initPt) throw()
{ *(HPVEC2D*)this = initPt; }

CHPVec2D::CHPVec2D(POINT initPt) throw()
{ x = initPt.x; y = initPt.y; }

void CHPVec2D::Set(double dX, double dY)
{ x = dX; y = dY; }
void CHPVec2D::Offset(double xOffset, double yOffset) throw()
{ x += xOffset; y += yOffset; }
void CHPVec2D::Offset(HPVEC2D point) throw()
{ x += point.x; y += point.y; }
BOOL CHPVec2D::operator==(HPVEC2D point) const throw()
{ return (x == point.x && y == point.y); }
BOOL CHPVec2D::operator!=(HPVEC2D point) const throw()
{ return (x != point.x || y != point.y); }
void CHPVec2D::operator+=(HPVEC2D point) throw()
{ x += point.x; y += point.y; }
void CHPVec2D::operator-=(HPVEC2D point) throw()
{ x -= point.x; y -= point.y; }
CHPVec2D CHPVec2D::operator-() const throw()
{ return CHPVec2D(-x, -y); }
CHPVec2D CHPVec2D::operator+(HPVEC2D point) const throw()
{ return CHPVec2D(x + point.x, y + point.y); }
CHPVec2D CHPVec2D::operator-(HPVEC2D point) const throw()
{ return CHPVec2D(x - point.x, y - point.y); }
/*inline CHPRect CHPVec2D::operator+(const HPRECT* lpRect) const throw()
{ return CHPRect(lpRect) + *this; }
inline CHPRect CHPVec2D::operator-(const HPRECT* lpRect) const throw()
{ return CHPRect(lpRect) - *this; }*/
double CHPVec2D::Square()
{ return (x * x + y * y);}

double CHPVec2D::DistanceSquare(HPVEC2D v)
{ return ((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));}
inline double CHPVec2D::DistanceSquare(double vx, double vy)
{ return ((x - vx) * (x - vx) + (y - vy) * (y - vy));}

void CHPVec2D::operator*=(double val) throw()
{	x *= val; y *= val;};
void CHPVec2D::operator/=(double val) throw()
{ x /= val; y /= val; };
CHPVec2D CHPVec2D::operator*(double val) const throw()
{ return CHPVec2D( x * val, y * val);};
CHPVec2D CHPVec2D::operator/(double val) const throw()
{ return CHPVec2D( x / val, y / val);};


POINT CHPVec2D::Floor()
{ POINT pt; pt.x = (int)floor(this->x); pt.y = (int)floor(this->y); return pt;}
POINT CHPVec2D::Ceil()
{ POINT pt; pt.x = (int)ceil(this->x); pt.y = (int)ceil(this->y); return pt;}
POINT CHPVec2D::Point()
{ POINT pt; pt.x = (int)this->x; pt.y = (int)this->y; return pt;}
BOOL CHPVec2D::Valid()
{return (x != WX_UNKNOWN_COEF && y != WX_UNKNOWN_COEF);};

// CHPVec3D
CHPVec3D::CHPVec3D() throw()
{ x = WX_UNKNOWN_COEF; y=WX_UNKNOWN_COEF; z=WX_UNKNOWN_COEF;}
CHPVec3D::CHPVec3D(double dV) throw()
{ x = dV; y=dV; z=dV;}
CHPVec3D::CHPVec3D(double initX, double initY, double initZ) throw()
{ x = initX; y = initY; z=initZ;}
CHPVec3D::CHPVec3D(HPVEC3D initPt) throw()
{ *(HPVEC3D*)this = initPt; }

void CHPVec3D::Set(double dX, double dY, double dZ)
{ x = dX; y = dY; z = dZ; }

void CHPVec3D::Offset(double xOffset, double yOffset, double zOffset) throw()
{ x += xOffset; y += yOffset; z += zOffset; }
void CHPVec3D::Offset(HPVEC3D point) throw()
{ x += point.x; y += point.y; z += point.z; }
BOOL CHPVec3D::operator==(HPVEC3D point) const throw()
{ return (x == point.x && y == point.y && z == point.z); }
BOOL CHPVec3D::operator!=(HPVEC3D point) const throw()
{ return (x != point.x || y != point.y || z != point.z); }
void CHPVec3D::operator+=(HPVEC3D point) throw()
{ x += point.x; y += point.y; z+= point.z; }
void CHPVec3D::operator-=(HPVEC3D point) throw()
{ x -= point.x; y -= point.y; z -= point.z; }
CHPVec3D CHPVec3D::operator-() const throw()
{ return CHPVec3D(-x, -y, -z); }
CHPVec3D CHPVec3D::operator+(HPVEC3D point) const throw()
{ return CHPVec3D(x + point.x, y + point.y, z + point.z); }
CHPVec3D CHPVec3D::operator-(HPVEC3D point) const throw()
{ return CHPVec3D(x - point.x, y - point.y, z - point.z); }
CHPVec3D &CHPVec3D::operator=(HPVEC3D val)
{ x=val.x;y=val.y;z=val.z;return (*this); }
CHPVec3D &CHPVec3D::operator=(double val)
{ x=val;y=val;z=val;return (*this); }


double CHPVec3D::Square()
{ return (x * x + y * y + z * z);}

double CHPVec3D::DistanceSquare(HPVEC3D v)
{ return ((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y)  + (z - v.z) * (z - v.z));}

double CHPVec3D::DistanceSquare(double vx,double vy, double vz)
{ return ((x - vx) * (x - vx) + (y - vy) * (y - vy)  + (z - vz) * (z - vz));}


void CHPVec3D::operator*=(double val) throw()
{	x *= val; y *= val; z *= val;};
void CHPVec3D::operator/=(double val) throw()
{ x /= val; y /= val; z /= val;};
CHPVec3D CHPVec3D::operator*(double val) const throw()
{ return CHPVec3D( x * val, y * val, z * val);};
CHPVec3D CHPVec3D::operator/(double val) const throw()
{ return CHPVec3D( x / val, y / val, z / val);};

CHPVec3D CHPVec3D::operator*(CHPVec3D p) const throw()
{ return CHPVec3D( x * p.x, y * p.y, z * p.z);};
CHPVec3D CHPVec3D::operator/(CHPVec3D p) const throw()
{ return CHPVec3D( x / p.x, y / p.y, z / p.z);};

BOOL CHPVec3D::Valid()
{return (x != WX_UNKNOWN_COEF && y != WX_UNKNOWN_COEF && z != WX_UNKNOWN_COEF);};

// CHPRect
CHPRect::CHPRect() throw()
{ left=0;top=0;right=0;bottom=0;}
CHPRect::CHPRect(double l, double t, double r, double b) throw()
{ left = l; top = t; right = r; bottom = b; }
/*
inline CHPRect::CHPRect(const HPRECT& srcRect) throw()
{ ::CopyRect(this, &srcRect); }
inline CHPRect::CHPRect(LPCHPRECT lpSrcRect) throw()
{ ::CopyRect(this, lpSrcRect); }*/
CHPRect::CHPRect(HPVEC2D topLeft, HPVEC2D bottomRight) throw()
{ left = topLeft.x; top = topLeft.y;right = bottomRight.x; bottom = bottomRight.y; }
double CHPRect::Width() const throw()
{ return right - left; }
double CHPRect::Height() const throw()
{ return bottom - top; }

CHPVec2D& CHPRect::TopLeft() throw()
{ return *((CHPVec2D*)this); }
CHPVec2D& CHPRect::BottomRight() throw()
{ return *((CHPVec2D*)this+1); }
CHPVec2D CHPRect::CenterPoint() const throw()
{ return CHPVec2D((left+right)/2, (top+bottom)/2); }
/*
inline void CHPRect::SwapLeftRight() throw()
{ SwapLeftRight(LPHPRECT(this)); }
inline void WINAPI CHPRect::SwapLeftRight(LPHPRECT lpRect) throw()
{ LONG temp = lpRect->left; lpRect->left = lpRect->right; lpRect->right = temp; }
*/
CHPRect::operator LPHPRECT() throw()
{ return this; }
CHPRect::operator LPCHPRECT() const throw()
{ return this; }
BOOL CHPRect::IsRectEmpty() const throw()
{ return (DoubleEquals(left,right) && DoubleEquals(top, bottom)); }
BOOL CHPRect::IsRectNull() const throw()
{ return (DoubleEquals(left, 0.0000f) && DoubleEquals(right, 0.0000f) && DoubleEquals(top, 0.0000f) && DoubleEquals(bottom, 0.0000f)); }

BOOL CHPRect::PtInRect(double px, double py) const throw()
{
	double xl,xr,yt,yb;
	left<right ? (xl = left,xr = right):(xl = right, xr = left);
	top < bottom ? (yt = top, yb = bottom) : (yt = bottom, yb = top);
	return (!(px<xl || px > xr || py < yt || py > yb)); 
}

BOOL CHPRect::PtInRect(HPVEC2D point) const throw()
{ return PtInRect(point.x,point.y); }

/*
inline void CHPRect::SetRect(int x1, int y1, int x2, int y2) throw()
{ ::SetRect(this, x1, y1, x2, y2); }
inline void CHPRect::SetRect(HPVEC2D topLeft, HPVEC2D bottomRight) throw()
{ ::SetRect(this, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y); }
inline void CHPRect::SetRectEmpty() throw()
{ ::SetRectEmpty(this); }
inline void CHPRect::CopyRect(LPCHPRECT lpSrcRect) throw()
{ ::CopyRect(this, lpSrcRect); }
inline BOOL CHPRect::EqualRect(LPCHPRECT lpRect) const throw()
{ return ::EqualRect(this, lpRect); }
inline void CHPRect::InflateRect(int x, int y) throw()
{ ::InflateRect(this, x, y); }
inline void CHPRect::InflateRect(SIZE size) throw()
{ ::InflateRect(this, size.cx, size.cy); }
inline void CHPRect::DeflateRect(int x, int y) throw()
{ ::InflateRect(this, -x, -y); }
inline void CHPRect::DeflateRect(SIZE size) throw()
{ ::InflateRect(this, -size.cx, -size.cy); }
inline void CHPRect::OffsetRect(SIZE size) throw()
{ ::OffsetRect(this, size.cx, size.cy); }
*/
void CHPRect::OffsetRect(double x, double y) throw()
{ left += x; right += x; top += y; bottom += y;}
void CHPRect::OffsetRect(HPVEC2D point) throw()
{ OffsetRect(point.x, point.y); }

void CHPRect::MoveToY(double y) throw()
{ bottom = Height() + y; top = y; }
void CHPRect::MoveToX(double x) throw()
{ right = Width() + x; left = x; }
void CHPRect::MoveToXY(double x, double y) throw()
{ MoveToX(x); MoveToY(y); }
void CHPRect::MoveToXY(HPVEC2D pt) throw()
{ MoveToX(pt.x); MoveToY(pt.y); }
/*
inline BOOL CHPRect::IntersectRect(LPCHPRECT lpRect1, LPCHPRECT lpRect2) throw()
{ return ::IntersectRect(this, lpRect1, lpRect2);}
inline BOOL CHPRect::UnionRect(LPCHPRECT lpRect1, LPCHPRECT lpRect2) throw()
{ return ::UnionRect(this, lpRect1, lpRect2); }
inline void CHPRect::operator=(const HPRECT& srcRect) throw()
{ ::CopyRect(this, &srcRect); }
inline BOOL CHPRect::operator==(const HPRECT& rect) const throw()
{ return ::EqualRect(this, &rect); }
inline BOOL CHPRect::operator!=(const HPRECT& rect) const throw()
{ return !::EqualRect(this, &rect); }
inline void CHPRect::operator+=(HPVEC2D point) throw()
{ ::OffsetRect(this, point.x, point.y); }
inline void CHPRect::operator+=(SIZE size) throw()
{ ::OffsetRect(this, size.cx, size.cy); }
*/
void CHPRect::operator+=(LPCHPRECT lpRect) throw()
{ InflateRect(lpRect); }
/*
inline void CHPRect::operator-=(HPVEC2D point) throw()
{ ::OffsetRect(this, -point.x, -point.y); }
inline void CHPRect::operator-=(SIZE size) throw()
{ ::OffsetRect(this, -size.cx, -size.cy); }
*/
void CHPRect::operator-=(LPCHPRECT lpRect) throw()
{ DeflateRect(lpRect); }
/*
inline void CHPRect::operator&=(const HPRECT& rect) throw()
{ ::IntersectRect(this, this, &rect); }
inline void CHPRect::operator|=(const HPRECT& rect) throw()
{ ::UnionRect(this, this, &rect); }
inline CHPRect CHPRect::operator+(HPVEC2D pt) const throw()
{ CHPRect rect(*this); ::OffsetRect(&rect, pt.x, pt.y); return rect; }
inline CHPRect CHPRect::operator-(HPVEC2D pt) const throw()
{ CHPRect rect(*this); ::OffsetRect(&rect, -pt.x, -pt.y); return rect; }
inline CHPRect CHPRect::operator+(SIZE size) const throw()
{ CHPRect rect(*this); ::OffsetRect(&rect, size.cx, size.cy); return rect; }
inline CHPRect CHPRect::operator-(SIZE size) const throw()
{ CHPRect rect(*this); ::OffsetRect(&rect, -size.cx, -size.cy); return rect; }
inline CHPRect CHPRect::operator+(LPCHPRECT lpRect) const throw()
{ CHPRect rect(this); rect.InflateRect(lpRect); return rect; }
inline CHPRect CHPRect::operator-(LPCHPRECT lpRect) const throw()
{ CHPRect rect(this); rect.DeflateRect(lpRect); return rect; }
inline CHPRect CHPRect::operator&(const HPRECT& rect2) const throw()
{ CHPRect rect; ::IntersectRect(&rect, this, &rect2);
return rect; }
inline CHPRect CHPRect::operator|(const HPRECT& rect2) const throw()
{ CHPRect rect; ::UnionRect(&rect, this, &rect2);
return rect; }
inline BOOL CHPRect::SubtractRect(LPCHPRECT lpRectSrc1, LPCHPRECT lpRectSrc2) throw()
{ return ::SubtractRect(this, lpRectSrc1, lpRectSrc2); }
*/
void CHPRect::NormalizeRect() throw()
{
	double nTemp;
	if (left > right)
	{
		nTemp = left;
		left = right;
		right = nTemp;
	}
	if (top > bottom)
	{
		nTemp = top;
		top = bottom;
		bottom = nTemp;
	}
}

void CHPRect::InflateRect(LPCHPRECT lpRect) throw()
{
	left -= lpRect->left;		top -= lpRect->top;
	right += lpRect->right;		bottom += lpRect->bottom;
}

void CHPRect::InflateRect(double l, double t, double r, double b) throw()
{
	left -= l;			top -= t;
	right += r;			bottom += b;
}

void CHPRect::DeflateRect(LPCHPRECT lpRect) throw()
{
	left += lpRect->left;	top += lpRect->top;
	right -= lpRect->right;	bottom -= lpRect->bottom;
}

void CHPRect::DeflateRect(double l, double t, double r, double b) throw()
{
	left += l;		top += t;
	right -= r;		bottom -= b;
}
/*
inline CHPRect CHPRect::MulDiv(int nMultiplier, int nDivisor) const throw()
{
	return CHPRect(
		::MulDiv(left, nMultiplier, nDivisor),
		::MulDiv(top, nMultiplier, nDivisor),
		::MulDiv(right, nMultiplier, nDivisor),
		::MulDiv(bottom, nMultiplier, nDivisor));
}
*/