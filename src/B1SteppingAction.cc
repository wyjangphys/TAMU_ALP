//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1SteppingAction.cc
/// \brief Implementation of the B1SteppingAction class

#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "B1DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleTypes.hh"
#include "g4root.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::B1SteppingAction(B1EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::~B1SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1SteppingAction::UserSteppingAction(const G4Step* step)
{
  if (!fScoringVolume) {
    const B1DetectorConstruction* detectorConstruction
      = static_cast<const B1DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume();
  }

  // get volume of the current step
  G4LogicalVolume* volume
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();

  // check if we are in scoring volume
  //if (volume != fScoringVolume) return;

  // collect energy deposited in this step
  G4double edepStep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edepStep);

  //G4Track* track = step->GetTrack();
  //G4cout << track->GetParticleDefinition()->GetParticleName() << G4std::endl;
  //G4cout << "Total Energy: " << track->GetKineticEnergy() << G4std::endl;
  //G4cout << "Edep: " << edepStep << G4std::endl;

  //G4String processName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  //G4cout << step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() << G4std::endl;
  //if( step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "eBrem" )
  //  G4cout << step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() << G4std::endl;
  auto analysisManager = G4AnalysisManager::Instance();

  const G4TrackVector* secondary = step->GetSecondary();
  for( size_t lp=0; lp < (*secondary).size(); lp++ )
  {
    analysisManager->FillNtupleSColumn(0, step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName());
    analysisManager->FillNtupleDColumn(1, (G4double)(*secondary)[lp]->GetTotalEnergy()/CLHEP::keV);
    analysisManager->FillNtupleDColumn(2, (G4double)(*secondary)[lp]->GetPosition().getX());
    analysisManager->FillNtupleDColumn(3, (G4double)(*secondary)[lp]->GetPosition().getY());
    analysisManager->FillNtupleDColumn(4, (G4double)(*secondary)[lp]->GetPosition().getZ());
    analysisManager->FillNtupleDColumn(5, (G4double)(*secondary)[lp]->GetMomentum().getX()/CLHEP::keV);
    analysisManager->FillNtupleDColumn(6, (G4double)(*secondary)[lp]->GetMomentum().getY()/CLHEP::keV);
    analysisManager->FillNtupleDColumn(7, (G4double)(*secondary)[lp]->GetMomentum().getZ()/CLHEP::keV);
    analysisManager->FillNtupleDColumn(8, (G4double)(*secondary)[lp]->GetMomentumDirection().getX());
    analysisManager->FillNtupleDColumn(9, (G4double)(*secondary)[lp]->GetMomentumDirection().getY());
    analysisManager->FillNtupleDColumn(10, (G4double)(*secondary)[lp]->GetMomentumDirection().getZ());
    analysisManager->FillNtupleSColumn(11, (*secondary)[lp]->GetDefinition()->GetParticleName() );
    analysisManager->FillNtupleDColumn(12, (volume == fScoringVolume?1:0));
    analysisManager->AddNtupleRow();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

