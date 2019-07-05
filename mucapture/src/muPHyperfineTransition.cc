#include "muPHyperfineTransition.hh"

#include "G4MuAtom.hh"

#if G4VERSION<4095
#include <strstream>
#endif

// singlet to triplet

muPHyperfineStoT::
muPHyperfineStoT(G4MuAtom const* p, G4double transitionRate, G4int verboseLevel) :
  G4VMuAtomCaptureKineticsChannel(p, "muP singlet/triplet transition", 
				  transitionRate,verboseLevel) {
  CheckIsApplicable();
}

G4DecayProducts* muPHyperfineStoT::CaptureIt(G4DynamicParticle const* part){
  G4DecayProducts* products = new G4DecayProducts;

  // Set the parent
  products->SetParentParticle(*part);

  G4MuAtom* triplet = G4MuAtom::MuAtom(1,1,2);
  G4ThreeVector v;
  G4DynamicParticle* p = new G4DynamicParticle(triplet, v);
  products->PushProducts(p);
  
  return products;
}

void muPHyperfineStoT::CheckIsApplicable() const{
  if( part->GetParticleName().substr(0,4) != "mu_P" ){
#if G4VERSION<4095
    std::ostrstream ed;
#else
    G4ExceptionDescription ed;
#endif
    ed << "Channel " << GetChannelName() << " is not applicable to particle "
       << part->GetParticleName();
    G4Exception("muPHyperfineStoT::CheckIsApplicable",
                "MUPH0001", FatalException, 
#if G4VERSION<4095
                ed.str());
#else
                ed);
#endif
    }
}

// triplet to singlet
muPHyperfineTtoS::
muPHyperfineTtoS(G4MuAtom const* p, G4double transitionRate, G4int verboseLevel) :
  G4VMuAtomCaptureKineticsChannel(p,"muP triplet/singlet transition", 
				  transitionRate,verboseLevel) {
  CheckIsApplicable();
}

G4DecayProducts* muPHyperfineTtoS::CaptureIt(G4DynamicParticle const* part){
  G4DecayProducts* products = new G4DecayProducts;

  // Set the parent
  products->SetParentParticle(*part);

  G4MuAtom* singlet = G4MuAtom::MuAtom(1,1,0);
  G4ThreeVector v;
  G4DynamicParticle* p = new G4DynamicParticle(singlet, v);
  products->PushProducts(p);
  
  return products;
}

void muPHyperfineTtoS::CheckIsApplicable() const{
  if( part->GetParticleName().substr(0,4) != "mu_P" ){
#if G4VERSION<4095
    std::ostrstream ed;
#else
    G4ExceptionDescription ed;
#endif
    ed << "Channel " << GetChannelName() << " is not applicable to particle "
       << part->GetParticleName();
    G4Exception("muPHyperfineTtoS::CheckIsApplicable",
                "MUPH0002", FatalException, 
#if G4VERSION<4095
                ed.str());
#else
                ed);
#endif
  }
}
