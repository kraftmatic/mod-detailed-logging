#include "Configuration/Config.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Group.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <fstream>

using std::stringstream;

class ZoneAreaTracker : public PlayerScript {
public:
  stringstream fullStream;
  ZoneAreaTracker() : PlayerScript("ZoneAreaTracker") {  }

  void OnUpdateZone(Player *player, uint32 newZone, uint32 newArea) override{
    stringstream zoneStream;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    Group* group = player->GetGroup();

    zoneStream << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << ",";
    zoneStream << player->GetName() << "," << player->getFaction() << "," << player->getLevel() << "," << player->GetMaxHealth() << "," << player->GetHealth() << ",";
    zoneStream << newZone << "," << newArea << (group != NULL) << ",";

    if (group != NULL){
      zoneStream << group->isRaidGroup();
    } else {
      zoneStream << "false";
    }

    zoneStream << "\n";

    fullStream << zoneStream.str();
    StringDump();
  }

  void OnUpdateArea(Player *player, uint32 oldArea, uint32 newArea) override{
    stringstream areaStream;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    Group* group = player->GetGroup();

    areaStream << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << ",";
    areaStream << player->GetName() << "," << player->getFaction() << "," << player->getLevel() << "," << player->GetMaxHealth() << "," << player->GetHealth() << ",";
    areaStream << "000000" << "," << newArea << (group != NULL) << ",";

    if (group != NULL){
      areaStream << group->isRaidGroup();
    } else {
      areaStream << "false";
    }

    areaStream << "\n";

    fullStream << areaStream.str();
    StringDump();
 
  }

private:

  int insertCount = 0;

  void StringDump(){

    insertCount++;

    if (true){
      // Dump to log
      ofstream logFile;
      logFile.open ("zonearea.log", ofstream::app);
      logFile << fullStream.rdbuf();
      logFile.close();

      // Reset the buffer
      fullStream.str("");
      fullStream.clear();

      // Reset line count
      insertCount = 0;
    }
  }
};

void AddZoneAreaTrackerScripts() { new ZoneAreaTracker(); }
