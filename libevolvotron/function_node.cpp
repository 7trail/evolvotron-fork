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
  \brief Implementation of class FunctionNode and derived classes.
*/

#include <algorithm>
#include <iostream>

#include "margin.h"

#include "function_node.h"
#include "function_node_info.h"

#include "mutatable_image.h"
#include "matrix.h"

// This is the ONLY place this should be included as FunctionNode::stub will instantiate everything
#include "functions.h"

const std::vector<FunctionNode*> FunctionNode::cloneargs() const
{
  std::vector<FunctionNode*> ret;
  for (std::vector<FunctionNode*>::const_iterator it=args().begin();it!=args().end();it++)
    {
      ret.push_back((*it)->deepclone());
    }
  return ret;
}

const std::vector<float> FunctionNode::cloneparams() const
{
  return params();
}

//! Obtain some statistics about the image function
void FunctionNode::get_stats(uint& total_nodes,uint& total_parameters,uint& depth,uint& width,float& proportion_constant) const
{
  uint total_sub_nodes=0;
  uint total_sub_parameters=0;
  uint max_sub_depth=0;
  uint total_sub_width=0;
  float sub_constants=0.0f;

  // Traverse child nodes.  Need to reconstruct the actual numbers from the proportions
  for (std::vector<FunctionNode*>::const_iterator it=args().begin();it!=args().end();it++)
    {
      uint sub_nodes;
      uint sub_parameters;
      uint sub_depth;
      uint sub_width;
      float sub_proportion_constant;

      (*it)->get_stats(sub_nodes,sub_parameters,sub_depth,sub_width,sub_proportion_constant);

      total_sub_nodes+=sub_nodes;
      total_sub_parameters+=sub_parameters;
      if (sub_depth>max_sub_depth) max_sub_depth=sub_depth;
      total_sub_width+=sub_width;
      sub_constants+=sub_nodes*sub_proportion_constant;
    }

  // And add our own details
  total_nodes=1+total_sub_nodes;  

  total_parameters=params().size()+total_sub_parameters;

  depth=1+max_sub_depth;

  if (total_sub_width==0)
    {
      width=1;
    }
  else 
    {
      width=total_sub_width;
    }

  if (is_constant())
    {
      proportion_constant=1.0f;
    }
  else
    {
      proportion_constant=sub_constants/(1+total_sub_nodes);
    }
}

/*! This returns a random bit of image tree.
  It needs to be capable of generating any sort of node we have.
  \warning Too much probability of highly branching nodes could result in infinite sized stubs.
  \todo Compute (statistically) the expected number of nodes in a stub.
  \todo Don't think FunctionNodeUsing<FunctionPreTransform> appears here ?
 */
