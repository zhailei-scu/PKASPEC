#include "NWSteppingAction_MatrixAtoms.h"
#include "G4SteppingManager.hh"
#include "G4Step.hh"
#include "NWGlobal.h"

using namespace std;


NWSteppingAction_MatrixAtoms::NWSteppingAction_MatrixAtoms() {

}

/*Copy constructor*/
NWSteppingAction_MatrixAtoms::NWSteppingAction_MatrixAtoms(const NWSteppingAction_MatrixAtoms &) {


}


/*reload the operator = */
NWSteppingAction_MatrixAtoms NWSteppingAction_MatrixAtoms::operator=(const NWSteppingAction_MatrixAtoms &) {

	return *this;
}


NWSteppingAction_MatrixAtoms::~NWSteppingAction_MatrixAtoms() {

}

void NWSteppingAction_MatrixAtoms::UserSteppingAction(const G4Step* stepping) {


	int OutWidth = NWGlobal::GetInstance()->GetSimParamters()->GetOutWidth();

	double PreEng;
	double PostEng;
	double DeltaEng;
	G4ThreeVector prePosition;
	G4ThreeVector postPosition;

	//int trackIndex = -1;

	const G4ParticleDefinition *particle = stepping->GetTrack()->GetParticleDefinition();

	//const G4DynamicParticle *theDynamicParticle = stepping->GetTrack()->GetDynamicParticle();

	if (particle->GetAtomicNumber() ==  NWGlobal::GetInstance()->GetSimParamters()->GetTargetAtomNumber()) {

		if (1 == stepping->GetTrack()->GetCurrentStepNumber()) {

			if (NULL != stepping->GetPreStepPoint()->GetMaterial()) {
				if (0 == stepping->GetPreStepPoint()->GetMaterial()->GetName().compare(NWGlobal::GetInstance()->GetSimParamters()->GetTargetMaterial()->c_str())) {

					PreEng = stepping->GetPreStepPoint()->GetKineticEnergy();
					PostEng = stepping->GetPostStepPoint()->GetKineticEnergy();
					DeltaEng = PreEng - PostEng;
					prePosition = stepping->GetPreStepPoint()->GetPosition();
					postPosition = stepping->GetPostStepPoint()->GetPosition();

					NWGlobal::GetInstance()->ofsSimRecord << std::setw(OutWidth) << NWGlobal::GetInstance()->CurrentEventID
						<< std::setw(OutWidth) << stepping->GetTrack()->GetTrackID()
						<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << stepping->GetTrack()->GetCurrentStepNumber()
						<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << PreEng
						<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << PostEng
						<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << DeltaEng
						<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << stepping->GetDeltaTime()
						<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << NWGlobal::GetInstance()->particleOriginDirection.getX()
						<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << NWGlobal::GetInstance()->particleOriginDirection.getY()
						<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << NWGlobal::GetInstance()->particleOriginDirection.getZ()
						<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << NWGlobal::GetInstance()->particleOriginPos.getX()
						<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << NWGlobal::GetInstance()->particleOriginPos.getY()
						<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << NWGlobal::GetInstance()->particleOriginPos.getZ()
						<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << prePosition.getX()
						<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << prePosition.getY()
						<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << prePosition.getZ()
						<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << postPosition.getX()
						<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << postPosition.getY()
						<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << postPosition.getZ()
						<< std::setw(OutWidth) << fpSteppingManager->GetfCurrentProcess()->GetProcessName()
						<< std::setw(OutWidth) << particle->GetParticleName()
						<< std::setw(OutWidth) << stepping->GetTrack()->GetTrackStatus()
						<< endl;

					NWGlobal::GetInstance()->flushRecord++;

					if (0 == NWGlobal::GetInstance()->flushRecord%NWGlobal::GetInstance()->GetSimParamters()->GetFlushFrequence()) {
						NWGlobal::GetInstance()->ofsSimRecord.flush();
					}

				}
			}
		}
	}

}