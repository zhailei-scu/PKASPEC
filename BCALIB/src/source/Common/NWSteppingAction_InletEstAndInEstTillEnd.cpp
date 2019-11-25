#include "NWSteppingAction_InletEstAndInEstTillEnd.h"
#include "G4SteppingManager.hh"
#include "G4Step.hh"
#include "NWGlobal.h"

using namespace std;


NWSteppingAction_InletEstAndInEstTillEnd::NWSteppingAction_InletEstAndInEstTillEnd() {
	this->theOneEvent.clean();
}

/*Copy constructor*/
NWSteppingAction_InletEstAndInEstTillEnd::NWSteppingAction_InletEstAndInEstTillEnd(const NWSteppingAction_InletEstAndInEstTillEnd &r) {
	if (NULL != &r) {

		/*The operation = had been overrided*/
		this->theOneEvent = r.theOneEvent;
	}

}


/*reload the operator = */
NWSteppingAction_InletEstAndInEstTillEnd NWSteppingAction_InletEstAndInEstTillEnd::operator=(const NWSteppingAction_InletEstAndInEstTillEnd &r) {
	if (NULL != &r) {

		/*The operation = had been overrided*/
		this->theOneEvent = r.theOneEvent;
	}


	return *this;
}


NWSteppingAction_InletEstAndInEstTillEnd::~NWSteppingAction_InletEstAndInEstTillEnd() {
	this->theOneEvent.clean();
}

void NWSteppingAction_InletEstAndInEstTillEnd::UserSteppingAction(const G4Step* stepping) {


	int OutWidth = NWGlobal::GetInstance()->GetSimParamters().GetOutWidth();

	double PreEng;
	double PostEng;
	double DeltaEng;
	G4ThreeVector prePosition;
	G4ThreeVector postPosition;
	bool DoIt;
	int ParentID;
	int GrandParentID;
	int FirstAncestor;
	std::string InletParticleName;
	OneTrack theTrack;
	std::map<int, OneTrack>::iterator it;

	const G4ParticleDefinition *particle = stepping->GetTrack()->GetParticleDefinition();

	//const G4DynamicParticle *theDynamicParticle = stepping->GetTrack()->GetDynamicParticle();


	DoIt = false;

	if (particle->GetAtomicNumber() == NWGlobal::GetInstance()->GetSimParamters().GetTargetMaterial().GetAtomNumber()) {

		if (1 == stepping->GetTrack()->GetCurrentStepNumber()) {

			ParentID = stepping->GetTrack()->GetParentID();

			if (ParentID > 0) {

				if (1 == ParentID) { // which means the this secondary particle are caused by the non-disappered inlet particle
					DoIt = true;     //for QSGP_BIC_HP model of neutron, which means this secondary particle are caused by neutron Elastic or non-Elastic 
					//stepping->GetTrack()->GetCreatorProcess();

				}
				else {

					InletParticleName = theOneEvent.Tracks.find(1)->second.GetParticleName();

					GrandParentID = theOneEvent.Tracks.find(ParentID)->second.GetParentTrackID();

					FirstAncestor = GrandParentID;

					while (GrandParentID > 0) {

						DoIt = true;

						if (0 != InletParticleName.compare(theOneEvent.Tracks.find(ParentID)->second.GetParticleName())) {
							DoIt = false;
							break;
						}

						if (0 != std::string("neutronInelastic").compare(theOneEvent.Tracks.find(GrandParentID)->second.GetEndProcessName())) {
							DoIt = false;
							break;
						}

						ParentID = GrandParentID;

						FirstAncestor = GrandParentID;

						GrandParentID = theOneEvent.Tracks.find(ParentID)->second.GetParentTrackID();
						
					}

					if (true == DoIt && 1 != FirstAncestor) {
						std::cout << "It is impossible that on track's first ancestor is not track 1" << std::endl;
						std::cout << FirstAncestor << std::endl;
						std::cout << NWGlobal::GetInstance()->CurrentEventID << std::endl;
						std::cout << stepping->GetTrack()->GetTrackID() << std::endl;
						std::cout << stepping->GetTrack()->GetCurrentStepNumber() << std::endl;
						std::cout << particle->GetParticleName() << std::endl;
						std::cout << stepping->GetTrack()->GetParentID() << std::endl;
						system("pause");
						exit(1);
					}

				}

			}

			if (true == DoIt) {

				if (NULL != stepping->GetPreStepPoint()->GetMaterial()) {
					if (0 == stepping->GetPreStepPoint()->GetMaterial()->GetName().compare(NWGlobal::GetInstance()->GetSimParamters().GetTargetMaterial().GetMaterialName().c_str())) {

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

						if (0 == NWGlobal::GetInstance()->flushRecord%NWGlobal::GetInstance()->GetSimParamters().GetFlushFrequence()) {
							NWGlobal::GetInstance()->ofsSimRecord.flush();
						}

					}
				}

			}


			
		}
	}


	/*Store Data*/
	if (NWGlobal::GetInstance()->CurrentEventID != theOneEvent.EventID) {
		theOneEvent.clean();
		theOneEvent.EventID = NWGlobal::GetInstance()->CurrentEventID;
	}

	if (theOneEvent.Tracks.count(stepping->GetTrack()->GetTrackID()) <= 0) {
		theTrack.SetCurrentTrackID(stepping->GetTrack()->GetTrackID());

		theTrack.SetParentTrackID(stepping->GetTrack()->GetParentID());

		theTrack.SetParticleName(particle->GetParticleName());

		theTrack.SetEndProcessName(fpSteppingManager->GetfCurrentProcess()->GetProcessName());

		theOneEvent.Tracks.insert(std::map<int,OneTrack>::value_type(stepping->GetTrack()->GetTrackID(), theTrack));
	}
	else {
		it = theOneEvent.Tracks.find(stepping->GetTrack()->GetTrackID());

		if (it->second.GetCurrentTrackID() != stepping->GetTrack()->GetTrackID()) {
			std::cout << "The recorded track id is not match." << std::endl;
			std::cout << it->second.GetCurrentTrackID() << std::endl;
			std::cout << stepping->GetTrack()->GetTrackID() << std::endl;
			system("pause");
			exit(1);
		}

		if (it->second.GetParentTrackID() != stepping->GetTrack()->GetParentID()) {
			std::cout << "The recorded track id is not match." << std::endl;
			std::cout << it->second.GetParentTrackID() << std::endl;
			std::cout << stepping->GetTrack()->GetParentID() << std::endl;
			system("pause");
			exit(1);
		}


		if (0 != particle->GetParticleName().compare(it->second.GetParticleName())) {
			std::cout << "The recorded particle id is not match." << std::endl;
			std::cout << particle->GetParticleName() << std::endl;
			std::cout << it->second.GetParticleName() << std::endl;
			system("pause");
			exit(1);
		}

		it->second.SetEndProcessName(fpSteppingManager->GetfCurrentProcess()->GetProcessName());

	}


}