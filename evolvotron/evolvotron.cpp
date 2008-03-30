// Source file for evolvotron
// Copyright (C) 2007 Tim Day
/*! \page License License

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
  \brief Application code for evolvotron executable (contains "main").
*/

/*! \mainpage Evolvotron : image evolver

  \author Tim Day 

  \section introduction Introduction
  "Evolvotron" is an interactive tool for producing "generative art".
  Images are generated from function trees, which are then mutated and evolved through a process of user selection.

  \todo For new features to be added, see the TODO file.
 */

#include "evolvotron_precompiled.h"

#include "args.h"
#include "platform_specific.h"

#include "evolvotron_main.h"

//! Application code
int main(int argc,char* argv[])
{
  QApplication app(argc,argv);

  Args args(argc,argv);

  // A 4:3 ratio would nice to get square images on most screens, but isn't many images
  uint cols=6;
  uint rows=5;
  
  uint frames=1;
  uint framerate=8;

  if (args.option("-v")) 
    std::clog.rdbuf(std::cerr.rdbuf());
  else
    std::clog.rdbuf(sink_ostream.rdbuf());

  if (args.option("-g",2)) args.after() >> cols >> rows;
  if (cols*rows<2)
    {
      std::cerr << "Must be at least 2 display grid cells (options: -g <cols> <rows>)\n";
      exit(1);
    }
  
  uint threads=get_number_of_processors();
    
  if (args.option("-t",1))
    {
      args.after() >> threads;
      if (threads<1)
	{
	  std::cerr << "Must specify at least one thread for option -t <threads>)\n";
	  exit(1);
	}
    }

  std::clog << "Using " << threads << " threads\n";

  int niceness_grid=4;
  if (args.option("-n",1)) args.after() >> niceness_grid;

  const bool separate_farm_for_enlargements=args.option("-E");

  int niceness_enlargement=niceness_grid+4;
  if (args.option("-N",1)) args.after() >> niceness_enlargement;

  if (args.option("-f",1)) args.after() >> frames;
  if (frames<1)
    {
      std::cerr << "Must specify at least 1 frame (option: -f <frames>)\n";
      exit(1);
    }

  if (args.option("-r",1)) args.after() >> framerate;
  if (framerate<1)
    {
      std::cerr << "Must specify framerate of at least 1 (option: -r <framerate>)\n";
      exit(1);
    }

  std::string favourite_function;
  bool favourite_function_unwrapped=false;
  if (args.option("-x",1))
    {
      args.after() >> favourite_function;
    }

  if (args.option("-X",1))
    {
      args.after() >> favourite_function;
      favourite_function_unwrapped=true;
    }

  // Use same keys as used by app to toggle modes
  bool start_fullscreen=args.option("-F");
  bool start_menuhidden=args.option("-M");

  const bool autocool=args.option("-a");

  const bool jitter=args.option("-j");

  uint multisample_level=1;
  if (args.option("-m",1)) args.after() >> multisample_level;

  const bool function_debug_mode=args.option("-D");

  std::clog
    << "Evolvotron version "
    << EVOLVOTRON_BUILD
    << " starting with " 
    << cols 
    << " by " 
    << rows 
    << " display cells and " 
    << threads
    << " compute threads per farm (niceness "
    << niceness_grid
    << " and "
    << niceness_enlargement
    << ")\n";

  EvolvotronMain*const main_widget=new EvolvotronMain
      (
       0,
       QSize(cols,rows),
       frames,
       framerate,
       threads,
       separate_farm_for_enlargements,
       niceness_grid,
       niceness_enlargement,
       start_fullscreen,
       start_menuhidden,
       autocool,
       jitter,
       multisample_level,
       function_debug_mode
       );

  main_widget->mutation_parameters().function_registry().status(std::clog);

  if (!favourite_function.empty())
    {
      std::clog
	<< "Selected specific function: "
	<< favourite_function
	<< (favourite_function_unwrapped ? " (unwrapped)" : " (wrapped)")
	<< "\n";

      if (!main_widget->favourite_function(favourite_function))
	{
	  std::cerr << "Unrecognised function name specified for -x/-X option\n";
	  exit(1);
	}

      main_widget->favourite_function_unwrapped(favourite_function_unwrapped);
    }
  
  app.setMainWidget(main_widget);
  main_widget->show();

  // NB Do this here rather than in constructor so that compute threads aren't being fired off during general GUI set-up
  
  std::clog << "Resetting main widget...\n";
  main_widget->reset_cold();
    
  // NB No need to worry about deleting EvolvotronMain... QApplication seems to do it for us.
  std::clog << "Commencing main loop...\n";
  return app.exec();
}
