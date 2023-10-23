#include "StarterBot.h"
#include "Tools.h"
#include "MapTools.h"

StarterBot::StarterBot() : frameHistory(20), mineralFrameCount(0), mineralOnPreviousFrame(0) {}


// Called when the bot starts!
void StarterBot::onStart()
{
    // Set our BWAPI options here    
	BWAPI::Broodwar->setLocalSpeed(15);
    BWAPI::Broodwar->setFrameSkip(0);
    
    // Enable the flag that tells BWAPI top let users enter input while bot plays
    BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);

    // Call MapTools OnStart
    m_mapTools.onStart();
    
    std::cout << "Map: ";
    std::cout << "Height" << BWAPI::Broodwar->mapHeight() << "Width" << BWAPI::Broodwar->mapWidth();
}

// Called on each frame of the game
void StarterBot::onFrame()
{
    // Update our MapTools information
    m_mapTools.onFrame();

    // Send our idle workers to mine minerals so they don't just stand there
    //sendIdleWorkersToMinerals();

    // Train more workers so we can gather more income
    //trainAdditionalWorkers();

    // Check mineral income information
    checkMineralDifference();

    // Build more supply if we are going to run out soon
    //buildAdditionalSupply();

    // Draw unit health bars, which brood war unfortunately does not do
    Tools::DrawUnitHealthBars();

    // Draw some relevent information to the screen to help us debug the bot
    drawDebugInformation();
}

// Send our idle workers to mine minerals so they don't just stand there
void StarterBot::sendIdleWorkersToMinerals()
{
    // Let's send all of our starting workers to the closest mineral to them
    // First we need to loop over all of the units that we (BWAPI::Broodwar->self()) own
    const BWAPI::Unitset& myUnits = BWAPI::Broodwar->self()->getUnits();
    for (auto& unit : myUnits)
    {
        // Check the unit type, if it is an idle worker, then we want to send it somewhere
        if (unit->getType().isWorker() && unit->isIdle())
        {
            // Get the closest mineral to this worker unit
            BWAPI::Unit closestMineral = Tools::GetClosestUnitTo(unit, BWAPI::Broodwar->getMinerals());

            // If a valid mineral was found, right click it with the unit in order to start harvesting
            if (closestMineral) { unit->rightClick(closestMineral); }
        }
    }
}

// Train more workers so we can gather more income
void StarterBot::trainAdditionalWorkers() 
{
    const BWAPI::UnitType workerType = BWAPI::Broodwar->self()->getRace().getWorker();
    const int workersWanted = 20;
    const int workersOwned = Tools::CountUnitsOfType(workerType, BWAPI::Broodwar->self()->getUnits());
    if (workersOwned < workersWanted)
    {
        // get the unit pointer to my depot
        const BWAPI::Unit myDepot = Tools::GetDepot();

        // if we have a valid depot unit and it's currently not training something, train a worker
        // there is no reason for a bot to ever use the unit queueing system, it just wastes resources
        if (myDepot && !myDepot->isTraining()) { myDepot->train(workerType); }
    }
}

// Build more supply if we are going to run out soon
void StarterBot::buildAdditionalSupply()
{
    // Get the amount of supply supply we currently have unused
    const int unusedSupply = Tools::GetTotalSupply(true) - BWAPI::Broodwar->self()->supplyUsed();

    // If we have a sufficient amount of supply, we don't need to do anything
    if (unusedSupply >= 2) { return; }

    // Otherwise, we are going to build a supply provider
    const BWAPI::UnitType supplyProviderType = BWAPI::Broodwar->self()->getRace().getSupplyProvider();

    const bool startedBuilding = Tools::BuildBuilding(supplyProviderType);
    if (startedBuilding)
    {
        BWAPI::Broodwar->printf("Started Building %s", supplyProviderType.getName().c_str());
    }
}

