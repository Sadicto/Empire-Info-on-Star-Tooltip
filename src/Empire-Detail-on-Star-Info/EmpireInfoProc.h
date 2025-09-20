#pragma once

#include <Spore\BasicIncludes.h>
#include "ArchetypeWinProcHelper.h"

#define EmpireInfoProcPtr intrusive_ptr<EmpireInfoProc>

// To avoid repeating UTFWin:: all the time.
using namespace UTFWin;

class EmpireInfoProc 
	: public IWinProc
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("Empire_Detail_on_Star_Info::EmpireInfoProc");
	
	EmpireInfoProc(ResourceKey configKey, 
		ArchetypeWinProcHelper* archetypeHelper, 
		UTFWin::IWindow* empireNameWindow,
		UTFWin::IWindow* archetypeImageWindow, 
		UTFWin::IWindow* starNameWindow, 
		UTFWin::IWindow* planetInfoWindow);

	~EmpireInfoProc();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;
	
	int GetEventFlags() const override;
	// This is the function you have to implement, called when a window you added this winproc to received an event
	bool HandleUIMessage(IWindow* pWindow, const Message& message) override;

	void HandleStarHover(Simulator::cStarRecord* star);

	ArchetypeWinProcHelperPtr archetypeHelper;

	Simulator::cEmpire* currentEmpire;

	bool colorInEmpireName;
	bool colorInArchetype;
	bool colorInStarName;

	UTFWin::IWindow* empireNameWindow;
	UTFWin::IWindow* archetypeImageWindow;
	UTFWin::IWindow* starNameWindow;
	UTFWin::IWindow* planetInfoWindow;
	
};
