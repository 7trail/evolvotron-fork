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
  \brief Implementation of specific Function classes.
  Except there's nothing here because it's all in the header.
  And in fact we don't even include functions.h because it just takes
  ages to compile, then never gets linked into any final executable
  because nothing is needed from this module.  (Although it seems that
  even referencing one symbol from here would do the trick).
*/
 
#include "useful.h"
#include "function_boilerplate.h"
#include "function_registry.h"             

//#include "functions.h"