FunctionNode*const FunctionNode::stub(const MutationParameters& parameters,bool exciting)
{
  // Base mutations are Constant or Identity types.  
  // (Identity can be Identity or PositionTransformed, proportions depending on identity_supression parameter)
  const float base=0.7;

  uint steps=71;

  if (!parameters.allow_fractal_nodes())
    {
      steps=minimum(steps,67u);     // Currently 4 fractal types
    }

  if (!parameters.allow_iterative_nodes())
    {
      steps=minimum(steps,58u);     // Currently 9 non-fractal iterative types (including multiscale noise - not strictly iterative but expensive)
    }

  const float step=(1.0-base)/steps;

  const float r=(exciting ? base+(1.0f-base)*parameters.r01() : parameters.r01());

  if (r<(1.0f-parameters.proportion_constant())*parameters.identity_supression()*base)
    {
      if (parameters.r01()<0.5f)
	return FunctionTransform::stubnew(parameters,false);
      else
	return FunctionTransformQuadratic::stubnew(parameters,false);
    }
  else if (r<(1.0f-parameters.proportion_constant())*base)
    return FunctionIdentity::stubnew(parameters,false);
  else if (r<base)
    return FunctionConstant::stubnew(parameters,false);
  else if (r<base+1*step)
    return FunctionCartesianToSpherical::stubnew(parameters,false);
  else if (r<base+2*step) 
    return FunctionSphericalToCartesian::stubnew(parameters,false);
  else if (r<base+3*step) 
    return FunctionEvaluateInSpherical::stubnew(parameters,false);
  else if (r<base+4*step) 
    return FunctionRotate::stubnew(parameters,false);
  else if (r<base+5*step) 
    return FunctionSin::stubnew(parameters,false);
  else if (r<base+6*step) 
    return FunctionCos::stubnew(parameters,false);
  else if (r<base+7*step) 
    return FunctionSpiralLinear::stubnew(parameters,false);
  else if (r<base+8*step) 
    return FunctionSpiralLogarithmic::stubnew(parameters,false);
  else if (r<base+9*step) 
    return FunctionGradient::stubnew(parameters,false);
  else if (r<base+10*step) 
    return FunctionComposePair::stubnew(parameters,false);
  else if (r<base+11*step) 
    return FunctionAdd::stubnew(parameters,false);
  else if (r<base+12*step) 
    return FunctionMultiply::stubnew(parameters,false);
  else if (r<base+13*step) 
    return FunctionDivide::stubnew(parameters,false);
  else if (r<base+14*step) 
    return FunctionCross::stubnew(parameters,false);
  else if (r<base+15*step) 
    return FunctionGeometricInversion::stubnew(parameters,false);
  else if (r<base+16*step) 
    return FunctionMax::stubnew(parameters,false);
  else if (r<base+17*step) 
    return FunctionMin::stubnew(parameters,false);
  else if (r<base+18*step) 
    return FunctionModulus::stubnew(parameters,false);
  else if (r<base+19*step) 
    return FunctionExp::stubnew(parameters,false);
  else if (r<base+20*step) 
    return FunctionComposeTriple::stubnew(parameters,false);
  else if (r<base+21*step) 
    return FunctionReflect::stubnew(parameters,false);
  else if (r<base+22*step) 
    return FunctionKaleidoscope::stubnew(parameters,false);
  else if (r<base+23*step) 
    return FunctionKaleidoscopeZRotate::stubnew(parameters,false);
  else if (r<base+24*step) 
    return FunctionKaleidoscopeTwist::stubnew(parameters,false);
  else if (r<base+25*step) 
    return FunctionWindmill::stubnew(parameters,false);
  else if (r<base+26*step) 
    return FunctionWindmillZRotate::stubnew(parameters,false);
  else if (r<base+27*step) 
    return FunctionWindmillTwist::stubnew(parameters,false);
  else if (r<base+28*step) 
    return FunctionMagnitude::stubnew(parameters,false);
  else if (r<base+29*step) 
    return FunctionMagnitudes::stubnew(parameters,false);
  else if (r<base+30*step) 
    return FunctionChooseSphere::stubnew(parameters,false);
  else if (r<base+31*step) 
    return FunctionChooseRect::stubnew(parameters,false);
  else if (r<base+32*step) 
    return FunctionChooseFrom2InCubeMesh::stubnew(parameters,false);
  else if (r<base+33*step) 
    return FunctionChooseFrom3InCubeMesh::stubnew(parameters,false);
  else if (r<base+34*step) 
    return FunctionChooseFrom2InSquareGrid::stubnew(parameters,false);
  else if (r<base+35*step) 
    return FunctionChooseFrom3InSquareGrid::stubnew(parameters,false);
  else if (r<base+36*step) 
    return FunctionChooseFrom2InTriangleGrid::stubnew(parameters,false);
  else if (r<base+37*step) 
    return FunctionChooseFrom3InTriangleGrid::stubnew(parameters,false);
  else if (r<base+38*step) 
    return FunctionChooseFrom3InDiamondGrid::stubnew(parameters,false);
  else if (r<base+39*step) 
    return FunctionChooseFrom3InHexagonGrid::stubnew(parameters,false);
  else if (r<base+40*step) 
    return FunctionChooseFrom2InBorderedHexagonGrid::stubnew(parameters,false);
  else if (r<base+41*step) 
    return FunctionOrthoSphereShaded::stubnew(parameters,false);
  else if (r<base+42*step) 
    return FunctionOrthoSphereShadedBumpMapped::stubnew(parameters,false);
  else if (r<base+43*step) 
    return FunctionOrthoSphereReflect::stubnew(parameters,false);
  else if (r<base+44*step) 
    return FunctionOrthoSphereReflectBumpMapped::stubnew(parameters,false);
  else if (r<base+45*step)
    return FunctionTransformGeneralised::stubnew(parameters,false);
  else if (r<base+46*step)
    return FunctionPreTransform::stubnew(parameters,false);
  else if (r<base+47*step)
    return FunctionPreTransformGeneralised::stubnew(parameters,false);
  else if (r<base+48*step)
    return FunctionPostTransform::stubnew(parameters,false);
  else if (r<base+49*step)
    return FunctionPostTransformGeneralised::stubnew(parameters,false);
  else if (r<base+50*step)
    return FunctionFilter2D::stubnew(parameters,false);
  else if (r<base+51*step)
    return FunctionFilter3D::stubnew(parameters,false);
  else if (r<base+52*step)
    return FunctionShadow::stubnew(parameters,false);
  else if (r<base+53*step)
    return FunctionShadowGeneralised::stubnew(parameters,false);
  else if (r<base+54*step)
    return FunctionCone::stubnew(parameters,false);
  else if (r<base+55*step)
    return FunctionExpCone::stubnew(parameters,false);
  else if (r<base+56*step)
    return FunctionSeparateZ::stubnew(parameters,false);
  else if (r<base+57*step)
    return FunctionNoiseOneChannel::stubnew(parameters,false);
  else if (r<base+58*step)
    return FunctionNoiseThreeChannel::stubnew(parameters,false);
  else if (r<base+59*step)
    return FunctionMultiscaleNoiseOneChannel::stubnew(parameters,false);
  else if (r<base+60*step)
    return FunctionMultiscaleNoiseThreeChannel::stubnew(parameters,false);
  else if (r<base+61*step)
    return FunctionIterate::stubnew(parameters,false);
  else if (r<base+62*step)
    return FunctionAverageSamples::stubnew(parameters,false);
  else if (r<base+63*step)
    return FunctionStreak::stubnew(parameters,false);
  else if (r<base+64*step)
    return FunctionAverageRing::stubnew(parameters,false);
  else if (r<base+65*step)
    return FunctionFilterRing::stubnew(parameters,false);
  else if (r<base+66*step)
    return FunctionConvolveSamples::stubnew(parameters,false);
  else if (r<base+67*step)
    return FunctionAccumulateOctaves::stubnew(parameters,false);
  else if (r<base+68*step)
    return FunctionMandelbrotChoose::stubnew(parameters,false);
  else if (r<base+69*step)
    return FunctionMandelbrotContour::stubnew(parameters,false);
  else if (r<base+70*step)
    return FunctionJuliaChoose::stubnew(parameters,false);
  else //if (r<base+71*step)
    return FunctionJuliaContour::stubnew(parameters,false);
}

