#ifndef NWACTIONINITIALIZATION_H
#define NWACTIONINITIALIZATION_H


#include "G4VUserActionInitialization.hh"

class NWActionInitialization : public G4VUserActionInitialization{
public:
	/*Default constructor*/
	NWActionInitialization();

	/*Deconstructor*/
	~NWActionInitialization();


public:
	/*override*/
	virtual void Build() const;
};



#endif
