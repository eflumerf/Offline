//
// conversion electron/positron spectrum, radiatively corrected
// 
//
// Mu2e includes
#include "GlobalConstantsService/inc/GlobalConstantsHandle.hh"
#include "GlobalConstantsService/inc/PhysicsParams.hh"
#include "GlobalConstantsService/inc/ParticleDataTable.hh"
#include "Mu2eUtilities/inc/ConversionSpectrum.hh"
#include "Mu2eUtilities/inc/RandomUnitSphere.hh"

// Framework includes
#include "cetlib/pow.h"

// CLHEP includes
#include "CLHEP/Random/RandFlat.h"

// C++ includes
#include <iostream>
#include <math.h>

//GSL includes
#include "stdio.h"
#include "gsl/gsl_integration.h"

using namespace std;

  
namespace mu2e {
 
  ConversionSpectrum::ConversionSpectrum(double maxEnergy, double bin, int RadCorrected) :
    _bin          (bin         ),
    _spectrumType (RadCorrected) 
  {
    GlobalConstantsHandle<ParticleDataTable> pdt;

    _par.me     = pdt->particle(PDGCode::e_minus ).ref().mass().value();
    _par.alpha = 1./137.035999139;
    _par.eMax  = maxEnergy;
  
    // calculate integral.... for n-1 bins;
     _integral = evalIntegral(bin); 

  }


//-----------------------------------------------------------------------------      
  double ConversionSpectrum::my_f(double E, void *p) { 
    double eMax  = ((ConversionSpectrum::Params_t*) p)->eMax;
    double me    = ((ConversionSpectrum::Params_t*) p)->me;
    double alpha = ((ConversionSpectrum::Params_t*) p)->alpha;

    double f     = (1./eMax)*(alpha/(2*M_PI))*(log(4*E*E/me/me)-2.)*((E*E+eMax*eMax)/eMax/(eMax-E));
    return  f;
  }

//-----------------------------------------------------------------------------  
  double ConversionSpectrum::getCorrectedConversionSpectrum(double e) const {
    return ConversionSpectrum::my_f(e,(void*) &_par);
  }

//-----------------------------------------------------------------------------  
  double ConversionSpectrum::getWeight(double E) const {
    
    double weight(0.);
  

    int bin    = E/_bin   ; 
    int _nbins = (_par.eMax/_bin)   ;

    if (bin < _nbins) {
      weight = 0.1* getCorrectedConversionSpectrum(E);
    }
    else {
      // last bin
      weight =( 1.-_integral);
    }
    
    return weight;
  }

//-----------------------------------------------------------------------------  
  double ConversionSpectrum::evalIntegral(double de){
    gsl_function F;
    F.function = &my_f;
    F.params   = &_par;

    size_t limit  = 1000;
    double epsabs = 0.001;
    double epsrel = 0.001;
  
    gsl_integration_workspace * ws = gsl_integration_workspace_alloc(10000);

    double result, abserr;

    double emin = 0.72;
    double emax = _par.eMax-de;

    gsl_integration_qags(&F,
			 emin, emax,
			 epsabs,
			 epsrel,
			 limit,
			 ws,
			 &result,
			 &abserr);

    gsl_integration_workspace_free(ws);
    std::cout<<"il valore dell'integrale fino al penultimo bin e' "<< result<<std::endl;
    return result;


  }
}