#pragma once

#include <Spore\BasicIncludes.h>

#define cEmpireInfoStrategyPtr intrusive_ptr<cEmpireInfoStrategy>

///
/// In your dllmain Initialize method, add the system like this:
/// ModAPI::AddSimulatorStrategy(new cEmpireInfoStrategy(), cEmpireInfoStrategy::NOUN_ID);
///

/// The only purpose of this class is deleting the layout when exiting the space stage, I couldn't find a better way to do it.
class cEmpireInfoStrategy
	: public Simulator::cStrategy
{
public:
	static const uint32_t TYPE = id("Empire_Detail_on_Star_Info::cEmpireInfoStrategy");
	static const uint32_t NOUN_ID = TYPE;

	int AddRef() override;
	int Release() override;
	void Initialize() override;
	void Dispose() override;
	const char* GetName() const override;
	bool Write(Simulator::ISerializerStream* stream) override;
	bool Read(Simulator::ISerializerStream* stream) override;
	void Update(int deltaTime, int deltaGameTime) override;
	bool WriteToXML(Simulator::XmlSerializer* xml) override;
	void OnModeExited(uint32_t previousModeID, uint32_t newModeID) override;

	//
	// You can add more methods here
	//

	static Simulator::Attribute ATTRIBUTES[];

	static cEmpireInfoStrategy* instance;
	static cEmpireInfoStrategy* Get();

	UTFWin::UILayout* layout;

private:
	//
	// You can add members here
	//
};