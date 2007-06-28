// Source file for evolvotron
// Copyright (C) 2007 Tim Day
/*
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

/*! \file
  \brief Interfaces and implementation for specific Function classes.
  As much as possible of the implementation should be pushed into the FunctionBoilerplate template.
*/

#ifndef _functions_choose_h_
#define _functions_choose_h_

#include "function_boilerplate.h"

//------------------------------------------------------------------------------------------

// Strip of one function across another
FUNCTION_BEGIN(FunctionChooseStrip,3,3,false,FnStructure)

  //! Evaluate function.
  virtual const XYZ evaluate(const XYZ& p) const
    {
      if (fabs(p.y()) > fabs(arg(2)(p)%XYZ(param(0),param(1),param(2)))) return arg(1)(p);
      else return arg(0)(p);
    }
  
FUNCTION_END(FunctionChooseStrip)

//------------------------------------------------------------------------------------------

// Strip of one function across another
FUNCTION_BEGIN(FunctionChooseStripBlend,6,4,false,FnStructure)

  //! Evaluate function.
  virtual const XYZ evaluate(const XYZ& p) const
    {
      const real r0=fabs(arg(2)(p)%XYZ(param(0),param(1),param(2)));
      const real r1=fabs(arg(3)(p)%XYZ(param(3),param(4),param(5)));
      const real inner=std::min(r0,r1);
      const real outer=std::max(r0,r1);

      const real ay=fabs(p.y());
      if (ay<=inner) return arg(0)(p);
      if (ay>=outer) return arg(1)(p);
      
      const XYZ v0(arg(0)(p));
      const XYZ v1(arg(1)(p));

      return v0+(v1-v0)*(ay-inner)/(outer-inner);
    }
  
FUNCTION_END(FunctionChooseStripBlend)

//------------------------------------------------------------------------------------------

//! Function implements selection between 2 functions based on the relative magnitudes of 2 other functions
FUNCTION_BEGIN(FunctionChooseSphere,0,4,false,FnStructure)

  //! Evaluate function.
  virtual const XYZ evaluate(const XYZ& p) const
    {
      if ((arg(0)(p)).magnitude2()<(arg(1)(p)).magnitude2())
	return arg(2)(p);
      else
	return arg(3)(p);
    }
  
FUNCTION_END(FunctionChooseSphere)

//------------------------------------------------------------------------------------------

//! Function implements selection between 2 functions based on whether a rectangle contains a point
FUNCTION_BEGIN(FunctionChooseRect,0,4,false,FnStructure)

  //! Evaluate function.
  virtual const XYZ evaluate(const XYZ& p) const
    {
      const XYZ p0(arg(0)(p));
      const XYZ p1(arg(1)(p));
      
      if (p1.origin_centred_rect_contains(p0))
	return arg(2)(p);
      else
	return arg(3)(p);
    }
  
FUNCTION_END(FunctionChooseRect)

//------------------------------------------------------------------------------------------

//! Function implements selection between 2 functions based on position in 3d mesh
FUNCTION_BEGIN(FunctionChooseFrom2InCubeMesh,0,2,false,FnStructure)

  //! Evaluate function.
  virtual const XYZ evaluate(const XYZ& p) const
    {
      const int x=static_cast<int>(floorf(p.x()));
      const int y=static_cast<int>(floorf(p.y()));
      const int z=static_cast<int>(floorf(p.z()));

      if ((x+y+z)&1)
	return arg(0)(p);
      else
	return arg(1)(p);
    }
  
FUNCTION_END(FunctionChooseFrom2InCubeMesh);

//------------------------------------------------------------------------------------------

//! Function implements selection between 2 functions based on position in 3d mesh
FUNCTION_BEGIN(FunctionChooseFrom3InCubeMesh,0,3,false,FnStructure)

  //! Evaluate function.
  virtual const XYZ evaluate(const XYZ& p) const
    {
      const int x=static_cast<int>(floorf(p.x()));
      const int y=static_cast<int>(floorf(p.y()));
      const int z=static_cast<int>(floorf(p.z()));

      return arg(modulusi(x+y+z,3))(p);
    }
  
FUNCTION_END(FunctionChooseFrom3InCubeMesh)

//------------------------------------------------------------------------------------------