/*! If a specific function's registration (ie meta info) is provided then that will be used as the wrapped function type.
 */
FunctionNode*const FunctionNode::initial(const MutationParameters& parameters,const FunctionRegistration* specific_fn)
{
  FunctionNode* root;
  bool image_is_constant;
  
  do
    {
      std::vector<float> params_toplevel;
      std::vector<FunctionNode*> args_toplevel;
      
      {
	const float which=parameters.r01();
	if (which<0.4f)
	  {
	    args_toplevel.push_back(FunctionTransformGeneralised::stubnew(parameters,false));
	  }
	else if (which<0.8f)
	  {
	    args_toplevel.push_back(FunctionTransform::stubnew(parameters,false));
	  }
	else if (which<0.9f)
	  {
	    args_toplevel.push_back(FunctionTransformQuadratic::stubnew(parameters,false));
	  }
	else
	  {
	    args_toplevel.push_back(FunctionIdentity::stubnew(parameters,false));
	  }
      }

      if (specific_fn)
	{
	  args_toplevel.push_back((*(specific_fn->stubnew_fn()))(parameters,true));
	}
      else
	{
	  // This one is crucial: we REALLY want something interesting to happen within here.
	  args_toplevel.push_back(FunctionNode::stub(parameters,true));
	}
      
      {
	const float which=parameters.r01();

	if (which<0.4f)
	  {
	    args_toplevel.push_back(FunctionTransformGeneralised::stubnew(parameters,false));
	  }
	else if (which<0.8f)
	  {
	    args_toplevel.push_back(FunctionTransform::stubnew(parameters,false));
	  }
	else if (which<0.9f)
	  {
	    args_toplevel.push_back(FunctionTransformQuadratic::stubnew(parameters,false));
	  }
	else
	  {
	    args_toplevel.push_back(FunctionIdentity::stubnew(parameters,false));
	  }	
      }

      root=new FunctionComposeTriple(params_toplevel,args_toplevel,0);
      
      assert(root->ok());
      
      image_is_constant=root->is_constant();
      
      if (image_is_constant)
	{
	  delete root;
	}
    }
  while (image_is_constant);
  
  assert(root->ok());
  
  return root;
}

/*! This returns a vector of random bits of stub, used for initialiing nodes with children. 
 */
