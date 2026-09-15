#include "HPGeMessenger.hh"

DetectorMessenger::DetectorMessenger(HPGeDetectorConstruction *det) : fDetector(det) {

    fDirectory = new G4UIdirectory("/mycmd");
    fDirectory->SetGuidance("Detector geometry control commands");

    //fDetLengthCmd
    fDetLengthCmd = new G4UIcmdWithADoubleAndUnit("/mycmd/setDetLength", this);
    fDetLengthCmd->SetGuidance("Set HPGe length");
    fDetLengthCmd->SetParameterName("DetLength", false);
    fDetLengthCmd->SetRange("DetLength>0.");
    fDetLengthCmd->SetDefaultUnit("mm");
    fDetLengthCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    //fDetRadiusCmd
    fDetRadiusCmd = new G4UIcmdWithADoubleAndUnit("/mycmd/setDetRadius", this);
    fDetRadiusCmd->SetGuidance("Set HPGe radius");
    fDetRadiusCmd->SetParameterName("DetRadius", false);
    fDetRadiusCmd->SetRange("DetRadius>0.");
    fDetRadiusCmd->SetDefaultUnit("mm");
    fDetRadiusCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    //fHoleRadiusCmd
    fHoleRadiusCmd = new G4UIcmdWithADoubleAndUnit("/mycmd/setHoleRadius", this);
    fHoleRadiusCmd->SetGuidance("Set hole radius");
    fHoleRadiusCmd->SetParameterName("HoleRadius", false);
    fHoleRadiusCmd->SetRange("HoleRadius>0.");
    fHoleRadiusCmd->SetDefaultUnit("mm");
    fHoleRadiusCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    //fHoleLengthCmd
    fHoleLengthCmd = new G4UIcmdWithADoubleAndUnit("/mycmd/setHoleLength", this);
    fHoleLengthCmd->SetGuidance("Set hole length");
    fHoleLengthCmd->SetParameterName("HoleLength", false);
    fHoleLengthCmd->SetRange("HoleLength>0.");
    fHoleLengthCmd->SetDefaultUnit("mm");
    fHoleLengthCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    //fUpdateCmd
    fUpdateCmd = new G4UIcmdWithoutParameter("/mycmd/update", this);
    fUpdateCmd->SetGuidance("Force geometry update");
    fUpdateCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

DetectorMessenger::~DetectorMessenger() {
    delete fDetLengthCmd;
    delete fDetRadiusCmd;
    delete fHoleRadiusCmd;
    delete fHoleLengthCmd;
    delete fUpdateCmd;
    delete fDirectory;
    
}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
    if (command == fDetLengthCmd) {
        G4double val = fDetLengthCmd->GetNewDoubleValue(newValue);
        fDetector->SetDetectorLength(val);
    } 

    else if (command == fDetRadiusCmd){
        G4double val = fDetRadiusCmd->GetNewDoubleValue(newValue);
        fDetector->SetDetectorRadius(val);
    }

    else if (command == fHoleRadiusCmd) {
        G4double val = fHoleRadiusCmd->GetNewDoubleValue(newValue);
        fDetector->SetHoleRadius(val);
    } 
    else if (command == fHoleLengthCmd) {
        G4double val = fHoleLengthCmd->GetNewDoubleValue(newValue);
        fDetector->SetHoleLength(val);
    }
    else if (command == fUpdateCmd) {
        fDetector->UpdateGeometry();
    }

}
