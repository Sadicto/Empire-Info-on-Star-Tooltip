#pragma once

#include <Spore\BasicIncludes.h>

#define ArchetypeWinProcHelperPtr intrusive_ptr<ArchetypeWinProcHelper>

class ArchetypeWinProcHelper 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("EmpireDetailsOnStarInfo::ArchetypeWinProcHelper");
	
	ArchetypeWinProcHelper();
	~ArchetypeWinProcHelper();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	char16_t* ArchetypeToString(Simulator::Archetypes archetype);

	bool GetArchetypeImagen(ImagePtr& image, Simulator::Archetypes archetype);

	Color GetArchetypeColor(Simulator::Archetypes archetype);
};
