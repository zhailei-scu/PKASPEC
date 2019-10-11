#include "NWSteppingAction.h"
#include "G4SteppingManager.hh"
#include "G4Step.hh"
#include "NWGlobal.h"
#include "NWInfoStore.h"

using namespace std;

NWSteppingAction::NWSteppingAction() {

}

/*Copy constructor*/
NWSteppingAction::NWSteppingAction(const NWSteppingAction &r) {
	if (NULL != &r) {

		this->targetTrackID = r.targetTrackID;
		this->targetProcessName = r.targetProcessName;
	}
	
}


/*reload the operator = */
NWSteppingAction NWSteppingAction::operator=(const NWSteppingAction &r) {
	if (NULL != &r) {

		this->targetTrackID = r.targetTrackID;
		this->targetProcessName = r.targetProcessName;
	}


	return *this;
}


NWSteppingAction::~NWSteppingAction() {

}

void NWSteppingAction::UserSteppingAction(const G4Step* stepping) {

	int OutWidth = NWGlobal::GetInstance()->OutWidth;

	double PreEng;
	double PostEng;
	double DeltaEng;
	G4ThreeVector prePosition;
	G4ThreeVector postPosition;

	int dumpCountTrackInfo = 0;
	int dumpCountStepInfo = 0;

	int trackIndex = -1;

	const G4ParticleDefinition *particle = stepping->GetTrack()->GetParticleDefinition();

	const G4DynamicParticle *theDynamicParticle =  stepping->GetTrack()->GetDynamicParticle();

	size_t PSDIPIDTriggered = fpSteppingManager->GetfPostStepDoItProcTriggered();

	G4String name = "Undefined Process name";

	int PostStepLoops = fpSteppingManager->GetMAXofPostStepLoops();

	for (size_t np = 0; np < PostStepLoops; np++) {
		//
		// Note: DoItVector has inverse order against GetPhysIntVector
		//       and SelectedPostStepDoItVector.
		//
		G4int Cond = (*fpSteppingManager->GetfSelectedPostStepDoItVector())[PostStepLoops - np - 1];

		if (Cond != InActivated) {
			if (((Cond == NotForced) && (fpSteppingManager->GetfStepStatus() == fPostStepDoItProc)) //||  //Here, we only care about the non-transportation process
				//((Cond == Forced) && (fpSteppingManager->GetfStepStatus() != fExclusivelyForcedProc)) ||
				//	   ((Cond == Conditionally) && (fpSteppingManager->GetfStepStatus() == fAlongStepDoItProc)) ||
				//((Cond == ExclusivelyForced) && (fpSteppingManager->GetfStepStatus() == fExclusivelyForcedProc)) ||
				//((Cond == StronglyForced))
				) {

				if (this->targetTrackID == stepping->GetTrack()->GetTrackID()) {

					if ((PostStepLoops - np - 1) != PSDIPIDTriggered) {
						std::cout << "The are wrong between : " << PostStepLoops - np - 1 << " and " << PSDIPIDTriggered << std::endl;
						//system("pause");
					}

					name = ((*fpSteppingManager->GetfPostStepDoItVector())[np])->GetProcessName();

					if (0 == name.compare(this->targetProcessName)) {
						if (NULL != stepping->GetPostStepPoint()->GetMaterial()) {
							if (0 == stepping->GetPostStepPoint()->GetMaterial()->GetName().compare(NWGlobal::GetInstance()->targetMaterial)) {
								
								PreEng = stepping->GetPreStepPoint()->GetKineticEnergy();
								PostEng = stepping->GetPostStepPoint()->GetKineticEnergy();
								DeltaEng = PreEng - PostEng;
								prePosition = stepping->GetPreStepPoint()->GetPosition();
								postPosition = stepping->GetPostStepPoint()->GetPosition();

								NWGlobal::GetInstance()->ofsSimRecord << std::setw(OutWidth) << NWGlobal::GetInstance()->CurrentEventID
									<< std::setw(OutWidth) << stepping->GetTrack()->GetTrackID()
									<< std::setw(OutWidth) <<std::setiosflags(std::ios::scientific)<<std::setprecision(7)<< stepping->GetTrack()->GetCurrentStepNumber()
									<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << PreEng 
									<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << PostEng 
									<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << DeltaEng
									<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << prePosition.getX()
									<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << prePosition.getY()
									<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << prePosition.getZ()
									<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << postPosition.getX()
									<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << postPosition.getY()
									<< std::setw(OutWidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << postPosition.getZ()
									<< endl;

								NWGlobal::GetInstance()->flushRecord++;

								if (0 == NWGlobal::GetInstance()->flushRecord%NWGlobal::GetInstance()->flushFrequence) {
									NWGlobal::GetInstance()->ofsSimRecord.flush();
								}

								//store info
								
								/*

								if (0 == NWInfoStore::GetInstance()->GetEventsInfo()->count(NWGlobal::GetInstance()->CurrentEventID)) {

									NWInfoStore::GetInstance()->GetEventsInfo()->insert(map<int, vector<TrackInfo>>::value_type(NWGlobal::GetInstance()->CurrentEventID, vector<TrackInfo>()));
								}
								
								map<int, vector<TrackInfo>>::iterator it = NWInfoStore::GetInstance()->GetEventsInfo()->find(NWGlobal::GetInstance()->CurrentEventID);

								vector<TrackInfo>* trackVector = &(it->second);

								trackIndex = -1;

								for (vector<TrackInfo>::iterator itera = trackVector->begin(); itera != trackVector->end(); itera++) {
									if (itera->GetTrackID() == stepping->GetTrack()->GetTrackID()) {
										dumpCountTrackInfo++;

										trackIndex++;

										if (dumpCountTrackInfo > 1) {
											cout << "There are dumplicate case for event: " << NWGlobal::GetInstance()->CurrentEventID
												<< "track : " << stepping->GetTrack()->GetTrackID() << endl;
											system("pause");
										}
									}
								}

								TrackInfo *tempTrackInfo = NULL;

								if (trackIndex < 0) {
									trackVector->push_back(TrackInfo(stepping->GetTrack()->GetTrackID()));

									trackIndex = trackVector->size() - 1;
								}
								tempTrackInfo = &trackVector->at(trackIndex);
								

								if (tempTrackInfo->GetStepsInfo()->size()) {
									for (vector<StepInfo>::iterator itStep = tempTrackInfo->GetStepsInfo()->begin(); itStep != tempTrackInfo->GetStepsInfo()->end(); itStep++) {
										if (itStep->GetStepID() == stepping->GetTrack()->GetCurrentStepNumber()) {
											cout << "There are dumplicate case for event: " << NWGlobal::GetInstance()->CurrentEventID
												<< "track : " << stepping->GetTrack()->GetTrackID()
												<< "step : " << stepping->GetTrack()->GetCurrentStepNumber() << endl;
											system("pause");
										}
									}
								}

								StepInfo tempStepInfo(stepping->GetTrack()->GetCurrentStepNumber());

								tempStepInfo.SetpreEng(PreEng);
								tempStepInfo.SetpostEng(PostEng);
								tempStepInfo.SetprePosition(prePosition);
								tempStepInfo.SetpostPosition(postPosition);

								tempTrackInfo->GetStepsInfo()->push_back(tempStepInfo);
								*/
							}
						}
					}
					

					//std::cout << name << std::endl;

					/*
					
					G4cout << "#####################################################################" << G4endl;

					G4cout << "Tracking ID: " << stepping->GetTrack()->GetTrackID()
						<< " Parent ID: " << stepping->GetTrack()->GetParentID()
						<< " process name: " << name << G4endl;

					G4cout << "Particle name: " << particle->GetParticleName()
						<< " Particle energy " << theDynamicParticle->GetKineticEnergy() << G4endl;


					G4cout << "The particle energy in pre step point: " << stepping->GetPreStepPoint()->GetKineticEnergy() << G4endl;
					G4cout << "The particle energy in post step point: " << stepping->GetPostStepPoint()->GetKineticEnergy() << G4endl;

					G4cout << "Pre Step point" << stepping->GetPreStepPoint()->GetPosition() << G4endl;

					G4cout << "Post Step point" << stepping->GetPostStepPoint()->GetPosition() << G4endl;

					if (NULL != stepping->GetPostStepPoint()->GetMaterial()) {
						G4cout << "The material is : " << stepping->GetPostStepPoint()->GetMaterial()->GetName() << G4endl;
					}
					else {
						G4cout << "The material is : " << "Null or undefine" << G4endl;
					}

				
					

					G4cout << "-------------This process is executed----------------" << G4endl;

					G4cout << "-------------This primary particle process is executed----------------" << G4endl;

					G4cout << "-------------Follow is the secondaries ----------------" << G4endl;

					vector<G4Track*>::const_iterator it = stepping->GetSecondary()->begin();

					for (; it != stepping->GetSecondary()->end(); it++) {
						G4cout << "Secondaries: " << G4endl;

						const G4ParticleDefinition *secondParticle = (*it)->GetParticleDefinition();
						const G4DynamicParticle *secondDynamicParticle = (*it)->GetDynamicParticle();

						G4cout << "Tracking ID: " << (*it)->GetTrackID()
							<< " Parent ID: " << (*it)->GetParentID() << G4endl;

						G4cout << "Particle name: " << secondParticle->GetParticleName()
							<< " Particle energy " << secondDynamicParticle->GetKineticEnergy() << G4endl;
					}

					
					*/

				}

			}
		}
	}
	
}


void NWSteppingAction::SetTargetTrackID(int targetTrackID) {
	this->targetTrackID = targetTrackID;
}

void NWSteppingAction::SetTargetProcessName(string targetProcessName) {
	this->targetProcessName = targetProcessName;
}