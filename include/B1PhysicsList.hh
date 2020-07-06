#ifndef B1PhysicsList_h
#define B1PhysicsList_h

#include "B1DetectorConstruction.hh"
#include "G4VModularPhysicsList.hh"
#include "globals.hh"
#include <vector>

class G4PhysicsConstructor;
class B1DetectorConstruction;

class B1PhysicsList: public G4VModularPhysicsList
{
  public:
    B1PhysicsList(B1DetectorConstruction* det);
    virtual ~B1PhysicsList();
    void ConstructProcess();

  private:
    G4String              fEmName;
    G4VPhysicsConstructor*  fEmPhysicsList;
    G4VPhysicsConstructor*  fDecPhysicsList;
    G4VPhysicsConstructor*  fHadPhysicsList;
    G4VPhysicsConstructor*  fRaddecayList;

    B1DetectorConstruction* fDetector;
};

#endif
