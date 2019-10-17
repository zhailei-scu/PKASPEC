#include "NWSimParameters.h"
#include <stdlib.h>
/*Singleton instance*/
NWSimParameters* NWSimParameters::nWSimParametersInstance = new NWSimParameters();


NWSimParameters::NWSimParameters() {
	this->nWSimParametersInstance = NULL;
}

NWSimParameters::~NWSimParameters() {

}

void NWSimParameters::ReleaseInstance() {
	if (NULL != nWSimParametersInstance) {
		delete nWSimParametersInstance;
		nWSimParametersInstance = NULL;
	}
}

NWSimParameters* NWSimParameters::GetInstance() {
	return nWSimParametersInstance;
}


NWSimParameters::NWSimParametersGarbo::NWSimParametersGarbo() {

}

NWSimParameters::NWSimParametersGarbo::~NWSimParametersGarbo() {
	ReleaseInstance();
}