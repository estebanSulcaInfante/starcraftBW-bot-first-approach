#pragma once

#include "MapTools.h"
#include <vector>

#include <BWAPI.h>
class FrameHistory {
private:
	std::vector<int> history;
	size_t index;
	size_t count;
public:
	FrameHistory() = default;
	FrameHistory(size_t size);


	void addFrame(int frameData);

	std::string serializeToString() const;
	void printHistory();
	float getAverage();
	int getLast();
};

class StarterBot
{
    MapTools m_mapTools;
	
	// p1 requirements
	FrameHistory frameHistory;
	size_t mineralFrameCount;
	size_t mineralOnPreviousFrame;
	
	// p2 requirements
	bool botHaveControl;
	size_t totalMineralFieldsOnScreen;
	size_t totalWorkers;
	struct MineralData {
		int lastResources;
		int lastFrame;
		FrameHistory frameHistory;
		int getSpeedInFrames() { return (BWAPI::Broodwar->getFrameCount() - lastFrame); }
		MineralData() : frameHistory(20){}
	};
	std::map < BWAPI::Unit, MineralData> lastMineralFieldData;


	// helper functions to get you started with bot programming and learn the API
    void sendIdleWorkersToMinerals();
    void trainAdditionalWorkers();
    void buildAdditionalSupply();
    void drawDebugInformation();
	void checkMineralDifference();
	void initMineralFieldResourcesMap();
	void checkMineralFieldDifference();
	void drawMineralFieldInfo();
	void drawIncomeInfo();
	void drawWorkersAndSupplyInfo();
	void manageWorkers();
	void sendWorkerOnlyOnceToMineral();

public:

    StarterBot();

    // functions that are triggered by various BWAPI events from main.cpp

	void onStart();
	void onFrame();
	void onEnd(bool isWinner);
	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitMorph(BWAPI::Unit unit);
	void onSendText(std::string text);
	void onUnitCreate(BWAPI::Unit unit);
	void onUnitComplete(BWAPI::Unit unit);
	void onUnitShow(BWAPI::Unit unit);
	void onUnitHide(BWAPI::Unit unit);
	void onUnitRenegade(BWAPI::Unit unit);

};



