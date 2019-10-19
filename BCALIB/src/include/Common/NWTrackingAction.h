#ifndef NWTRACKINGACTION_H
#define NWTRACKINGACTION_H

#include "G4Track.hh"
#include "G4UserTrackingAction.hh"


class NWTrackingAction : public G4UserTrackingAction {
public:
	/*Default constructor*/
	NWTrackingAction();

	/*Destructor*/
	~NWTrackingAction();

public:
	/*override*/

	virtual void PreUserTrackingAction(const G4Track*);
	virtual void PostUserTrackingAction(const G4Track*);
};


#endif