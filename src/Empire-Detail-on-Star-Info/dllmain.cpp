// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "Spore-Mod-Utils/Include/SporeModUtils.h"
using namespace Simulator;
using namespace SporeModUtils;

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

char16_t* ArchetypeToString(Archetypes archetype) {
	switch (archetype) {
	case Archetypes::kArchetypeWarrior:         return u"Warrior";
	case Archetypes::kArchetypePlayerWarrior:   return u"Knight";
	case Archetypes::kArchetypeTrader:          return u"Trader";
	case Archetypes::kArchetypeScientist:       return u"Scientist";
	case Archetypes::kArchetypeShaman:          return u"Shaman";
	case Archetypes::kArchetypeBard:            return u"Bard";
	case Archetypes::kArchetypeZealot:          return u"Zealot";
	case Archetypes::kArchetypeDiplomat:        return u"Diplomat";
	case Archetypes::kArchetypeGrob:            return u"Wanderer";  // Returned when playerWanderer is mapped with preservePlayerSubtypes = true
	case Archetypes::kArchetypeEcologist:       return u"Ecologist";
	}
}

bool GetArchetypeImagen(ImagePtr& image, Archetypes archetype) {
	const char16_t* imageName = nullptr;

	switch (archetype) {
	case Simulator::Archetypes::kArchetypeBard:
		imageName = u"ConsequenceGraphics!trait_spg_bard.png";
		break;
	case Simulator::Archetypes::kArchetypeDiplomat:
		imageName = u"ConsequenceGraphics!trait_spg_diplomat.png";
		break;
	case Simulator::Archetypes::kArchetypeEcologist:
		imageName = u"ConsequenceGraphics!trait_spg_ecologist.png";
		break;
	case Simulator::Archetypes::kArchetypePlayerKnight:
	case Simulator::Archetypes::kArchetypePlayerWarrior:
		imageName = u"ConsequenceGraphics!trait_spg_knight.png";
		break;
	case Simulator::Archetypes::kArchetypeWarrior:
		imageName = u"ConsequenceGraphics!trait_spg_warrior.png";
		break;
	case Simulator::Archetypes::kArchetypeScientist:
		imageName = u"ConsequenceGraphics!trait_spg_scientist.png";
		break;
	case Simulator::Archetypes::kArchetypeShaman:
		imageName = u"ConsequenceGraphics!trait_spg_shaman.png";
		break;
	case Simulator::Archetypes::kArchetypeTrader:
		imageName = u"ConsequenceGraphics!trait_spg_trader.png";
		break;
	case Simulator::Archetypes::kArchetypePlayerWanderer:
	case Simulator::Archetypes::kArchetypeGrob:
		imageName = u"ConsequenceGraphics!trait_spg_wanderer.png";
		break;
	case Simulator::Archetypes::kArchetypeZealot:
		imageName = u"ConsequenceGraphics!trait_spg_zealot.png";
		break;
	default:
		return false; // No matching image
	}

	ResourceKey key;
	if (!ResourceKey::Parse(key, imageName)) {
		return false;
	}

	UTFWin::Image::GetImage(key, image);
	return true;
}

