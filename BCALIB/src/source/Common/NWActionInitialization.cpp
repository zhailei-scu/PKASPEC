#include "NWActionInitialization.h"
#include "NWTrackingAction.h"
#include "NWPrimaryGeneratorAction.h"
#include "NWSteppingAction_InletToLastEst.h"
#include "NWSteppingAction_InletToFirstNonEst.h"
#include "NWSteppingAction_InletEstAndInEstTillEnd.h"
#include "NWSteppingAction_MatrixAtoms.h"
#include "NWSteppingAction_Iso.h"
#include "NWGlobal.h"

NWActionInitialization::NWActionInitialization() {

}


NWActionInitialization::~NWActionInitialization() {

}


void NWActionInitialization::Build() const{
	SetUserAction(new NWPrimaryGeneratorAction());

	//SetUserAction(new NWTrackingAction());

	switch (NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {
		case ConcentReaction(InletToLastEst):
		{
			NWSteppingAction_InletToLastEst *theNWSteppingAction = new NWSteppingAction_InletToLastEst();

			SetUserAction(theNWSteppingAction);
		}
		break;

		case ConcentReaction(InletToFirstNonEst):
		{
			NWSteppingAction_InletToFirstNonEst *theNWSteppingAction = new NWSteppingAction_InletToFirstNonEst();

			SetUserAction(theNWSteppingAction);
		}
		break;
		

		case ConcentReaction(InletEstAndInEstTillEnd):
		{
			NWSteppingAction_InletEstAndInEstTillEnd *theNWSteppingAction = new NWSteppingAction_InletEstAndInEstTillEnd();

			SetUserAction(theNWSteppingAction);
		}
		break;

		case ConcentReaction(MatrixAtom):
		{
			NWSteppingAction_MatrixAtoms *theNWSteppingAction = new NWSteppingAction_MatrixAtoms();

			SetUserAction(theNWSteppingAction);
		}
		break;

		case ConcentReaction(Iso):
		{
			NWSteppingAction_Iso *theNWSteppingAction = new NWSteppingAction_Iso();

			SetUserAction(theNWSteppingAction);
		}
		break;

		default:
		{
			std::cout << "Unknown record way: " << NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() << std::endl;
			system("pause");
			exit(1);
		}
		break;
	}
}

