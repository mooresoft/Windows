#pragma once

//#include <atltypes.h>

#pragma pack(push, 1)
typedef struct tagHPVEC2D{
	double x;
	double y;
}HPVEC2D;

typedef struct tagHPVEC3D{
	double x;
	double y;
	double z;
}HPVEC3D;

typedef struct tagHPRECT{
	double left;
	double top;
	double right;
	double bottom;
}HPRECT, *PHPRECT, NEAR *NHPPRECT, FAR *LPHPRECT;
#pragma pack(pop)
typedef const HPRECT FAR* LPCHPRECT;;



#define WX_UNKNOWN_COEF (2000000.00F)

//class CHPRect;
//bool DoubleEquals( const double &d1, const double d2, const double zero=1e-9);

/////////////////////////////////////////////////////////////////////////////
// CHPVec2D
class CHPVec2D : public tagHPVEC2D
{
public:

	// Constructors
	// construct an uninitialized size
	CHPVec2D() throw();
	// create from two integers
	CHPVec2D(double dX, double dY) throw();
	// create from another size
	CHPVec2D(HPVEC2D initVal) throw();

	CHPVec2D(POINT initVal) throw();

	void Set(double dX, double dY);
	// Operations
	BOOL operator==(HPVEC2D val) const throw();
	BOOL operator!=(HPVEC2D val) const throw();
	void operator+=(HPVEC2D val) throw();
	void operator-=(HPVEC2D val) throw();

	CHPVec2D operator+(HPVEC2D val) const throw();
	CHPVec2D operator-(HPVEC2D val) const throw();
	CHPVec2D operator-() const throw();

	// Operators returning CRect values
//	CHPRect operator+(const HPRECT* lpRect) const throw();
//	CHPRect operator-(const HPRECT* lpRect) const throw();

	// Operations

	// translate the point
	void Offset(double xOffset, double yOffset) throw();
	void Offset(HPVEC2D point) throw();

	double Square();

	double DistanceSquare(HPVEC2D v);
	double DistanceSquare(double vx, double vy);

	void operator*=(double val) throw();
	void operator/=(double val) throw();
	CHPVec2D operator*(double val) const throw();
	CHPVec2D operator/(double val) const throw();

	POINT Floor();
	POINT Ceil();
	POINT Point();

	BOOL Valid();
};

class CHPVec3D : public tagHPVEC3D
{
public:

	// Constructors
	// construct an uninitialized size
	CHPVec3D() throw();
	// create from double(s)
	CHPVec3D(double dV) throw();
	CHPVec3D(double dX, double dY, double dZ) throw();
	// create from another size
	CHPVec3D(HPVEC3D initVal) throw();

	void Set(double dX, double dY, double dZ);

	// Operations
	BOOL operator==(HPVEC3D val) const throw();
	BOOL operator!=(HPVEC3D val) const throw();
	void operator+=(HPVEC3D val) throw();
	void operator-=(HPVEC3D val) throw();

	CHPVec3D operator+(HPVEC3D val) const throw();
	CHPVec3D operator-(HPVEC3D val) const throw();
	CHPVec3D operator-() const throw();
	
	CHPVec3D &operator=(HPVEC3D val);
	CHPVec3D &operator=(double val);
	// Operations

	// translate the point
	void Offset(double xOffset, double yOffset, double zOffset) throw();
	void Offset(HPVEC3D point) throw();

	double Square();

	double DistanceSquare(HPVEC3D v);
	double DistanceSquare(double vx,double vy, double vz);

	void operator*=(double val) throw();
	void operator/=(double val) throw();
	CHPVec3D operator*(double val) const throw();
	CHPVec3D operator/(double val) const throw();

	//µã³Ë£¬µã³ý
	CHPVec3D operator*(CHPVec3D p) const throw();
	CHPVec3D operator/(CHPVec3D p) const throw();

	BOOL Valid();
};

/////////////////////////////////////////////////////////////////////////////
// CRect - A 2-D rectangle, similar to Windows RECT structure.
class CHPRect : public tagHPRECT
{
	// Constructors
public:
	// uninitialized rectangle
	CHPRect() throw();
	// from left, top, right, and bottom
	CHPRect(double l, double t, double r, double b) throw();
	// copy constructor
/*	CHPRect(const HPRECT& srcRect) throw();
	// from a pointer to another rect
	CHPRect(LPCHPRECT lpSrcRect) throw();*/
	// from two points
	CHPRect(HPVEC2D topLeft, HPVEC2D bottomRight) throw();

