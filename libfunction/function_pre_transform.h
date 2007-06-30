// Source file for evolvotron
// Copyright (C) 2002,2003 Tim Day
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
  \brief Interfaces for class FunctionPreTransform 
  This class would normally live in functions.h (and is included and registered there), 
  but is split out so it can be efficiently used by MutatableImageDisplay and EvolvotronMain.
  NB There is no class heirarchy here as all virtualisation and boilerplate services are supplied when the functions are plugged into the FunctionNode template.
*/

#ifndef _function_pre_transform_h_
#define _function_pre_transform_h_

 
#include "transform.h"

//! Function class returning leaf node evaluated at position transfomed by a 12-component linear transform.
FUNCTION_BEGIN(FunctionPreTransform,12,1,false,0)

  //! Return the evaluation of arg(0) at the transformed position argument.
  virtual const XYZ evaluate(const XYZ& p) const
  {
    const Transform transform(params());
    return arg(0)(transform.transformed(p));
  }

FUNCTION_END(FunctionPreTransform)

#endif
