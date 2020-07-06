#include "B1PhysicsList.hh"

#include "G4EmStandardPhysics_option3.hh"
#include "G4DecayPhysics.hh"
#include "G4Decay.hh"
#include "G4StepLimiter.hh"
#include "G4LossTableManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ProcessManager.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"

#include "G4EmExtraPhysics.hh"
#include "G4EmParameters.hh"
#include "G4NuclideTable.hh"

#include "G4HadronPhysicsQGSP_BERT.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_AllHP.hh"

#include "G4RadioactiveDecayPhysics.hh"
#include "G4PhysListFactory.hh"
#include "G4DeexPrecoParameters.hh"

B1PhysicsList::B1PhysicsList(B1DetectorConstruction* det)
  : G4VModularPhysicsList()
{
  // add new units for radioactive decays

  new G4UnitDefinition("millielectronVolt", "meV", "Energy", 1.-3*eV);
  const G4double minute =  60 * second;
  const G4double hour   =  60 * minute;
  const G4double day    =  24 * hour;
  const G4double year   = 365 * day;
  new G4UnitDefinition("minute",  "min",  "Time", minute);
  new G4UnitDefinition("hour",    "h",    "Time", hour);
  new G4UnitDefinition("day",     "d",    "Time", day);
  new G4UnitDefinition("year",    "y",    "Time", year);

  // Mandatory for G4NuclideTable
  // Half-life threshold must be set small or many short-lived isomers
  // will not be assigned life times (default to 0)
  G4NuclideTable::GetInstance()->SetThresholdOfHalfLife(0.1*second);
  G4NuclideTable::GetInstance()->SetLevelTolerance(1.0*eV);

  //---
  fDetector = det;

  G4LossTableManager::Instance();

  // place your cut values here:
  
  // EM Physics
  fEmPhysicsList = new G4EmStandardPhysics_option3(0);
  fEmName = G4String("emstandard_opt3");

  // Decay physics and all particles
  fDecPhysicsList = new G4DecayPhysics(0);
  fRaddecayList   = new G4RadioactiveDecayPhysics(0);

  // Hadron physics
  fHadPhysicsList = new G4HadronPhysicsQGSP_BIC_AllHP(0);

}

B1PhysicsList::~B1PhysicsList()
{
  delete fEmPhysicsList;
  delete fDecPhysicsList;
  delete fRaddecayList;
  delete fHadPhysicsList;
}

void B1PhysicsList::ConstructProcess()
{
  // Define transportation process
  AddTransportation();

  // Electromagnetic physics list
  fEmPhysicsList->ConstructProcess();

  // Decay physics list
  fDecPhysicsList->ConstructProcess();
  fRaddecayList->ConstructProcess();

  // Hadronic physics lists
  fHadPhysicsList->ConstructProcess();

  // 
}
