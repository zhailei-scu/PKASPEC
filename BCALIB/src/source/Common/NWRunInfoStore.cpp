#include "NWRunInfoStore.h"

OneTrack::OneTrack() {
	clean();
}

OneTrack::~OneTrack() {
	clean();
}


void OneTrack::clean() {
	this->CurrentTrackID = -1;
	this->ParentTrackID = -1;

	this->EndProcessName.clear();
	std::string().swap(this->EndProcessName);

	this->ParticleName.clear();
	std::string().swap(this->ParticleName);
}


OneTrack::OneTrack(const OneTrack & r) {
	this->CurrentTrackID = r.CurrentTrackID;
	this->ParentTrackID = r.ParentTrackID;

	this->EndProcessName = r.EndProcessName;
	this->ParticleName = r.ParticleName;

}

/*reload operation =*/
OneTrack& OneTrack::operator = (const OneTrack &r) {
	this->CurrentTrackID = r.CurrentTrackID;
	this->ParentTrackID = r.ParentTrackID;

	this->EndProcessName = r.EndProcessName;
	this->ParticleName = r.ParticleName;

	return *this;
}


OneEvent::OneEvent() {
	clean();
}


OneEvent::~OneEvent() {
	clean();
}


OneEvent::OneEvent(const OneEvent & r) {
	this->EventID = r.EventID;

	this->Tracks.swap(std::map<int, OneTrack>());

	for (std::map<int, OneTrack>::iterator it = this->Tracks.begin(); it != this->Tracks.end(); it++) {

		this->Tracks.insert(std::map<int, OneTrack>::value_type(it->first, it->second));
	}
}


OneEvent& OneEvent::operator = (const OneEvent &r) {
	this->EventID = r.EventID;

	this->Tracks.swap(std::map<int, OneTrack>());

	for (std::map<int, OneTrack>::iterator it = this->Tracks.begin(); it != this->Tracks.end(); it++) {

		this->Tracks.insert(std::map<int, OneTrack>::value_type(it->first, it->second));
	}


	return *this;
}

void OneEvent::clean() {
	this->EventID = -1;

	this->Tracks.swap(std::map<int, OneTrack>());
}