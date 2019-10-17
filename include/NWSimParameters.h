#ifndef NWSIMPARAMETERS_H
#define NWSIMPARAMETERS_H



/*Singleton class*/
class NWSimParameters {

private:
	/*Default constructor*/
	NWSimParameters();
	NWSimParameters(const NWSimParameters &r) = delete;
	NWSimParameters operator = (const NWSimParameters &r) = delete;

	static NWSimParameters* nWSimParametersInstance;

public:
	/*Destructor*/
	~NWSimParameters();

	static NWSimParameters* GetInstance();

	static void ReleaseInstance();

public:

	class NWSimParametersGarbo{
	public:
		NWSimParametersGarbo();
		~NWSimParametersGarbo();
	};

private:
	static NWSimParametersGarbo nWSimParametersGarboInstance;

};








#endif