#include "NWSteppingAction_Iso.h"
#include "G4SteppingManager.hh"
#include "G4Step.hh"
#include "NWGlobal.h"
#include "NWVAuxiliaryTrackInformation.h"

using namespace std;


NWSteppingAction_Iso::NWSteppingAction_Iso() {

}

/*Copy constructor*/
NWSteppingAction_Iso::NWSteppingAction_Iso(const NWSteppingAction_Iso &) {


}


/*reload the operator = */
NWSteppingAction_Iso NWSteppingAction_Iso::operator=(const NWSteppingAction_Iso &) {

	return *this;
}


NWSteppingAction_Iso::~NWSteppingAction_Iso() {

}

void NWSteppingAction_Iso::UserSteppingAction(const G4Step* stepping) {


	int OutWidth = NWGlobal::GetInstance()->GetSimParamters().GetOutWidth();

	double PreEng;
	double PostEng;
	double DeltaEng;
	G4ThreeVector prePosition;
	G4ThreeVector postPosition;
	int totalSecondBaryonNum;
	int totalSecondAtomNum;
	//bool matrixAtomReaction;
	//int trackIndex = -1;

	const G4ParticleDefinition *particle = stepping->GetTrack()->GetParticleDefinition();

	//const G4DynamicParticle *theDynamicParticle = stepping->GetTrack()->GetDynamicParticle();

	/*
	if (NULL != stepping->GetTrack()->GetAuxiliaryTrackInformationMap()) {
		if (stepping->GetTrack()->GetAuxiliaryTrackInformationMap()->size() > 0) {
			std::cout << "----Current TrackID---- " << stepping->GetTrack()->GetTrackID() << std::endl;
			std::cout << "----Current StepID---- " << stepping->GetTrack()->GetCurrentStepNumber() << std::endl;
			dynamic_cast<NWVAuxiliaryTrackInformation*>(stepping->GetTrack()->GetAuxiliaryTrackInformation(0))->Print();
		}
	}
	*/

	totalSecondBaryonNum = 0;

	totalSecondAtomNum = 0;

	if (stepping->GetSecondaryInCurrentStep()->size() > 0) {

		NWVAuxiliaryTrackInformation* aux;

		std::vector<const G4Track*>::const_iterator it = stepping->GetSecondaryInCurrentStep()->begin();

		for (; it != stepping->GetSecondaryInCurrentStep()->end(); it++) {
			(*it)->SetAuxiliaryTrackInformation(0, aux = new NWVAuxiliaryTrackInformation());

			aux->SetCreatorStepID(stepping->GetTrack()->GetCurrentStepNumber());

			aux->SetCreatorTrackID(stepping->GetTrack()->GetTrackID());

			totalSecondBaryonNum += (*it)->GetParticleDefinition()->GetBaryonNumber();

			totalSecondAtomNum += (*it)->GetParticleDefinition()->GetAtomicNumber();

			(*it)->GetParticleDefinition()->GetPDGCharge();
		}
	}

	if (fStopAndKill == stepping->GetTrack()->GetTrackStatus()) {
		if (totalSecondBaryonNum > particle->GetBaryonNumber()) {
			//matrixAtomReaction = true;
		}
	}
	else {
		if (totalSecondBaryonNum > 0) {
			//matrixAtomReaction = true;
		}
	}


	if (particle->GetAtomicNumber() != NWGlobal::GetInstance()->GetSimParamters().GetTargetMaterial().GetAtomNumber() &&
		particle->GetAtomicNumber() > 2 &&
		0 == stepping->GetSecondaryInCurrentStep()->size()) {

		if (fStopAndKill == stepping->GetTrack()->GetTrackStatus()) {


			if (NULL != stepping->GetPreStepPoint()->GetMaterial()) {
				if (0 == stepping->GetPreStepPoint()->GetMaterial()->GetName().compare(NWGlobal::GetInstance()->GetSimParamters().GetTargetMaterial().GetMaterialName().c_str())) {

					PreEng = stepping->GetPreStepPoint()->GetKineticEnergy();
					PostEng = stepping->GetPostStepPoint()->GetKineticEnergy();
					DeltaEng = PreEng - PostEng;
					prePosition = stepping->GetPreStepPoint()->GetPosition();
					postPosition = stepping->GetPostStepPoint()->GetPosition();

					NWGlobal::GetInstance()->ofs_Iso << std::setw(OutWidth) << NWGlobal::GetInstance()->CurrentEventID
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
						<< std::setw(OutWidth) << particle->GetAtomicNumber()
						<< std::setw(OutWidth)<< particle->GetBaryonNumber()
						<< std::setw(OutWidth) << stepping->GetTrack()->GetTrackStatus()
						<< endl;

					NWGlobal::GetInstance()->flushRecord++;

					if (0 == NWGlobal::GetInstance()->flushRecord%NWGlobal::GetInstance()->GetSimParamters().GetFlushFrequence()) {
						NWGlobal::GetInstance()->ofsSimRecord.flush();
					}

				}
			}
		}
	}

}