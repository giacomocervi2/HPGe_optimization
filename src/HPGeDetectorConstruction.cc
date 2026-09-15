#include "HPGeDetectorConstruction.hh"
#include "HPGeMessenger.hh"

HPGeDetectorConstruction::HPGeDetectorConstruction() : G4VUserDetectorConstruction(), fDetLength(27. * mm), fDetRadius(28.5 * mm), fHoleLength(21. * mm), fHoleRadius(6. * mm)
{
	DetectorMessenger *fMessenger= new DetectorMessenger(this);
}

HPGeDetectorConstruction::~HPGeDetectorConstruction()
{
	delete fMessenger;
}

G4VPhysicalVolume *HPGeDetectorConstruction::Construct()
{
	G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();
	
	G4bool checkOverlaps = true;

	//materials definition
	G4NistManager *nist = G4NistManager::Instance();
	G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
	G4Material *AlMat = nist->FindOrBuildMaterial("G4_Al");
	G4Material *windowMat = nist->FindOrBuildMaterial("G4_Be");
	G4Material *plasticMat = nist->FindOrBuildMaterial("G4_PLEXIGLASS");
	G4Material *detMat = new G4Material("detMat", 32., 72.64*g/mole, 5.323*g/cm3);
	G4Material *LitMat = nist->FindOrBuildMaterial("G4_Li");
	G4Material *holeMat = nist->FindOrBuildMaterial("G4_Cu");

	//Vacuum
	G4double atomicNumber = 1.;
	G4double massOfMole = 1.008*g/mole;
	G4double density = 1.e-25*g/cm3;
	G4double temperature = 2.73*kelvin;
	G4double pressure = 3.e-18*pascal;
	G4Material *vacuumMat = new G4Material("vacuumMat", atomicNumber, massOfMole, density, kStateGas,temperature, pressure);

	
	//World 
	G4double xWorld = 1. * m;
	G4double yWorld = 1. * m;
	G4double zWorld = 1. * m;

	G4Box *solidWorld = new G4Box("solidworld",0.5 * xWorld, 0.5 * yWorld, 0.5 * zWorld);
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicalVolume");
	G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);


	
	//Aluminum placement
	G4double Al_z = 60 * mm;
	G4double Al_rMin = 0. * mm;
	G4double Al_rMax = 38. * mm;
	G4double Al_SPhi = 0. * deg;
	G4double Al_DPhi = 360. * deg;
	
	G4Tubs *solidAl = new G4Tubs("solidAl", Al_rMin, Al_rMax, Al_z, Al_SPhi, Al_DPhi);
	G4LogicalVolume *logicAl = new G4LogicalVolume(solidAl, AlMat, "logicAl");
	G4VPhysicalVolume *physAl = new G4PVPlacement(0, G4ThreeVector(0., 0., -Al_z), logicAl, "physAl", logicWorld, false, 0, checkOverlaps);

	G4VisAttributes *AlVisAtt = new G4VisAttributes(G4Color(1.0, 0.0, 0.0, 0.5));
	AlVisAtt->SetForceSolid(true);
	logicAl->SetVisAttributes(AlVisAtt);

	//Carbon window
	G4double C_z = 0.3 *mm; 
	G4double C_rMin = 0. * mm;
	G4double C_rMax = 38. * mm;
	G4double C_SPhi = 0. * deg;
	G4double C_DPhi = 360. * deg;

	G4Tubs *solidC = new G4Tubs("solidC", C_rMin, C_rMax, C_z, C_SPhi, C_DPhi);
	G4LogicalVolume *logicC = new G4LogicalVolume(solidC, windowMat, "logicC");
	G4VPhysicalVolume *physC = new G4PVPlacement(0, G4ThreeVector(0., 0., Al_z - C_z), logicC, "physC", logicAl, false, 0, checkOverlaps);
	
	G4VisAttributes *CVisAtt = new G4VisAttributes(G4Color(0.5, 0.5, 0.5, 1.));
	CVisAtt->SetForceSolid(true);
	logicC->SetVisAttributes(CVisAtt);

	
	//vacuum placement
	G4double vac_z = fDetLength + 4 * mm;
	G4double vac_rMin = 0. * mm;
	G4double vac_rMax = 36.5 * mm; 
	G4double vac_SPhi = 0. * deg;
	G4double vac_DPhi = 360. * deg;

	G4Tubs *solidVac = new G4Tubs("solidVac", vac_rMin, vac_rMax, vac_z, vac_SPhi, vac_DPhi);
	G4LogicalVolume *logicVac = new G4LogicalVolume(solidVac, vacuumMat, "logicVac");
	G4VPhysicalVolume *physVac = new G4PVPlacement(0, G4ThreeVector(0., 0., Al_z - vac_z - 2 * C_z - 0.75 * mm), logicVac, "physVac", logicAl, false, 0, checkOverlaps);

	

	//HPGe crystal placement
	G4double HPGe_z = fDetLength;
	G4double HPGe_rMin = 0. * mm;
	G4double HPGe_rMax = fDetRadius; 
	G4double HPGe_SPhi = 0. * deg;
	G4double HPGe_DPhi = 360. * deg;
	
	G4Tubs *solidDetector = new G4Tubs("solidDetector", HPGe_rMin, HPGe_rMax, HPGe_z, HPGe_SPhi, HPGe_DPhi);
	logicDetector = new G4LogicalVolume(solidDetector, detMat, "logicDetector");
	G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicDetector, "physDetector", logicVac, false, 0, checkOverlaps);

	G4VisAttributes *detVisAtt = new G4VisAttributes(G4Color(1.0, 1., 0.0, 1.0));
	detVisAtt->SetForceSolid(true);
	logicDetector->SetVisAttributes(detVisAtt);





	//hole
	G4double hole_length = fHoleLength; 
	G4double hole_rMin = 0. * mm;
	G4double hole_rMax = fHoleRadius; 
	G4double hole_SPhi = 0. * deg;
	G4double hole_DPhi = 360. * deg;

	G4Tubs *solidHole = new G4Tubs("solidHole", hole_rMin, hole_rMax, hole_length, hole_SPhi, hole_DPhi);
	G4LogicalVolume *logicHole = new G4LogicalVolume(solidHole, holeMat, "logicHole");
	G4VPhysicalVolume *physHole = new G4PVPlacement(0, G4ThreeVector(0., 0., - HPGe_z + hole_length), logicHole, "physHole", logicDetector, false, 0, checkOverlaps);


	
	//Lithium
	G4double Li_rMax = hole_rMax + 1. * mm; 
	
	G4Tubs *solidLi = new G4Tubs("solidLi", hole_rMax, Li_rMax, hole_length, hole_SPhi, hole_DPhi);
	G4LogicalVolume *logicLi = new G4LogicalVolume(solidLi, LitMat, "logicLi");
	G4VPhysicalVolume *physLi = new G4PVPlacement(0, G4ThreeVector(0., 0., -HPGe_z + hole_length), logicLi, "physLi", logicDetector, false, 0, checkOverlaps);
	
	G4double Li_z = 0.5 * mm;
	G4Tubs *solidLi2 = new G4Tubs("solidLi2", 0., Li_rMax, Li_z, hole_SPhi, hole_DPhi);
	G4LogicalVolume *logicLi2 = new G4LogicalVolume(solidLi2, LitMat, "logicLi2");
	G4VPhysicalVolume *physLi2 = new G4PVPlacement(0, G4ThreeVector(0., 0., -HPGe_z + 2 * hole_length + Li_z), logicLi2, "physLi2", logicDetector, false, 0, checkOverlaps);

	

	//front plastic layer
	G4double front_z = 1.6 * mm;
	G4double rmin_front = Al_rMax;
	G4double rmax_front = Al_rMax + 3.9 * mm;
	G4double front_length = 25.1 / 2. * mm;

	G4Tubs *solidFront = new G4Tubs("solidFront", 0., rmax_front, front_z, Al_SPhi, Al_DPhi);
	G4LogicalVolume *logicFront = new G4LogicalVolume(solidFront, plasticMat, "logicfront");
	G4VPhysicalVolume *physFront = new G4PVPlacement(0, G4ThreeVector(0.,0.,front_z), logicFront, "physfront", logicWorld, false, 0, checkOverlaps);

	G4Tubs *solidFrontSide = new G4Tubs("solidFrontSide", rmin_front, rmax_front, front_length, Al_SPhi, Al_DPhi);
	G4LogicalVolume *logicFrontSide = new G4LogicalVolume(solidFrontSide, plasticMat, "logicFrontSide");
	G4VPhysicalVolume *physFrontSide = new G4PVPlacement(0, G4ThreeVector(0.,0.,-front_length), logicFrontSide, "physFrontSide", logicWorld, false, 0, checkOverlaps);



	//source placement
	G4double thickness = 1 * mm; 
	G4double side1 = 0.5 * cm;
	G4double side2 = 1 * cm;
	G4Box *solidPlastic = new G4Box("solidPlastic", thickness, side1, side2);
	G4LogicalVolume *logicPlastic = new G4LogicalVolume(solidPlastic, plasticMat, "logicPlastic");
	G4VPhysicalVolume *physPlastic = new G4PVPlacement(0, G4ThreeVector(-1.3 * cm, 0., 8.5 * cm), logicPlastic,"physPlastic", logicWorld, false, 0, checkOverlaps);



	return physWorld;

}
void HPGeDetectorConstruction::SetDetectorLength(G4double val) {
    fDetLength = val;
}

void HPGeDetectorConstruction::SetDetectorRadius(G4double val) {
    fDetRadius = val;
}

void HPGeDetectorConstruction::SetHoleRadius(G4double val) {
    fHoleRadius = val;
}

void HPGeDetectorConstruction::SetHoleLength(G4double val) {
    fHoleLength = val;
}

void HPGeDetectorConstruction::UpdateGeometry() {
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void HPGeDetectorConstruction::ConstructSDandField()
{
	HPGeSensitiveDetector *sensDet = new HPGeSensitiveDetector("SensitiveDetector");
	logicDetector->SetSensitiveDetector(sensDet);
	G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
}

