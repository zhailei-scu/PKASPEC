#include "NWActionInitialization.h"
#include "NWPrimaryGeneratorAction.h"
#include "NWSteppingAction_InletToLastEst.h"
#include "NWSteppingAction_InletToFirstNonEst.h"
#include "NWSteppingAction_InletEstAndInEstTillEnd.h"
#include "NWSteppingAction_MatrixAtoms.h"
#include "NWGlobal.h"

NWActionInitialization::NWActionInitialization() {

}


NWActionInitialization::~NWActionInitialization() {

}


void NWActionInitialization::Build() const{
	SetUserAction(new NWPrimaryGeneratorAction());

	switch (NWGlobal::GetInstance()->GetSimParamters()->GetTheConcentReaction()) {
		case ConcentReaction(InletToLastEst):
		{
			NWSteppingAction_InletToLastEst *theNWSteppingAction = new NWSteppingAction_InletToLastEst();

			theNWSteppingAction->SetTargetTrackID(1);

			SetUserAction(theNWSteppingAction);
		}
		break;

		case ConcentReaction(InletToFirstNonEst):
		{
			NWSteppingAction_InletToFirstNonEst *theNWSteppingAction = new NWSteppingAction_InletToFirstNonEst();
			theNWSteppingAction->SetTargetAtomNumber(40);

			SetUserAction(theNWSteppingAction);
		}
		break;
		

		case ConcentReaction(InletEstAndInEstTillEnd):
		{
			NWSteppingAction_InletEstAndInEstTillEnd *theNWSteppingAction = new NWSteppingAction_InletEstAndInEstTillEnd();
			theNWSteppingAction->SetTargetAtomNumber(40);

			SetUserAction(theNWSteppingAction);
		}
		break;

		case ConcentReaction(MatrixAtom):
		{
			NWSteppingAction_MatrixAtoms *theNWSteppingAction = new NWSteppingAction_MatrixAtoms();
			theNWSteppingAction->SetTargetAtomNumber(40);

			SetUserAction(theNWSteppingAction);
		}
		break;

		default:
		{
			std::cout << "Unknown record way: " << NWGlobal::GetInstance()->GetSimParamters()->GetTheConcentReaction() << std::endl;
			system("pause");
			exit(1);
		}
		break;
	}
}

