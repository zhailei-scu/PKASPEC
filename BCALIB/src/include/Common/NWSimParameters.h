#ifndef NWSIMPARAMETERS_H
#define NWSIMPARAMETERS_H

#include <string>
#include <fstream>
#include "G4ThreeVector.hh"
#include "NWBeam.h"

extern std::string simMode;
extern std::string analysisMode;

enum ConcentReaction {
	InletToLastEst,				//record inlet particle till last elastic to matrix atom
	InletToFirstInEst,				//record inlet particle till first inelastic to matrix atom
	InletEstAndInEstTillEnd,    //track inlet particle had elastice and inelastic to matrix atom till really end of this particle
								//(for QSGP_BIC_HP model neutron,inelastic, captured or nfission or decay or out boundary would 
								//end the track, but here, we still track the neturon occured inelastic reaction)
	MatrixAtom				    //consider all reaction to matrix atom
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

private:
	int flushFrequence = 100;

	int EventLoopsNumber;

	double linkCellInterval_xy;

	int linkCellNum_z;

	std::string targetMaterial;

private:
	NWBeam beam;

public:

	void ReadParametersFromFile(const char* filePath);

	void PrintParameters();

	void SetDefulatValue();

public:

	inline void SetTheConcentReaction(ConcentReaction way) {
		this->theConcentReaction = way;
	}

	inline ConcentReaction GetTheConcentReaction(){
		return this->theConcentReaction;
	}


	inline void SetOutPath(const std::string &OutPath) {
		this->OutPath = OutPath;
	}

	inline const std::string * GetOutPath() const{
		return &this->OutPath;
	}

	inline void SetOutWidth(const int & OutWidth) {
		this->OutWidth = OutWidth;
	}

	inline const int GetOutWidth() const{
		return this->OutWidth;
	}

	inline void SetFlushFrequence(const int & flushFrequence) {
		this->flushFrequence = flushFrequence;
	}

	inline const int GetFlushFrequence() const{
		return this->flushFrequence;
	}

	inline void SetEventLoopsNumber(const int & EventLoopsNumber) {
		this->EventLoopsNumber = EventLoopsNumber;
	}

	inline const int GetEventLoopsNumber() const{
		return this->EventLoopsNumber;
	}

	inline void SetLinkCellInterval_xy(const double linkCellInterval_xy) {
		this->linkCellInterval_xy = linkCellInterval_xy;
	}

	inline double GetLinkCellInterval_xy() {
		return this->linkCellInterval_xy;
	}


	inline void SetLinkCellNum_z(const int linkCellNum_z) {
		this->linkCellNum_z = linkCellNum_z;
	}

	inline int GetLinkCellNum_z() {
		return this->linkCellNum_z;
	}

	inline void SetTargetMaterial(const std::string &targetMaterial) {
		this->targetMaterial = targetMaterial;
	}

	inline const std::string * GetTargetMaterial() const{
		return &this->targetMaterial;
	}

	inline void SetBeam(const NWBeam & beam) {
		this->beam = beam;
	}

	inline NWBeam * GetNWBeam(){
		return &this->beam;
	}

};



#endif