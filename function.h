// Source file for evolvotron
// Copyright (C) 2002 Tim Day
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
  \brief Interfaces for Function classes.
  NB There is no class heirarchy here as all virtualisation and boilerplate services are supplied when the functions are plugged into the FunctionNode template.
*/

#ifndef _function_h_
#define _function_h_

#include "function_node.h"
#include "xyz.h"
#include "mutatable_image.h"
#include "matrix.h"
#include "transform.h"

/*! \file
  \brief Interfaces for Function classes
  NB There is no class heirarchy here as all virtualisation and boilerplate services are supplied when the functions are plugged into the FunctionNode template.
  \warning This file should ONLY be included in function_node.cpp, which instantiate everything.
  The one function allowed to escape into the wild is FunctionPreTransform which has it's own header file.
*/

//! Macro to force instantiation of static registration members, and register them with Registry.
#define REGISTER(F) static const Registration* force_ ## F = Registry::add(#F,&FunctionNodeUsing<F>::registration)

//------------------------------------------------------------------------------------------

//! Function class representing a constant value.
class FunctionConstant
{
 public:
  
  // 3 parameters: one for each component of XYZ
  static const uint parameters()
    {
      return 3;
    }

  // No leaf arguments
  static const uint arguments()
    {
      return 0;
    }

  //! Returns the constant value
  static const XYZ evaluate(const FunctionNode& our,const XYZ& p)
    {
      return XYZ(our.param(0),our.param(1),our.param(2));
    }

  //! Returns true, obviously.
  static const bool is_constant(const FunctionNode&)
    {
      return true;
    }
};

REGISTER(FunctionConstant);

//------------------------------------------------------------------------------------------

//! Function class simply returning the position argument.
class FunctionIdentity
{
 public:

  //! No parameters
  static const uint parameters()
    {
      return 0;
    }

  //! No leaf arguments
  static const uint arguments()
    {
      return 0;
    }

  //! Simply return the position argument.
  static const XYZ evaluate(const FunctionNode& our,const XYZ& p)
    {
      return p;
    }

  //! Is definitely not constant.
  static const bool is_constant(const FunctionNode&)
    {
      return false;
    }
};

REGISTER(FunctionIdentity);

//------------------------------------------------------------------------------------------

//! Function class returning position transfomed by a 12-component linear transform.
class FunctionTransform
{
 public:

  //! 12 parameters
  static const uint parameters()
    {
      return 12;
    }

  //! No leaf arguments
  static const uint arguments()
    {
      return 0;
    }

  //! Return the transformed position argument.
  static const XYZ evaluate(const FunctionNode& our,const XYZ& p)
  {
    const Transform transform(our.params());
    return transform.transformed(p);
  }

  //! Is definitely not constant.
  static const bool is_constant(const FunctionNode&)
    {
      return false;
    }
};

REGISTER(FunctionTransform);

//------------------------------------------------------------------------------------------

//! Function class returning position transfomed by a 12-component linear transform.
class FunctionTransformGeneralised
{
 public:

  //! 0 parameters
  static const uint parameters()
    {
      return 0;
    }

  //! 4 leaf arguments providing transform components.
  static const uint arguments()
    {
      return 4;
    }

  //! Return the transformed position argument.
  static const XYZ evaluate(const FunctionNode& our,const XYZ& p)
  {
    const Transform transform(our.arg(0)(p),our.arg(1)(p),our.arg(2)(p),our.arg(3)(p));
    return transform.transformed(p);
  }

  //! Is almost certainly not constant.
  static const bool is_constant(const FunctionNode& our)
  {
    return false;
  }

};

REGISTER(FunctionTransformGeneralised);

//------------------------------------------------------------------------------------------

#include "function_pre_transform.h"

REGISTER(FunctionPreTransform);

//------------------------------------------------------------------------------------------

//! Function class returning leaf node evaluated at position transfomed by a 12-component linear transform.
/*! Unlike FunctionPreTransform, the basis vectors for the transform are not fixed but determined from leaf functions
 */
class FunctionPreTransformGeneralised
{
 public:

  //! 0 parameters
  static const uint parameters()
    {
      return 0;
    }

  //! 5 leaf arguments
  static const uint arguments()
    {
      return 5;
    }

  //! Return the evaluation of arg(0) at the transformed position argument.
  static const XYZ evaluate(const FunctionNode& our,const XYZ& p)
    {
      const Transform transform(our.arg(1)(p),our.arg(2)(p),our.arg(3)(p),our.arg(4)(p));
      return our.arg(0)(transform.transformed(p));
    }

  //! Has the same const-ness as arg(0)
  static const bool is_constant(const FunctionNode& our)
    {
      return our.arg(0).is_constant();
    }
};

REGISTER(FunctionPreTransformGeneralised);

//------------------------------------------------------------------------------------------

#include "function_post_transform.h"

REGISTER(FunctionPostTransform);

//------------------------------------------------------------------------------------------

//! Function class returning leaf node evaluated at given position; result is then transfomed by a 12-component linear transform.
/*! Unlike FunctionPostTransform, the basis vectors for the transform are not fixed but determined from leaf functions
 */
class FunctionPostTransformGeneralised
{
 public:

  //! 0 parameters
  static const uint parameters()
    {
      return 0;
    }

  //! 1 function leaf argument, and 4 for the transform.
  static const uint arguments()
    {
      return 5;
    }

  //! Return the evaluation of arg(0) at the transformed position argument.
  static const XYZ evaluate(const FunctionNode& our,const XYZ& p)
    {
      const Transform transform(our.arg(1)(p),our.arg(2)(p),our.arg(3)(p),our.arg(4)(p));
      return transform.transformed(our.arg(0)(p));
    }