//! Function implements selection between 2 functions based on position in 2d grid
FUNCTION_BEGIN(FunctionChooseFrom2InSquareGrid,0,2,false,FnStructure)

  //! Evaluate function.
  virtual const XYZ evaluate(const XYZ& p) const
    {
      const int x=static_cast<int>(floorf(p.x()));
      const int y=static_cast<int>(floorf(p.y()));

      if ((x+y)&1)
	return arg(0)(p);
      else
	return arg(1)(p);
    }
  
FUNCTION_END(FunctionChooseFrom2InSquareGrid)

//------------------------------------------------------------------------------------------

//! Function implements selection between 3 functions based on position in 2d grid
FUNCTION_BEGIN(FunctionChooseFrom3InSquareGrid,0,3,false,FnStructure)

  //! Evaluate function.
  virtual const XYZ evaluate(const XYZ& p) const
    {
      const int x=static_cast<int>(floorf(p.x()));
      const int y=static_cast<int>(floorf(p.y()));

      return arg(modulusi(x+y,3))(p);
    }
  
FUNCTION_END(FunctionChooseFrom3InSquareGrid)

//------------------------------------------------------------------------------------------

//! Function implements selection between 2 functions based on position in grid of triangles 
FUNCTION_BEGIN(FunctionChooseFrom2InTriangleGrid,0,2,false,FnStructure)

  //! Evaluate function.
  virtual const XYZ evaluate(const XYZ& p) const
    {
      static const XYZ d0(1.0         ,0.0         ,0.0);
      static const XYZ d1(cos(  M_PI/3),sin(  M_PI/3),0.0);
      static const XYZ d2(cos(2*M_PI/3),sin(2*M_PI/3),0.0);
      
      const int a=static_cast<int>(floorf(p%d0));
      const int b=static_cast<int>(floorf(p%d1));
      const int c=static_cast<int>(floorf(p%d2));

      if ((a+b+c)&1)
	return arg(0)(p);
      else
	return arg(1)(p);
    }
  
FUNCTION_END(FunctionChooseFrom2InTriangleGrid)

//------------------------------------------------------------------------------------------

//! Function implements selection between 2 functions based on position in grid of triangles 
/*! Not entirely sure this one produces a sensible pattern.  Needs explicitly testing.
 */
FUNCTION_BEGIN(FunctionChooseFrom3InTriangleGrid,0,3,false,FnStructure)

  //! Evaluate function.
  virtual const XYZ evaluate(const XYZ& p) const
    {
      static const XYZ d0(1.0         ,0.0         ,0.0);
      static const XYZ d1(cos(  M_PI/3),sin(  M_PI/3),0.0);
      static const XYZ d2(cos(2*M_PI/3),sin(2*M_PI/3),0.0);
      
      const int a=static_cast<int>(floorf(p%d0));
      const int b=static_cast<int>(floorf(p%d1));
      const int c=static_cast<int>(floorf(p%d2));

      return arg(modulusi(a+b+c,3))(p);
    }
  
FUNCTION_END(FunctionChooseFrom3InTriangleGrid)

//------------------------------------------------------------------------------------------

//! Function implements selection between 3 functions based on position in grid of hexagons
/*! Don't entirely understand how this works, but it looks nice.
 */
FUNCTION_BEGIN(FunctionChooseFrom3InDiamondGrid,0,3,false,FnStructure)

  //! Evaluate function.
  virtual const XYZ evaluate(const XYZ& p) const
    {
      // Basis vectors for hex grid
      static const XYZ d0(1.0         ,0.0         ,0.0);
      static const XYZ d1(cos(  M_PI/3),sin(  M_PI/3),0.0);
      static const XYZ d2(cos(2*M_PI/3),sin(2*M_PI/3),0.0);
      
      // Dot with basis
      const real p0=p%d0;
      const real p1=p%d1;
      const real p2=p%d2;

      // Find nearest on-grid point
      const int i0=(int)floorf(p0+0.5);
      const int i1=(int)floorf(p1+0.5);
      const int i2=(int)floorf(p2+0.5);

      // Measure distance
      const real m0=fabsf(p0-i0);
      const real m1=fabsf(p1-i1);
      const real m2=fabsf(p2-i2);

      // Closest one decides which function
      if (m0<=m1 && m0<=m2)
	return arg(0)(p);
      else if (m1<=m0 && m1<=m2)
	return arg(1)(p);
      else 
	return arg(2)(p);
    }
  
FUNCTION_END(FunctionChooseFrom3InDiamondGrid)

//------------------------------------------------------------------------------------------

