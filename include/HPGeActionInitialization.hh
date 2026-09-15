#ifndef HPGeACTIONINITIALIZATION_HH
#define HPGeACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"

#include "HPGePrimaryGenerator.hh"
#include "HPGeRunAction.hh"

class HPGeActionInitialization : public G4VUserActionInitialization
{
public:
	HPGeActionInitialization();
	~HPGeActionInitialization();

	virtual void BuildForMaster() const;
	virtual void Build() const;

};


#endif
