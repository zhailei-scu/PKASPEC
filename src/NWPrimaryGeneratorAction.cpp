#include "NWPrimaryGeneratorAction.h"
#include "G4ParticleTable.hh"
#include "G4Exception.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "NWGlobal.h"


NWPrimaryGeneratorAction::NWPrimaryGeneratorAction() {
	this->particleGun = new G4ParticleGun();


	/*Here, we get the particle define from the registered physical model*/
	this->particleDef = G4ParticleTable::GetParticleTable()->FindParticle(NWGlobal::GetInstance()->gunParticleName);

	if (NULL == particleDef) {
		G4ExceptionDescription msg;
		msg << "The particle : " << NWGlobal::GetInstance()->gunParticleName << "cannot be finded!";
		G4Exception("NWPrimaryGeneratorAction::GeneratePrimaries", "Code01", FatalException, msg);

		system("pause");
		exit(1);
	}

	particleGun->SetParticleDefinition(particleDef);
}

NWPrimaryGeneratorAction::NWPrimaryGeneratorAction(const NWPrimaryGeneratorAction &r) {

	if (NULL != &r) {
		if (this->particleGun) delete this->particleGun;
		this->particleGun = NULL;

		if (r.particleGun) {
			
			this->particleGun = new G4ParticleGun();
			this->particleGun->SetParticleDefinition(r.particleGun->GetParticleDefinition());
			this->particleGun->SetNumberOfParticles(r.particleGun->GetNumberOfParticles());
			this->particleGun->SetParticleMomentumDirection(r.particleGun->GetParticleMomentumDirection());
			this->particleGun->SetParticleEnergy(r.particleGun->GetParticleEnergy());
			this->particleGun->SetParticlePosition(r.particleGun->GetParticlePosition());
		}
	}

}


NWPrimaryGeneratorAction NWPrimaryGeneratorAction::operator=(const NWPrimaryGeneratorAction &r) {

	if (NULL != &r) {
		if (this->particleGun) delete this->particleGun;
		this->particleGun = NULL;

		if (r.particleGun) {

			this->particleGun = new G4ParticleGun();
			this->particleGun->SetParticleDefinition(r.particleGun->GetParticleDefinition());
			this->particleGun->SetNumberOfParticles(r.particleGun->GetNumberOfParticles());
			this->particleGun->SetParticleMomentumDirection(r.particleGun->GetParticleMomentumDirection());
			this->particleGun->SetParticleEnergy(r.particleGun->GetParticleEnergy());
			this->particleGun->SetParticlePosition(r.particleGun->GetParticlePosition());
		}
	}


	return *this;
}


NWPrimaryGeneratorAction::~NWPrimaryGeneratorAction() {
	if (particleGun) {
		delete particleGun;
	}

	particleDef = NULL;
}

void NWPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {

	NWGlobal::GetInstance()->particleOriginDirection = G4ThreeVector(0, 0, -1);
	
	/*Set the particle gun for this event*/
	particleGun->SetNumberOfParticles(G4int(1));
	particleGun->SetParticleMomentumDirection(NWGlobal::GetInstance()->particleOriginDirection);
	particleGun->SetParticleEnergy(NWGlobal::GetInstance()->gunEnergy*MeV);

	//G4Box* fTargetBox = (G4Box*)(G4LogicalVolumeStore::GetInstance()->GetVolume("targetBoxLogical")->GetSolid());
	G4LogicalVolume* fTargetLogicalBox = G4LogicalVolumeStore::GetInstance()->GetVolume("targetBoxLogical");
	if (!fTargetLogicalBox) {
		G4ExceptionDescription msg;
		msg << "The targetBox is not defined yet!";
		G4Exception("NWPrimaryGeneratorAction::GeneratePrimaries", "Code02", FatalException, msg);

		system("pause");
		exit(1);
	}
	G4Box* fTargetVolumBox = dynamic_cast<G4Box*>(fTargetLogicalBox->GetSolid());

	G4LogicalVolume* fWorldLogicalBox = G4LogicalVolumeStore::GetInstance()->GetVolume("worldBoxLogical");
	if (!fWorldLogicalBox) {
		G4ExceptionDescription msg;
		msg << "The worldBox is not defined yet!";
		G4Exception("NWPrimaryGeneratorAction::GeneratePrimaries", "Code02", FatalException, msg);

		system("pause");
		exit(1);
	}
	G4Box* fWorldVolumBox = dynamic_cast<G4Box*>(fWorldLogicalBox->GetSolid());

	G4double X_High = fWorldVolumBox->GetXHalfLength();
	G4double X_Low = fTargetVolumBox->GetXHalfLength();
	G4double Y_High = fWorldVolumBox->GetYHalfLength();
	G4double Y_Low = fTargetVolumBox->GetYHalfLength();
	G4double Z_High = fWorldVolumBox->GetZHalfLength();
	G4double Z_Low = fTargetVolumBox->GetZHalfLength();

	//G4double X0 = 0.5*(X_High + X_Low);
	//G4double Y0 = 0.5*(Y_High + Y_Low);
	//G4double Z0 = 0.5*(Z_High + Z_Low);

	G4double X0 = 0.0;
	G4double Y0 = 0.0;
	G4double Z0 = 0.5*(Z_High + Z_Low);

	NWGlobal::GetInstance()->particleOriginPos = G4ThreeVector(X0, Y0, Z0);

	particleGun->SetParticlePosition(NWGlobal::GetInstance()->particleOriginPos);

	particleGun->GeneratePrimaryVertex(anEvent);

	NWGlobal::GetInstance()->CurrentEventID = anEvent->GetEventID();

	G4cout << "Event : " << anEvent->GetEventID() << G4endl;
}