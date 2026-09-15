#include "HPGePrimaryGenerator.hh"

HPGePrimaryGenerator::HPGePrimaryGenerator()
{
	fParticleSource = new G4GeneralParticleSource();


}

HPGePrimaryGenerator::~HPGePrimaryGenerator()
{
	delete fParticleSource;
}

void HPGePrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
	fParticleSource->GeneratePrimaryVertex(anEvent);
}
