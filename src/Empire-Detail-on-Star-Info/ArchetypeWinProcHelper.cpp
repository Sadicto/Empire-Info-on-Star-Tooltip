#include "stdafx.h"
#include "ArchetypeWinProcHelper.h"
using namespace Simulator;

ArchetypeWinProcHelper::ArchetypeWinProcHelper()
{
}


ArchetypeWinProcHelper::~ArchetypeWinProcHelper()
{
}

// For internal use, do not modify.
int ArchetypeWinProcHelper::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int ArchetypeWinProcHelper::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* ArchetypeWinProcHelper::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(ArchetypeWinProcHelper);
	return nullptr;
}

bool ArchetypeWinProcHelper::GetArchetypeImagen(ImagePtr& image, Archetypes archetype) {
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
	case Simulator::Archetypes::kArchetypeGrob:
		imageName = u"ConsequenceGraphics!trait_spg_wanderer.png";
		break;
	case Simulator::Archetypes::kArchetypeZealot:
		imageName = u"ConsequenceGraphics!trait_spg_zealot.png";
		break;
	case Simulator::Archetypes::kArchetypePlayerTrader: // Grox empire.
		imageName = u"EdsiConfig!Groxeyecon.png";
		break;
	default:
		return false;
	}

	ResourceKey key;
	if (!ResourceKey::Parse(key, imageName)) {
		return false;
	}

	UTFWin::Image::GetImage(key, image);
	return true;
}

Color ArchetypeWinProcHelper::GetArchetypeColor(Simulator::Archetypes archetype) {
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
	case Simulator::Archetypes::kArchetypePlayerTrader: // Grox empire.
		return Color(153, 39, 108, 255);
	default:
		return Color(255, 255, 255, 255); // fallback to white
	}
}
