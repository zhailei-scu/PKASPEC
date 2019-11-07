#ifndef NWRUNINFOSTORE_H
#define NWRUNINFOSTORE_H

#include <string>
#include <map>

class OneTrack {
public:
	/*Default constructor*/
	OneTrack();

	/*Destructor*/
	~OneTrack();

	/*Copy Constructor*/
	OneTrack(const OneTrack & r);

	/*reload operation =*/

	OneTrack& operator = (const OneTrack &r);

	void clean();

private:
	int ParentTrackID = -1;
	int CurrentTrackID = -1;

	std::string EndProcessName;
	std::string ParticleName;

public:
	inline void SetParentTrackID(int ParentTrackID) {
		this->ParentTrackID = ParentTrackID;
	}

	inline int GetParentTrackID() {
		return	this->ParentTrackID;
	}


	inline void SetCurrentTrackID(int CurrentTrackID) {
		this->CurrentTrackID = CurrentTrackID;
	}

	inline int GetCurrentTrackID() {
		return	this->CurrentTrackID;
	}


	inline void SetEndProcessName(std::string EndProcessName) {
		this->EndProcessName = EndProcessName;
	}

	inline std::string GetEndProcessName() {
		return	this->EndProcessName;
	}

	inline void SetParticleName(std::string ParticleName) {
		this->ParticleName = ParticleName;
	}

	inline std::string GetParticleName() {
		return	this->ParticleName;
	}

};


class OneEvent {
public:
	/*Default constructor*/
	OneEvent();

	/*Destructor*/
	~OneEvent();

	/*Copy Constructor*/
	OneEvent(const OneEvent & r);

	/*reload operation =*/

	OneEvent& operator = (const OneEvent &r);


	void clean();

public:

	int EventID;

	std::map<int, OneTrack> Tracks;
};

#endif