#include "NWVAuxiliaryTrackInformation.h"


NWVAuxiliaryTrackInformation::NWVAuxiliaryTrackInformation() {
	CleanUp();
}


NWVAuxiliaryTrackInformation::~NWVAuxiliaryTrackInformation() {
	CleanUp();
}


void NWVAuxiliaryTrackInformation::Print() {
	std::cout << "Creator Track ID: " << this->CreatorTrackID << std::endl;
	std::cout << "Creator Step ID: " << this->CreatorStepID << std::endl;
}

void NWVAuxiliaryTrackInformation::CleanUp() {
	this->CreatorTrackID = -1;
	this->CreatorStepID = -1;
}