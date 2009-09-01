/**************************************************************************/
/*  Copyright 2009 Tim Day                                                */
/*                                                                        */
/*  This file is part of Evolvotron                                       */
/*                                                                        */
/*  Evolvotron is free software: you can redistribute it and/or modify    */
/*  it under the terms of the GNU General Public License as published by  */
/*  the Free Software Foundation, either version 3 of the License, or     */
/*  (at your option) any later version.                                   */
/*                                                                        */
/*  Evolvotron is distributed in the hope that it will be useful,         */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/*  GNU General Public License for more details.                          */
/*                                                                        */
/*  You should have received a copy of the GNU General Public License     */
/*  along with Evolvotron.  If not, see <http://www.gnu.org/licenses/>.   */
/**************************************************************************/

/*! \file
  \brief Interface for class Args.
*/

#ifndef _args_h_
#define _args_h_

//! Class for sanitizing access to commandline arguments.
/*! The main purpose is to allow >> operators to be used to read args into parameters.
 */
class Args
{
 protected:

  int _argc;
  
  std::vector<std::string> _argv;
  
  //! The stringstream after the selected option.
  std::auto_ptr<std::istringstream> _after;

  static Args* _global;

 public:
  //! Construct from usual arg vector
  Args(int argc,char* argv[]);

  //! Destructor.
  ~Args();

  static Args& global()
    {
      assert(_global!=0);
      return *_global;
    }

  //! Return true if option is present, and set up in anticipation of subsequent after()
  bool option(const std::string& opt,int n=0);

  //! Return a stream containing the n option arguments requested by the last
  std::istringstream& after();

  //! Return the n-th last argument
  const std::string& last(int n) const;
};

#endif
