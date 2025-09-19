#include "stdafx.h"
#include "EmpireInfoProc.h"
#include "Spore-Mod-Utils/Include/SporeModUtils.h"

using namespace Simulator;
using namespace SporeModUtils;

EmpireInfoProc::EmpireInfoProc(ResourceKey configKey,
	ArchetypeWinProcHelper* archetypeHelper,
	UTFWin::IWindow* empireNameWindow,
	UTFWin::IWindow* archetypeImageWindow,
	UTFWin::IWindow* starNameWindow,
	UTFWin::IWindow* planetInfoWindow)
{
	currentEmpire = nullptr;
	this->archetypeHelper = archetypeHelper;
	PropertyListPtr mainConfig;
	PropManager.GetPropertyList(configKey.instanceID, configKey.groupID, mainConfig);
	ResourceKey colorInEmpireNameConfigKey;
	ResourceKey colorInArchetypeConfigKey;
	ResourceKey colorInStarNameConfigKey;
	App::Property::GetKey(mainConfig.get(), 0xCF65ADB9, colorInEmpireNameConfigKey);
	App::Property::GetKey(mainConfig.get(), 0xA28C2E7F, colorInArchetypeConfigKey);
	App::Property::GetKey(mainConfig.get(), 0x350B05E5, colorInStarNameConfigKey);

	PropertyListPtr colorInEmpireNameConfig;
	PropManager.GetPropertyList(colorInEmpireNameConfigKey.instanceID, colorInEmpireNameConfigKey.groupID, colorInEmpireNameConfig);
	App::Property::GetBool(colorInEmpireNameConfig.get(), 0xC44BCF56, colorInEmpireName);

	PropertyListPtr colorInArchetypeConfig;
	PropManager.GetPropertyList(colorInArchetypeConfigKey.instanceID, colorInArchetypeConfigKey.groupID, colorInArchetypeConfig);
	App::Property::GetBool(colorInArchetypeConfig.get(), 0xFD5DF708, colorInArchetype);

	PropertyListPtr colorInStarNameConfig;
	PropManager.GetPropertyList(colorInStarNameConfigKey.instanceID, colorInStarNameConfigKey.groupID, colorInStarNameConfig);
	App::Property::GetBool(colorInStarNameConfig.get(), 0xCF3F3562, colorInStarName);

	this->empireNameWindow = empireNameWindow;
	empireNameWindow->SetVisible(false);
	this->archetypeImageWindow = archetypeImageWindow;
	archetypeImageWindow->SetVisible(false);
	this->starNameWindow = starNameWindow;
	this->planetInfoWindow = planetInfoWindow;
}


EmpireInfoProc::~EmpireInfoProc()
{
}

// For internal use, do not modify.
int EmpireInfoProc::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int EmpireInfoProc::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* EmpireInfoProc::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IWinProc);
	CLASS_CAST(EmpireInfoProc);
	return nullptr;
}

// This method returns a combinations of values in UTFWin::EventFlags.
// The combination determines what types of events (messages) this window procedure receives.
// By default, it receives mouse/keyboard input and advanced messages.
int EmpireInfoProc::GetEventFlags() const
{
	return kEventFlagBasicInput | kEventFlagAdvanced;
}

// The method that receives the message. The first thing you should do is probably
// checking what kind of message was sent...
bool EmpireInfoProc::HandleUIMessage(IWindow* window, const Message& message)
{
	
	// Return true if the message was handled, and therefore no other window procedure should receive it.
	return false;
}

void EmpireInfoProc::HandleStarHover(Simulator::cStarRecord* star) {
	cEmpire* empire = StarManager.GetEmpire(star->mEmpireID);
	if (planetInfoWindow->IsVisible() &&
		star->GetTechLevel() == TechLevel::Empire &&
		EmpireUtils::ValidNpcEmpire(empire, true, false, true) &&
		DiplomacyUtils::PlayerContactedEmpire(empire)) {
		if (currentEmpire != empire) {
			currentEmpire = empire;
			eastl::string16 empireName = empire->mEmpireName;
			Archetypes empireArchetype = SporeModUtils::ArchetypeUtils::GetBaseArchetype(empire->mArchetype, true);
			Math::Color archetypeColor = archetypeHelper->GetArchetypeColor(empireArchetype);

			// If the empire name is too long cut the name at 14 characters and add a '.' in the end.
			if (empireName.length() > 15) {
				empireName.resize(14);
				empireName.push_back('.');
			}
			empireNameWindow->SetCaption(empireName.c_str());
			if (colorInEmpireName) {
				empireNameWindow->SetShadeColor(archetypeColor);
			}
			empireNameWindow->SetVisible(true);
			if (colorInStarName) {
				starNameWindow->SetShadeColor(archetypeColor);
			}


			// If the star name is too long don't show the archetype imagen.
			if (star->mName.length() < 10) {
				ImagePtr image;
				archetypeHelper->GetArchetypeImagen(image, empireArchetype);

				UTFWin::IImageDrawable* archetypeImagenDrawable = object_cast<UTFWin::IImageDrawable>(archetypeImageWindow->GetDrawable());
				archetypeImagenDrawable->SetImage(image.get());
				if (colorInArchetype) {
					archetypeImageWindow->SetShadeColor(archetypeColor);
				}
				archetypeImageWindow->SetVisible(true);
			}
			else {
				archetypeImageWindow->SetVisible(false);
			}
		}

	}
	else {
		currentEmpire = nullptr;
		empireNameWindow->SetVisible(false);
		archetypeImageWindow->SetVisible(false);
		if (colorInStarName) {
			starNameWindow->SetShadeColor(Color(255, 255, 255, 255));
		}
	}
}
