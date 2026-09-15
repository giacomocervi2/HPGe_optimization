#ifndef HPGE_MESSENGER_HH
#define HPGE_MESSENGER_HH

#include "G4UImessenger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "globals.hh"
#include "HPGeDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"


class DetectorMessenger : public G4UImessenger {
public:
    DetectorMessenger(HPGeDetectorConstruction* det);
    virtual ~DetectorMessenger();

    virtual void SetNewValue(G4UIcommand* command, G4String newValue) override;

private:
    HPGeDetectorConstruction *fDetector;
    G4UIdirectory *fDirectory;
    G4UIcmdWithADoubleAndUnit *fDetLengthCmd;
    G4UIcmdWithADoubleAndUnit *fDetRadiusCmd;
    G4UIcmdWithADoubleAndUnit *fHoleLengthCmd;
    G4UIcmdWithADoubleAndUnit *fHoleRadiusCmd;
    G4UIcmdWithoutParameter *fUpdateCmd;
};

#endif