//! Function implements selection between 3 functions based on position in grid of hexagons
FUNCTION_BEGIN(FunctionChooseFrom3InHexagonGrid,0,3,false,FnStructure)

  //! Co-ordinates of hexagon with given hex-grid coords
  static const XYZ hex(int x,int y)
    {
      const real k=sqrt(3.0)/2.0;
      return XYZ(
		 x*k,
		 y+((x&1) ? 0.5 : 0.0),
		 0.0
		 );
    }

  //! Finds hex-grid coordinates of hex containing cartesian px,py
  static void nearest_hex(real px,real py,int &hx,int& hy)
    {
      // Initial guess at which hex we're in:
      const real k=sqrt(3.0)/2.0;

      const int nx=(int)rintf(px/k);
      const int ny=(int)(
			 (nx&1) 
			 ? 
			 rintf(py-0.5) 
			 : 
			 rintf(py)
			 );

      hx=nx;
      hy=ny;
      const XYZ ph=hex(nx,ny);
      real m2b=(XYZ(px,py,0.0)-ph).magnitude2();
      
      for (int dy=-1;dy<=1;dy++)
	for (int dx=-1;dx<=1;dx++)
	  if (!(dy==0 && dx==0))
	    {
	      const real m2=(XYZ(px,py,0.0)-hex(nx+dx,ny+dy)).magnitude2();
	      if (m2<m2b)
		{
		  hx=nx+dx;
		  hy=ny+dy;
		  m2b=m2;
		}
	    }
    } 
  
  //! Evaluate function.
  virtual const XYZ evaluate(const XYZ& p) const
    {
      int hx;
      int hy;
      nearest_hex(p.x(),p.y(),hx,hy);
      const uint which=hy+((hx&1)? 2 : 0);
      return arg(modulusi(which,3))(p);
    }
    
FUNCTION_END(FunctionChooseFrom3InHexagonGrid)

//------------------------------------------------------------------------------------------

//! Function implements selection between 2 functions based on position in grid of hexagons
FUNCTION_BEGIN(FunctionChooseFrom2InBorderedHexagonGrid,1,2,false,FnStructure)

  //! Co-ordinates of hexagon with given hex-grid coords
  static const XYZ hex(int x,int y)
    {
      const real k=sqrt(3.0)/2.0;
      return XYZ(
		 x*k,
		 y+((x&1) ? 0.5 : 0.0),
		 0.0
		 );
    }

  //! Finds hex-grid coordinates of hex containing cartesian px,py
  static void nearest_hex(real px,real py,int &hx,int& hy)
    {
      // Initial guess at which hex we're in:
      const real k=sqrt(3.0)/2.0;

      const int nx=(int)rintf(px/k);
      const int ny=(int)(
			 (nx&1) 
			 ? 
			 rintf(py-0.5) 
			 : 
			 rintf(py)
			 );

      hx=nx;
      hy=ny;
      const XYZ ph=hex(nx,ny);
      real m2b=(XYZ(px,py,0.0)-ph).magnitude2();
      
      for (int dy=-1;dy<=1;dy++)
	for (int dx=-1;dx<=1;dx++)
	  if (!(dy==0 && dx==0))
	    {
	      const real m2=(XYZ(px,py,0.0)-hex(nx+dx,ny+dy)).magnitude2();
	      if (m2<m2b)
		{
		  hx=nx+dx;
		  hy=ny+dy;
		  m2b=m2;
		}
	    }
    } 
  
  //! Evaluate function.
  virtual const XYZ evaluate(const XYZ& p) const
    {
      int hx;
      int hy;
      nearest_hex(p.x(),p.y(),hx,hy);

      bool in_border=false;

      // Hex centres are separated by 1.0 so limit border size
      const real b=modulusf(param(0),0.5);

      // Step along grid co-ordinates in various directions.  If there's a nearer point, we're in the border.
      for (uint a=0;a<6;a++)
	{
	  const real dx=b*sin(a*M_PI/3.0);
	  const real dy=b*cos(a*M_PI/3.0);
	  
	  int ax;
	  int ay;
	  nearest_hex(p.x()+dx,p.y()+dy,ax,ay);

	  if (hx!=ax || hy!=ay)
	    {
	      in_border=true;
	      break;
	    }
	}

      return arg(in_border)(p);
    }  
FUNCTION_END(FunctionChooseFrom2InBorderedHexagonGrid)

//------------------------------------------------------------------------------------------


#endif
