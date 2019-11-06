#include "NWSteppingAction_InletEstAndInEst.h"
#include "G4SteppingManager.hh"
#include "G4Step.hh"
#include "NWGlobal.h"

using namespace std;


NWSteppingAction_InletEstAndInEst::NWSteppingAction_InletEstAndInEst() {

}

/*Copy constructor*/
NWSteppingAction_InletEstAndInEst::NWSteppingAction_InletEstAndInEst(const NWSteppingAction_InletEstAndInEst &r) {
	if (NULL != &r) {
		this->targetAtomNumber = r.targetAtomNumber;
	}

}


/*reload the operator = */
NWSteppingAction_InletEstAndInEst NWSteppingAction_InletEstAndInEst::operator=(const NWSteppingAction_InletEstAndInEst &r) {
	if (NULL != &r) {
		this->targetAtomNumber = r.targetAtomNumber;
	}


	return *this;
}


NWSteppingAction_InletEstAndInEst::~NWSteppingAction_InletEstAndInEst() {

}

void NWSteppingAction_InletEstAndInEst::UserSteppingAction(const G4Step* stepping) {


	int OutWidth = NWGlobal::GetInstance()->GetSimParamters()->GetOutWidth();

	double PreEng;
	double PostEng;
	double DeltaEng;
	G4ThreeVector prePosition;
	G4ThreeVector postPosition;
	bool DoIt;
	int dumpCountTrackInfo = 0;
	int dumpCountStepInfo = 0;
	int ParentID;
	int FirstAncestor;
	std::string InletParticleName;

	int trackIndex = -1;

	const G4ParticleDefinition *particle = stepping->GetTrack()->GetParticleDefinition();

	const G4DynamicParticle *theDynamicParticle = stepping->GetTrack()->GetDynamicParticle();


	DoIt = false;

	if (particle->GetAtomicNumber() == this->targetAtomNumber) {

		if (1 == stepping->GetTrack()->GetCurrentStepNumber()) {

			ParentID = stepping->GetTrack()->GetParentID();

			if (1 == ParentID) { // which means the this secondary particle are caused by the non-disappered inlet particle
				DoIt = true;     //for QSGP_BIC_HP model of neutron, which means this secondary particle are caused by neutron hadElastic 
				//stepping->GetTrack()->GetCreatorProcess();

			}else{
				/*
				InletParticleName = GetParticleName(1);

				FirstAncestor = ParentID;

				while(ParentID > 0){

					DoIt = true;

					if (0 != InletParticleName.compare(GetParticleName(ParentID))) {
						DoIt = false;
						break;
					}

					if (0 != std::string("neutronInElastic").compare£¨GetEndProcessName(ParentID)) {
						DoIt = false;
						break;
					}

					FirstAncestor = ParentID;

					ParentID = GetParentID(ParentID);
				}

				if (1 != FirstAncestor) {
					std::cout << "It is impossible that on track's first ancestor is not track 1" << std::endl;
					system("pause");
					exit(1);
				}

				*/
			}


			if (true == DoIt) {

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

}