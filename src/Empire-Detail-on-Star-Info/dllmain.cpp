// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "Spore-Mod-Utils/Include/SporeModUtils.h"
#include "ArchetypeWinProcHelper.h"
#include "EmpireInfoProc.h"
#include "cEmpireInfoStrategy.h"

using namespace Simulator;
using namespace SporeModUtils;

void Initialize()
{
	cSimulatorSystem::Get()->AddStrategy(new cEmpireInfoStrategy(), cEmpireInfoStrategy::NOUN_ID);
}

void Dispose()
{
	// This method is called when the game is closing
}

member_detour(SpaceGameUi__FillStarTooltipStarInfo__detour, UI::SpaceGameUI, int(ISimulatorSerializable*)){
	int detoured(ISimulatorSerializable * serializable) {
		int toReturn = original_function(this, serializable);
		// If the mouse is over a star.
		if (serializable != nullptr && serializable->GetNounID() == cStarRecord::CLASS_ID) {
			cStarRecord* star = object_cast<cStarRecord>(serializable);
			if (star != nullptr) {

				UTFWin::UILayout* starTooltip = this->mpSpaceStarTooltipLayout.get();
				// If the required windows are not in the UI, we create them and add the procedure to empireNameWindow.
				if (!starTooltip->FindWindowByID(0x7CFBCBE1)) {
					// Add the window to the starTooltipWindow.
					UTFWin::UILayout* layout = new UTFWin::UILayout();
					layout->LoadByID(0xd27d91cf);
					cEmpireInfoStrategy::Get()->layout = layout;

					UTFWin::IWindow* starTooltipWindow = starTooltip->FindWindowByID(0x01C22500);
					layout->SetParentWindow(starTooltipWindow);

					// Create and add the winProc.
					ResourceKey configKey = ResourceKey(id("MainConfig"), 0, id("EdsiConfig"));
					ArchetypeWinProcHelperPtr archetypeHelper = new ArchetypeWinProcHelper();
					UTFWin::IWindow* empireNameWindow = starTooltip->FindWindowByID(0x7CFBCBE1);
					UTFWin::IWindow* archetypeImageWindow = starTooltip->FindWindowByID(0x6BE3873C);
					UTFWin::IWindow* starNameWindow = starTooltip->FindWindowByID(0x01c22501);
					UTFWin::IWindow* planetInfoWindow = starTooltip->FindWindowByID(0x0684BC00);

					EmpireInfoProc* empireInfoProc = new EmpireInfoProc(configKey, 
						archetypeHelper.get(), 
						empireNameWindow, 
						archetypeImageWindow, 
						starNameWindow, 
						planetInfoWindow);

					empireNameWindow->AddWinProc(empireInfoProc);

				}
				UTFWin::IWindow* empireNameWindow = starTooltip->FindWindowByID(0x7CFBCBE1);
				EmpireInfoProc* empireInfoProcFunction = nullptr;
				IWinProc* proc = empireNameWindow->GetNextWinProc(empireInfoProcFunction);
				EmpireInfoProc* empireInfoProc = object_cast<EmpireInfoProc>(proc);
				empireInfoProc->HandleStarHover(star);


			}
		}
		return toReturn;
	}
};

void AttachDetours()
{
	SpaceGameUi__FillStarTooltipStarInfo__detour::attach(Address(ModAPI::ChooseAddress(0x0106d8f0, 0x0106cca0)));
}


// Generally, you don't need to touch any code here
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		ModAPI::AddPostInitFunction(Initialize);
		ModAPI::AddDisposeFunction(Dispose);

		PrepareDetours(hModule);
		AttachDetours();
		CommitDetours();
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