const std::vector<FunctionNode*> FunctionNode::stubargs(const MutationParameters& parameters,uint n,bool exciting)
{
  std::vector<FunctionNode*> ret;
  for (uint i=0;i<n;i++)
    ret.push_back(stub(parameters,exciting));
  return ret;
}


const std::vector<float> FunctionNode::stubparams(const MutationParameters& parameters,uint n)
{
  std::vector<float> ret;
  for (uint i=0;i<n;i++)
    {
      ret.push_back(-1.0f+2.0f*parameters.r01());
    }
  return ret;
}

const uint FunctionNode::stubiterations(const MutationParameters& parameters)
{
  return 1+static_cast<uint>(floor(parameters.r01()*parameters.max_initial_iterations()));
}

FunctionNode::FunctionNode(const std::vector<float>& p,const std::vector<FunctionNode*>& a,uint iter)
  :_args(a)
   ,_params(p)
   ,_iterations(iter)
{
  assert(ok());
}

/*! Returns null ptr if there's a problem, in which case there will be an explanation in report.
 */
FunctionNode*const FunctionNode::create(const FunctionNodeInfo* info,std::string& report)
{
  const FunctionRegistration*const reg=FunctionRegistry::get()->lookup(info->type());
  if (reg)
    {
      return (*(reg->create_fn()))(info,report);
    }
  else
    {
      report+="Error: Unrecognised function name: "+info->type()+"\n";
      return 0;
    }
}


/*! Deletes all arguments.  No one else should be referencing nodes except the root node of an image.
 */
FunctionNode::~FunctionNode()
{
  assert(ok());
  for (std::vector<FunctionNode*>::iterator it=args().begin();it!=args().end();it++)
    if (*it) delete (*it);
}

/*! There are 2 kinds of mutation:
  - random adjustments to constants 
  - structural mutations (messing with the function tree)
  For structural mutations the obvious things to do are:
  - reordering argsuments
  - dropping argsuments and replacing them with new "stubs".
  - duplicating arguments
  - substituting nodes with other types (can't do this for ourself very easily, but we can do it for children)
  - inserting new nodes between children and ourself

  And of course all children have to be mutated too.
 */
void FunctionNode::mutate(const MutationParameters& parameters)
{
  // First mutate all child nodes.
  for (std::vector<FunctionNode*>::iterator it=args().begin();it!=args().end();it++)
    (*it)->mutate(parameters);
  
  // Perturb any parameters we have
  for (std::vector<float>::iterator it=params().begin();it!=params().end();it++)
    {
      (*it)+=parameters.magnitude()*(-1.0f+2.0f*parameters.r01());
    }

  // Perturb iteration count if any
  if (_iterations)
    {
      if (parameters.r01()<parameters.probability_iterations_change_step())
	{
	  if (parameters.r01()<0.5)
	    {
	      if (_iterations>=2) _iterations--;
	    }
	  else
	    {
	      _iterations++;
	    }
	  if (parameters.r01()<parameters.probability_iterations_change_jump())
	    {
	      if (parameters.r01()<0.5)
		{
		  if (_iterations>1) _iterations=(_iterations+1)/2;
		}
	      else
		{
		  _iterations*=2;
		}
	    }
	  
	  // For safety but shouldn't happen
	  if (_iterations==0) _iterations=1;
	}
    }
      
      
  // Then go to work on the argument structure...
  
  // Think about glitching some nodes.
  for (std::vector<FunctionNode*>::iterator it=args().begin();it!=args().end();it++)
    {
      if (parameters.r01()<parameters.probability_glitch())
	{
	  delete (*it);
	  (*it)=stub(parameters,false);
	}
    }

  // Think about substituting some nodes.
  //! \todo Substitution might make more sense if it was for a node with the same/similar number of arguments.
  for (std::vector<FunctionNode*>::iterator it=args().begin();it!=args().end();it++)
    {
      if (parameters.r01()<parameters.probability_substitute())
	{
	  // Take a copy of the nodes parameters and arguments
	  std::vector<FunctionNode*> a((*it)->deepclone_args());
	  std::vector<float> p((*it)->params());
	  
	  // Replace the node with something interesting (maybe this should depend on how complex the original node was)
	  delete (*it);
	  (*it)=stub(parameters,true);

	  // Do we need some extra arguments ?
	  if (a.size()<(*it)->args().size())
	    {
	      const std::vector<FunctionNode*> xa(stubargs(parameters,(*it)->args().size()-a.size()));
	      a.insert(a.end(),xa.begin(),xa.end());
	    }
	  // Shuffle them
	  std::random_shuffle(a.begin(),a.end());
	  // Have we go too many arguments ?
	  while (a.size()>(*it)->args().size())
	    {
	      delete (a.back());
	      a.pop_back();
	    }
	  
	  // Do we need some extra parameters ?
	  if (p.size()<(*it)->params().size())
	    {
	      const std::vector<float> xp(stubparams(parameters,(*it)->params().size()-p.size()));
	      p.insert(p.end(),xp.begin(),xp.end());
	    }
	  // Shuffle them
	  std::random_shuffle(p.begin(),p.end());
	  // Have we go too many arguments ?
	  while (p.size()>(*it)->params().size())
	    {
	      p.pop_back();
	    }

	  // Impose the new parameters and arguments on the new node (iterations not touched)
	  (*it)->impose(p,a);
	}
    }
  
  // Think about randomising child order
  if (parameters.r01()<parameters.probability_shuffle())
    {
      // This uses rand() (would rather use our own one).
      // This bit of STL seems a bit up in the air (at least in GNU implementation), but it works so who cares.
      std::random_shuffle(args().begin(),args().end());
    }

  // Think about inserting a random stub between us and some children
  for (std::vector<FunctionNode*>::iterator it=args().begin();it!=args().end();it++)
    {
      if (parameters.r01()<parameters.probability_insert())
	{
	  std::vector<float> p;
	  std::vector<FunctionNode*> a;

	  a.push_back((*it));
	  a.push_back(stub(parameters,false));

	  (*it)=new FunctionComposePair(p,a,0);
	}
    }
}