// Draw Mineral Income on left top
void StarterBot::drawIncomeInfo()
{
    // serialize
    std::string incomeInfo = frameHistory.serializeToString();
    std::string mineralFrameCountInfo = "Frame Count: " +
        std::to_string(mineralFrameCount) + 
        "\n";
    std::string averageIncome = "Average Income (frames): " +
        std::to_string(frameHistory.getAverage()) +
        "\n";
    std::string previousMineralFrameInfo = "Minerals on previous frame: " +
        std::to_string(mineralOnPreviousFrame) + 
        "\n";

    // draw on left-top
    BWAPI::Broodwar->drawTextScreen(BWAPI::Position(10, 10), incomeInfo.c_str());
    BWAPI::Broodwar->drawTextScreen(BWAPI::Position(10, 20), mineralFrameCountInfo.c_str());
    BWAPI::Broodwar->drawTextScreen(BWAPI::Position(120, 20), averageIncome.c_str());
    BWAPI::Broodwar->drawTextScreen(BWAPI::Position(10, 30), previousMineralFrameInfo.c_str());
}

// Draw some relevent information to the screen to help us debug the bot
void StarterBot::drawDebugInformation()
{
    drawIncomeInfo();
    Tools::DrawUnitCommands();
    Tools::DrawUnitBoundingBoxes();
    Tools::DrawMineralFieldInfo();
}

void StarterBot::checkMineralDifference()
{
    
    // When worker deliver minerals
    if (BWAPI::Broodwar->self()->minerals() - mineralOnPreviousFrame >= 8)
    {
        int framesBeforeLastIncome = mineralFrameCount;
        frameHistory.addFrame(framesBeforeLastIncome);
        mineralFrameCount = 0;
    }
    else
    {
        mineralFrameCount += 1;
    }
    mineralOnPreviousFrame = BWAPI::Broodwar->self()->minerals();
}

// Called whenever the game ends and tells you if you won or not
void StarterBot::onEnd(bool isWinner)
{
    std::cout << "We " << (isWinner ? "won!" : "lost!") << "\n";
}

// Called whenever a unit is destroyed, with a pointer to the unit
void StarterBot::onUnitDestroy(BWAPI::Unit unit)
{
	
}

// Called whenever a unit is morphed, with a pointer to the unit
// Zerg units morph when they turn into other units
void StarterBot::onUnitMorph(BWAPI::Unit unit)
{
	
}

// Called whenever a text is sent to the game by a user
void StarterBot::onSendText(std::string text) 
{ 
    
    if (text == "/map")
    {
        m_mapTools.toggleDraw();
    }
    else if (text == "hola")
    {
        BWAPI::Broodwar->sendText("mundo:)");
    }
}

// Called whenever a unit is created, with a pointer to the destroyed unit
// Units are created in buildings like barracks before they are visible, 
// so this will trigger when you issue the build command for most units
void StarterBot::onUnitCreate(BWAPI::Unit unit)
{ 
	
}

// Called whenever a unit finished construction, with a pointer to the unit
void StarterBot::onUnitComplete(BWAPI::Unit unit)
{
	
}

// Called whenever a unit appears, with a pointer to the destroyed unit
// This is usually triggered when units appear from fog of war and become visible
void StarterBot::onUnitShow(BWAPI::Unit unit)
{ 
	
}

// Called whenever a unit gets hidden, with a pointer to the destroyed unit
// This is usually triggered when units enter the fog of war and are no longer visible
void StarterBot::onUnitHide(BWAPI::Unit unit)
{ 
	
}

// Called whenever a unit switches player control
// This usually happens when a dark archon takes control of a unit
void StarterBot::onUnitRenegade(BWAPI::Unit unit)
{ 
	
}


// ******************************************************************


FrameHistory::FrameHistory(size_t size) : history(size), index(0), count(0) {}

void FrameHistory::addFrame(int frameData) {
    history[index] = frameData;
    index = (index + 1) % history.size();
    if (count < history.size()) count++;
}

void FrameHistory::printHistory() {
    for (size_t i = 0; i < count; ++i) {
        size_t idx = (index + history.size() - count + i) % history.size();
        std::cout << history[idx] << " ";
    }
    std::cout << std::endl;
}
std::string FrameHistory::serializeToString() const {
    std::string result = "Frame History: ";
    for (size_t i = 0; i < count; ++i) {
        size_t idx = (index + history.size() - count + i) % history.size();
        result += std::to_string(history[idx]);
        if (i < count - 1) result += ", ";
    }
    return result;
}
float FrameHistory::getAverage()
{
    float average = 0;
    for (size_t i = 0; i < history.size(); i++)
    {
        average += history[i];
    }
    average = average / history.size();
    return average;
}

