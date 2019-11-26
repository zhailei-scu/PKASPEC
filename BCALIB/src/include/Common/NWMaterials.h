#ifndef NWMATERIALS_H
#define NWMATERIALS_H

#include <string>

enum MaterialModel
{
	G4Default,
	UserDef
};

enum MaterialType
{
	G4_W,
	G4_Zr,
	User_W,
	User_Zr
};


class NWMaterial {
public:
	NWMaterial();
	~NWMaterial();
	NWMaterial(const NWMaterial & r);
	NWMaterial operator = (const NWMaterial & r);

private:
	MaterialModel theModel;

	std::string name;

	int atomNumber;
	int baryonNumber;

	double moleMass;
	double density;  

public:
	void ConstructMaterial(MaterialType type);

	void Clean();
public:

	inline MaterialModel GetTheModel() const{
		return this->theModel;
	}

	inline void SetTheModel(MaterialModel model) {
		this->theModel = model;
	}

	inline std::string GetMaterialName() const{
		return this->name;
	}

	inline void SetMaterialName(std::string theName) {
		this->name = theName;
	}

	inline int GetAtomNumber() const {
		return this->atomNumber;
	}

	inline void SetAtomNumber(int theNumber) {
		this->atomNumber = theNumber;
	}

	inline int GetBaryonNumber() const {
		return this->baryonNumber;
	}


	inline void SetBaryonNumber(int theBaryonNumber) {
		this->baryonNumber = theBaryonNumber;
	}

	inline double GetMoleMass() const {
		return this->moleMass;
	}

	inline void SetMoleMass(double theMoleMass) {
		this->moleMass = theMoleMass;
	}


	inline double GetDensity() const {
		return this->density;
	}

	inline void SetDensity(double theDensity) {
		this->density = theDensity;
	}
};






#endif