Color GetArchetypeColor(Simulator::Archetypes archetype) {
	switch (archetype) {
	case Simulator::Archetypes::kArchetypeBard:
		return Color(72, 187, 140, 255); // (0.282, 0.733, 0.549)
	case Simulator::Archetypes::kArchetypeDiplomat:
		return Color(197, 202, 71, 255); // (0.773, 0.792, 0.278)
	case Simulator::Archetypes::kArchetypeEcologist:
		return Color(162, 205, 70, 255); // (0.635, 0.804, 0.275)
	case Simulator::Archetypes::kArchetypePlayerWarrior:
		return Color(205, 70, 153, 255); // (0.804, 0.275, 0.6)
	case Simulator::Archetypes::kArchetypeScientist:
		return Color(80, 70, 205, 255);  // (0.314, 0.275, 0.804)
	case Simulator::Archetypes::kArchetypeShaman:
		return Color(76, 234, 94, 255);  // (0.3, 0.92, 0.37)
	case Simulator::Archetypes::kArchetypeTrader:
		return Color(74, 188, 217, 255); // (0.29, 0.737, 0.851)
	case Simulator::Archetypes::kArchetypeGrob:
		return Color(147, 149, 152, 255); // (0.576, 0.584, 0.596)
	case Simulator::Archetypes::kArchetypeWarrior:
		return Color(205, 63, 23, 255);   // (0.803, 0.247, 0.09)
	case Simulator::Archetypes::kArchetypeZealot:
		return Color(165, 70, 205, 255);  // (0.647, 0.275, 0.804)

	default:
		return Color(255, 255, 255, 255); // fallback to white
	}
}

member_detour(SpaceGameUi__FillStarTooltipStarInfo__detour, UI::SpaceGameUI, int(ISimulatorSerializable*)){
	int detoured(ISimulatorSerializable * serializable) {
		int toReturn = original_function(this, serializable);
		if (serializable != nullptr && serializable->GetNounID() == 65287484) {
			cStarRecord* star = object_cast<cStarRecord>(serializable);
			if (star != nullptr) {

				cEmpire* empire = StarManager.GetEmpire(star->mEmpireID);
				UTFWin::UILayout* starTooltip = this->mpSpaceStarTooltipLayout.get();
				UTFWin::IWindow* planetInfoWindow = starTooltip->FindWindowByID(0x0684BC00);
				UTFWin::IWindow* textWindow = starTooltip->FindWindowByID(0x7CFBCBE1);
				UTFWin::IWindow* archetypeImageWindow = starTooltip->FindWindowByID(0x6BE3873C);
				UTFWin::IWindow* starNameWindow = starTooltip->FindWindowByID(0x01c22501);

				if (planetInfoWindow->IsVisible() && 
					star->GetTechLevel() == TechLevel::Empire && 
					EmpireUtils::ValidNpcEmpire(empire, true, false, true) && 
					DiplomacyUtils::PlayerContactedEmpire(empire)) {

					eastl::string16 empireName = empire->mEmpireName;

					// If the empire name is too long cut the name at 14 characters and add a '.' in the end.
					if (empireName.length() > 15) {
						empireName.resize(14);
						empireName.push_back('.');
					}
					textWindow->SetCaption(empireName.c_str());
					textWindow->SetShadeColor(GetArchetypeColor(SporeModUtils::ArchetypeUtils::GetBaseArchetype(empire->mArchetype, true)));
					textWindow->SetVisible(true);
					starNameWindow->SetShadeColor(GetArchetypeColor(SporeModUtils::ArchetypeUtils::GetBaseArchetype(empire->mArchetype, true)));

					// If the star name is too long don't show the archetype imagen.
					if (star->mName.length() < 10) {
						UTFWin::IImageDrawable* archetypeImagenDrawable = object_cast<UTFWin::IImageDrawable>(archetypeImageWindow->GetDrawable());
						ImagePtr image;
						GetArchetypeImagen(image, SporeModUtils::ArchetypeUtils::GetBaseArchetype(empire->mArchetype, true));
						//UTFWin::Image::GetImage(key, image);
						archetypeImagenDrawable->SetImage(image.get());
						archetypeImageWindow->SetShadeColor(GetArchetypeColor(SporeModUtils::ArchetypeUtils::GetBaseArchetype(empire->mArchetype, true)));
						archetypeImageWindow->SetVisible(true);
					}
					else {
						archetypeImageWindow->SetVisible(false);
					}
				}
				else {
					textWindow->SetVisible(false);
					archetypeImageWindow->SetVisible(false);
					starNameWindow->SetShadeColor(Color(255, 255, 255, 255));
				}
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
	SpaceGameUi__FillStarTooltipStarInfo__detour::attach(Address(0x0106cca0));
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

