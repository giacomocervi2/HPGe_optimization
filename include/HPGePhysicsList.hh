#ifndef HPGePHYSICSLISTS_HH
#define HPGePHYSICSLISTS_HH

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4DecayPhysics.hh"

class HPGePhysicsList : public G4VModularPhysicsList
{
	public:
		HPGePhysicsList();
		~HPGePhysicsList();

};

#endif
