#ifndef HPGeDETECTORCONSTRUCTION_HH
#define HPGeDETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"

#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4SDManager.hh"

#include "HPGeSensitiveDetector.hh"
#include "CADMesh.hh"

#include "G4GenericMessenger.hh"
#include "G4SolidStore.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"

class DetectorMessenger;

class HPGeDetectorConstruction : public G4VUserDetectorConstruction
{
public:
	HPGeDetectorConstruction();
	virtual ~HPGeDetectorConstruction();

	virtual G4VPhysicalVolume *Construct() override;

	void SetDetectorLength(G4double val); 
	void SetDetectorRadius(G4double val);
	void SetHoleRadius(G4double val);
	void SetHoleLength(G4double val);

	void UpdateGeometry();

private:
	G4LogicalVolume *logicDetector;
	G4double fDetLength;
	G4double fDetRadius;
	G4double fHoleLength;
	G4double fHoleRadius;

	DetectorMessenger *fMessenger;

	virtual void ConstructSDandField();
};


#endif
