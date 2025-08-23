// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
using namespace Simulator;

void Initialize()
{
	// This method is executed when the game starts, before the user interface is shown
	// Here you can do things such as:
	//  - Add new cheats
	//  - Add new simulator classes
	//  - Add new game modes
	//  - Add new space tools
	//  - Change materials
}

void Dispose()
{
	// This method is called when the game is closing
}

member_detour(SpaceGameUI_FillStarTooltipPlanetInfo__detour, UI::SpaceGameUI,
	bool(UTFWin::UILayout*, int, cPlanetRecord*)) {
	bool detoured(UTFWin::UILayout * layout, int slotIndex, cPlanetRecord * planetRecord) {
		int bAddress = baseAddress;
		cStarRecord* star = planetRecord->GetStarRecord();
		if (star->GetTechLevel() == TechLevel::Empire) {
			cEmpire* empire = StarManager.GetEmpire(star->mEmpireID);
			if (empire != nullptr) {
				UTFWin::IWindow* window = layout->FindWindowByID(0x7CFBCBE1);
				window->SetCaption(empire->mEmpireName.c_str());
			}
		}
		return original_function(this, layout, slotIndex, planetRecord);
	}
};

member_detour(SpaceGameUi__FillStarTooltipStarInfo__detour, UI::SpaceGameUI, int(ISimulatorSerializable*)){
	int detoured(ISimulatorSerializable * serializable) {
		int toReturn = original_function(this, serializable);
		if (serializable != nullptr && serializable->GetNounID() == 65287484) {
			/*
			cStarRecord* star = static_cast<cStarRecord*>(serializable);
			UTFWin::UILayout* layout = this->mpSpaceStarTooltipLayout.get();
			UTFWin::IWindow* window = layout->FindWindowByID(0x7CFBCBE1);
			cEmpire* empire = StarManager.GetEmpire(star->mEmpireID);
			eastl::vector<StarID> starsKnown = GetPlayer()->mStarsKnown;
			eastl::vector<StarID> starsVisited = GetPlayer()->mStarsVisited;
			auto it1 = eastl::find(starsKnown.begin(), starsKnown.end(), star->GetID());
			auto it2 = eastl::find(starsVisited.begin(), starsVisited.end(), star->GetID());
			if ((it1 != starsKnown.end() || it2 != starsVisited.end()) && star->GetTechLevel() == TechLevel::Empire && empire !=  nullptr) {
				window->SetCaption(empire->mEmpireName.c_str());
				window->SetVisible(true);
			}
			else {
				window->SetVisible(false);
			}
			*/
			cStarRecord* star = static_cast<cStarRecord*>(serializable);
			cEmpire* empire = StarManager.GetEmpire(star->mEmpireID);
			UTFWin::UILayout* starTooltip = this->mpSpaceStarTooltipLayout.get();
			UTFWin::IWindow* planetInfoLayout = starTooltip->FindWindowByID(0x0684BC00);
			UTFWin::IWindow* textLayout = starTooltip->FindWindowByID(0x7CFBCBE1);

			if (planetInfoLayout->IsVisible() && star->GetTechLevel() == TechLevel::Empire && empire != nullptr) {
				textLayout->SetCaption(empire->mEmpireName.c_str());
				textLayout->SetVisible(true);
			}
			else {
				textLayout->SetVisible(false);
			}
		}
		return toReturn;
	}
};

member_detour(SpaceGameUi__capaz__detour, UI::SpaceGameUI, void(UTFWin::UILayout*, int)) {
	void detoured(UTFWin::UILayout * layout, int a) {
		original_function(this, layout, a);
	}
};

void AttachDetours()
{
	//SpaceGameUI_FillStarTooltipPlanetInfo__detour::attach(GetAddress(UI::SpaceGameUI, FillStarTooltipPlanetInfo));
	SpaceGameUi__FillStarTooltipStarInfo__detour::attach(Address(0x0106cca0));
	//SpaceGameUi__capaz__detour::attach(Address(0x01065b70));
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

