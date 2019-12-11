#ifndef NWSIMPARAMETERS_H
#define NWSIMPARAMETERS_H

#include <string>
#include <fstream>
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "NWBeam.h"
#include "NWMaterials.h"

extern std::string simMode;
extern std::string analysisMode;

enum ConcentReaction {
	InletToLastEst,				//record inlet particle till last elastic to matrix atom
	InletToFirstNonEst,				//record inlet particle till first non elastic to matrix atom
	InletEstAndInEstTillEnd,    //track inlet particle had elastice and inelastic to matrix atom till really end of this particle
								//(for QSGP_BIC_HP model neutron,inelastic, captured or nfission or decay or out boundary would 
								//end the track, but here, we still track the neturon occured inelastic reaction)
	MatrixAtom,				    //consider all reaction to matrix atom

	Iso                        // Record the isotope
};


/*Singleton class*/
class NWSimParameters {

public:
	/*Default constructor*/
	NWSimParameters();
	/*Copy Constructor*/
	NWSimParameters(const NWSimParameters &r);
	/*Reload the default operator =*/
	NWSimParameters& operator = (const NWSimParameters &r);

	/*Destructor*/
	~NWSimParameters();

	void Clean();


private:

	ConcentReaction theConcentReaction;

	std::string OutPath;

	int OutWidth = 20;

	int flushFrequence = 100;

	G4double halfWorld_x = 10000 * cm;
	G4double halfWorld_y = 10000 * cm;
	G4double halfWorld_z = 10000 * cm;

private:

	int EventLoopsNumber;

	double linkCellInterval_xy;

	int linkCellNum_z;

	double linkCellInterval_z;

private:
	NWMaterial targetMaterial;

	NWBeam beam;

public:

	void ReadParametersFromFile(const char* filePath);

	void PrintParameters();

	void SetDefulatValue();

public:

	inline void SetTheConcentReaction(ConcentReaction way) {
		this->theConcentReaction = way;
	}

	inline ConcentReaction GetTheConcentReaction() const{
		return this->theConcentReaction;
	}


	inline void SetOutPath(const std::string &theOutPath) {
		this->OutPath = theOutPath;
	}

	inline const std::string * GetOutPath() const{
		return &this->OutPath;
	}

	inline void SetOutWidth(const int & theOutWidth) {
		this->OutWidth = theOutWidth;
	}

	inline int GetOutWidth() const{
		return this->OutWidth;
	}

	inline void SetFlushFrequence(const int & theFlushFrequence) {
		this->flushFrequence = theFlushFrequence;
	}

	inline int GetFlushFrequence() const{
		return this->flushFrequence;
	}


	inline double GetHalfWorld_x() const {
		return this->halfWorld_x;
	}

	inline double GetHalfWorld_y() const {
		return this->halfWorld_y;
	}

	inline double GetHalfWorld_z() const {
		return this->halfWorld_z;
	}

	inline void SetEventLoopsNumber(const int & theEventLoopsNumber) {
		this->EventLoopsNumber = theEventLoopsNumber;
	}

	inline int GetEventLoopsNumber() const{
		return this->EventLoopsNumber;
	}

	inline void SetLinkCellInterval_xy(const double theLinkCellInterval_xy) {
		this->linkCellInterval_xy = theLinkCellInterval_xy;
	}

	inline double GetLinkCellInterval_xy() const{
		return this->linkCellInterval_xy;
	}


	inline void SetLinkCellInterval_z(const double theLinkCellInterval_z) {
		this->linkCellInterval_z = theLinkCellInterval_z;
	}

	inline double GetLinkCellInterval_z() const {
		return this->linkCellInterval_z;
	}


	inline void SetLinkCellNum_z(const int theLinkCellNum_z) {
		this->linkCellNum_z = theLinkCellNum_z;
	}

	inline int GetLinkCellNum_z() const{
		return this->linkCellNum_z;
	}


	inline void SetTargetMaterial(const NWMaterial & theMaterial) {
		this->targetMaterial = theMaterial;
	}

	inline NWMaterial const & GetTargetMaterial() const {
		return this->targetMaterial;
	}
	

	inline void SetBeam(const NWBeam & theBeam) {
		this->beam = theBeam;
	}

	inline const NWBeam & GetNWBeam() const{
		return this->beam;
	}

};



#endif