	// Attributes (in addition to RECT members)

	// retrieves the width
	double Width() const throw();
	// returns the height
	double Height() const throw();

	// reference to the top-left point
	CHPVec2D& TopLeft() throw();
	// reference to the bottom-right point
	CHPVec2D& BottomRight() throw();
	// the geometric center point of the rectangle
	CHPVec2D CenterPoint() const throw();
	/*

	// swap the left and right
	void SwapLeftRight() throw();
	static void WINAPI SwapLeftRight(LPRECT lpRect) throw();

	*/
	// convert between CRect and LPRECT/LPCRECT (no need for &)
	operator LPHPRECT() throw();
	operator LPCHPRECT() const throw();
	// returns TRUE if rectangle has no area
	BOOL IsRectEmpty() const throw();
	// returns TRUE if rectangle is at (0,0) and has no area
	BOOL IsRectNull() const throw();
	// returns TRUE if point is within rectangle
	BOOL PtInRect(HPVEC2D point) const throw();
	BOOL PtInRect(double px, double py) const throw();

	/*
	// Operations

	// set rectangle from left, top, right, and bottom
	void SetRect(double x1, double y1, double x2, double y2) throw();
	void SetRect(HPVEC2D topLeft, HPVEC2D bottomRight) throw();
	// empty the rectangle
	void SetRectEmpty() throw();
	// copy from another rectangle
	void CopyRect(LPCHPRECT lpSrcRect) throw();
	// TRUE if exactly the same as another rectangle
	BOOL EqualRect(LPCHPRECT lpRect) const throw();

	// Inflate rectangle's width and height by
	// x units to the left and right ends of the rectangle
	// and y units to the top and bottom.
	void InflateRect(double x, double y) throw();
	// Inflate rectangle's width and height by
	// size.cx units to the left and right ends of the rectangle
	// and size.cy units to the top and bottom.
	void InflateRect(HPVEC2D size) throw();
	// Inflate rectangle's width and height by moving individual sides.
	// Left side is moved to the left, right side is moved to the right,
	// top is moved up and bottom is moved down.
*/
	void InflateRect(LPCHPRECT lpRect) throw();
	void InflateRect(double l, double t, double r, double b) throw();

	// deflate the rectangle's width and height without
	// moving its top or left
/*
	void DeflateRect(double x, double y) throw();
	void DeflateRect(HPVEC2D size) throw();
*/
	void DeflateRect(LPCHPRECT lpRect) throw();
	void DeflateRect(double l, double t, double r, double b) throw();

	// translate the rectangle by moving its top and left
	void OffsetRect(double x, double y) throw();
	void OffsetRect(HPVEC2D size) throw();


	void NormalizeRect() throw();
	// absolute position of rectangle
	void MoveToY(double y) throw();
	void MoveToX(double x) throw();
	void MoveToXY(double x, double y) throw();
	void MoveToXY(HPVEC2D point) throw();

	/*
	// set this rectangle to intersection of two others
	BOOL IntersectRect(LPCHPRECT lpRect1, LPCHPRECT lpRect2) throw();

	// set this rectangle to bounding union of two others
	BOOL UnionRect(LPCHPRECT lpRect1, LPCHPRECT lpRect2) throw();

	// set this rectangle to minimum of two others
	BOOL SubtractRect(LPCHPRECT lpRectSrc1, LPCHPRECT lpRectSrc2) throw();

	// Additional Operations
	void operator=(const HPRECT& srcRect) throw();
	BOOL operator==(const HPRECT& rect) const throw();
	BOOL operator!=(const HPRECT& rect) const throw();
	void operator+=(HPVEC2D point) throw();
	void operator-=(HPVEC2D point) throw();
*/
	void operator+=(LPCHPRECT lpRect) throw();
	void operator-=(LPCHPRECT lpRect) throw();
/*
	void operator&=(const HPRECT& rect) throw();
	void operator|=(const HPRECT& rect) throw();

	// Operators returning CRect values
	CHPRect operator+(HPVEC2D point) const throw();
	CHPRect operator-(HPVEC2D point) const throw();
	CHPRect operator+(LPCHPRECT lpRect) const throw();
	CHPRect operator-(LPCHPRECT lpRect) const throw();
	CHPRect operator&(const HPRECT& rect2) const throw();
	CHPRect operator|(const HPRECT& rect2) const throw();
	CHPRect MulDiv(int nMultiplier, int nDivisor) const throw();
*/
};

