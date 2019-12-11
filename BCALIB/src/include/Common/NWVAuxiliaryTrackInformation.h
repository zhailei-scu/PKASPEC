#ifndef NWVAUXILIARYTRACKINFORMATION_H
#define NWVAUXILIARYTRACKINFORMATION_H

#include "G4VAuxiliaryTrackInformation.hh"


class NWVAuxiliaryTrackInformation :public G4VAuxiliaryTrackInformation {
public:
	NWVAuxiliaryTrackInformation();

	~NWVAuxiliaryTrackInformation();

	void CleanUp();

	void Print();

private:
	int CreatorTrackID;
	int CreatorStepID;

public:
	inline void SetCreatorTrackID(int theCreatorTrackID) {
		this->CreatorTrackID = theCreatorTrackID;
	}

	inline int GetCreatorTrackID() const{
		return this->CreatorTrackID;
	}


	inline void SetCreatorStepID(int theCreatorStepID) {
		this->CreatorStepID = theCreatorStepID;
	}

	inline int GetCreatorStepID() const {
		return this->CreatorStepID;
	}
};


#endif