  //! Only constant if all the leaf functions are constant.
  static const bool is_constant(const FunctionNode& our)
    {
      return (
	      our.arg(0).is_constant()
	      && our.arg(1).is_constant()
	      && our.arg(2).is_constant()
	      && our.arg(3).is_constant()
	      && our.arg(4).is_constant()
	      );
    }
};

REGISTER(FunctionPostTransformGeneralised);

//------------------------------------------------------------------------------------------

//! Transforms position transformed by a 30 paramter quadratic transform.
class FunctionTransformQuadratic
{
 public:

  //! 30 parameters: 12 linear plus 9 cross terms plus 9 squared terms.
  static const uint parameters()
    {
      return 30;
    }

  //! 0 leaf arguments.
  static const uint arguments()
    {
      return 0;
    }

  //! Return p transformed.
  static const XYZ evaluate(const FunctionNode& our,const XYZ& p)
    {
      const XYZ translate(our.param( 0),our.param( 1),our.param( 2));
      const XYZ basis_x  (our.param( 3),our.param( 4),our.param( 5));
      const XYZ basis_y  (our.param( 6),our.param( 7),our.param( 8));
      const XYZ basis_z  (our.param( 9),our.param(10),our.param(11));
      const XYZ basis_xy (our.param(12),our.param(13),our.param(14));
      const XYZ basis_xz (our.param(15),our.param(16),our.param(17));
      const XYZ basis_yz (our.param(18),our.param(19),our.param(20));
      const XYZ basis_xx (our.param(21),our.param(22),our.param(23));
      const XYZ basis_yy (our.param(24),our.param(25),our.param(26));
      const XYZ basis_zz (our.param(27),our.param(28),our.param(29));

      return 
	translate
	+basis_x*p.x()+basis_y*p.y()+basis_z*p.z()
	+basis_xy*(p.x()*p.y())+basis_xz*(p.x()*p.z())+basis_yz*(p.y()*p.z())
	+basis_xx*(p.x()*p.x())+basis_yy*(p.y()*p.y())+basis_zz*(p.z()*p.z());
    }

  //! Unlikely to ever be constant (requires all parameters zero).
  static const bool is_constant(const FunctionNode& our)
    {
      return false;
    }
};

REGISTER(FunctionTransformQuadratic);

//------------------------------------------------------------------------------------------

//! Transforms cartesian coordinates to spherical
class FunctionCartesianToSpherical
{
 public:

  //! No parameters.
  static const uint parameters()
    {
      return 0;
    }

  //! No leaf arguments.
  static const uint arguments()
    {
      return 0;
    }

  //! Evaluate function.
  static const XYZ evaluate(const FunctionNode& our,const XYZ& p)
  {
    const float r=p.magnitude();
    
    // Angles are normalised (-1 to +1) over their usual possible range.
    const float theta=atan2(p.y(),p.x())*(1.0f/M_PI);
    const float phi=(r== 0.0f ? 0.0f : asin(p.z()/r)*(1.0f/(0.5f*M_PI)));
  
    return XYZ(r,theta,phi);
  }

  //! Not constant.
  static const bool is_constant(const FunctionNode&)
    {
      return false;
    }

};

REGISTER(FunctionCartesianToSpherical);

//------------------------------------------------------------------------------------------

//! Transforms spherical coordinates to cartesian 
class FunctionSphericalToCartesian
{
 public:

  //! No parameters.
  static const uint parameters()
    {
      return 0;
    }

  //! No leaf arguments.
  static const uint arguments()
    {
      return 0;
    }

  //! Evaluate function.
  static const XYZ evaluate(const FunctionNode& our,const XYZ& p)
  {
    const float r=p.x();
    const float theta=M_PI*p.y();
    const float phi=0.5*M_PI*p.z();
    
    const float x=r*cos(theta)*sin(phi);
    const float y=r*sin(theta)*sin(phi);
    const float z=r*cos(phi);
    
    return XYZ(x,y,z);
  }

  //! Not constant.
  static const bool is_constant(const FunctionNode& our)
  {
    return false;
  }
};

REGISTER(FunctionSphericalToCartesian);

//------------------------------------------------------------------------------------------

// Converts the position argument to spherical coords, pass these through the leaf node, and convert the result back to cartesian.
class FunctionEvaluateInSpherical
{
 public:
  //! No parameters.
  static const uint parameters()
    {
      return 0;
    }

  //! 1 leaf argument.
  static const uint arguments()
    {
      return 1;
    }

  //! Evaluate function.
  static const XYZ evaluate(const FunctionNode& our,const XYZ& p)
    {
      const float in_r=p.magnitude();
      const float in_theta=atan2(p.y(),p.x())*(1.0f/M_PI);
      const float in_phi=(in_r== 0.0f ? 0.0f : asin(p.z()/in_r)*(1.0f/(0.5f*M_PI)));
      
      const XYZ v(our.arg(0)(XYZ(in_r,in_theta,in_phi)));
      
      const float out_r=v.x();
      const float out_theta=M_PI*v.y();
      const float out_phi=0.5*M_PI*v.z();
      
      const float x=out_r*cos(out_theta)*sin(out_phi);
      const float y=out_r*sin(out_theta)*sin(out_phi);
      const float z=out_r*cos(out_phi);
      
      return XYZ(x,y,z);
    }
  
  //! Is constant if leaf node is.
  static const bool is_constant(const FunctionNode& our)
  {
    return our.arg(0).is_constant();
  }

};

REGISTER(FunctionEvaluateInSpherical);

//------------------------------------------------------------------------------------------

#endif
