#pragma once

#include <BWAPI.h>
#include <set>
namespace Tools
{
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

    BWAPI::Unit GetClosestUnitTo(BWAPI::Position p, const BWAPI::Unitset& units);
    BWAPI::Unit GetClosestUnitTo(BWAPI::Unit unit, const BWAPI::Unitset& units);

    int CountUnitsOfType(BWAPI::UnitType type, const BWAPI::Unitset& units);
    
    int CountCompletedUnitsOfType(BWAPI::UnitType type, const BWAPI::Unitset& units);
    
    template <class... Types>
    BWAPI::Unitset getUnitsOfTypes(Types... types) {
        
        BWAPI::Unitset unitOfTypes;
        std::set<BWAPI::UnitType> typesSet = { types... };
        
        for (auto& u : BWAPI::Broodwar->getAllUnits()) {
            if (typesSet.find(u->getType()) != typesSet.end()) {
                unitOfTypes.insert(u);
            }
        }
        return unitOfTypes;

    }

    BWAPI::Unitset getWorkers();
    BWAPI::Unit GetUnitOfType(BWAPI::UnitType type);
    BWAPI::Unit GetDepot();


    bool BuildBuilding(BWAPI::UnitType type);

    void DrawUnitBoundingBoxes();
    void DrawUnitCommands();
    //void DrawMineralFieldInfo();
    
    
    void SmartRightClick(BWAPI::Unit unit, BWAPI::Unit target);

    int GetTotalSupply(bool inProgress = false);
    
    BWAPI::Unitset getUnitsOnScreen();
    BWAPI::Unitset getUnitsOnScreen(BWAPI::Unitset& units);


    void DrawUnitHealthBars();
    void DrawHealthBar(BWAPI::Unit unit, double ratio, BWAPI::Color color, int yOffset);
}