void FunctionNode::simplify_constants() 
{
  for (std::vector<FunctionNode*>::iterator it=args().begin();it!=args().end();it++)
    {
      if ((*it)->is_constant())
	{
	  const XYZ v((*(*it))(XYZ(0.0f,0.0f,0.0f)));
	  std::vector<float> vp;
	  vp.push_back(v.x());
	  vp.push_back(v.y());
	  vp.push_back(v.z());
	  std::vector<FunctionNode*> va; 
	  delete (*it);
	  (*it)=new FunctionConstant(vp,va,0);
	}
      else
	{
	  (*it)->simplify_constants();
	}
    }
}

const std::vector<FunctionNode*> FunctionNode::deepclone_args() const
{
  std::vector<FunctionNode*> ret;
  for (std::vector<FunctionNode*>::const_iterator it=args().begin();it!=args().end();it++)
    ret.push_back((*it)->deepclone());
  return ret;
}

void FunctionNode::impose(std::vector<float>& p,std::vector<FunctionNode*>& a)
{
  for (std::vector<FunctionNode*>::iterator it=args().begin();it!=args().end();it++)
    delete (*it);
  args().clear();

  args()=a;
  params()=p;

  assert(ok());
}


const FunctionPreTransform*const FunctionNode::is_a_FunctionPreTransform() const
{
  return 0;
}

FunctionPreTransform*const FunctionNode::is_a_FunctionPreTransform()
{
  return 0;
}

const FunctionPostTransform*const FunctionNode::is_a_FunctionPostTransform() const
{
  return 0;
}

FunctionPostTransform*const FunctionNode::is_a_FunctionPostTransform()
{
  return 0;
}

const bool FunctionNode::ok() const
{
  // Args vector should never contain a null or a non-ok argument
  for (std::vector<FunctionNode*>::const_iterator it=args().begin();it!=args().end();it++)
    {
      if ((*it)==0 || !(*it)->ok())
	return false;
    }
  return true;
}

/*! This function only saves the parameters, iteration count if any and child nodes.
  It is intended to be called from save_function of subclasses which will write a function node wrapper.
  The indent number is just the level of recursion, incrementing by 1 each time.
  Outputting multiple spaces per level is handled by the Margin class.
 */
std::ostream& FunctionNode::save_function(std::ostream& out,uint indent) const
{
  if (iterations()!=0)
    out << Margin(indent) << "<i>" << iterations() << "</i>\n";
  
  for (std::vector<float>::const_iterator it=params().begin();it!=params().end();it++)
    {
      out << Margin(indent) << "<p>" << (*it) << "</p>\n";
    }

  for (std::vector<FunctionNode*>::const_iterator it=args().begin();it!=args().end();it++)
    {
      (*it)->save_function(out,indent);
    }
    
  return out;
}
