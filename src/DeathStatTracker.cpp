#include "Configuration/Config.h"
#include "Player.h"
#include "ScriptMgr.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <fstream>

using std::stringstream;

class DeathStatTracker : public PlayerScript {
public:
  DeathStatTracker() : PlayerScript("DeathStatTracker") {}
  stringstream fullStream;

  void OnPlayerKilledByCreature(Creature *creature, Player *player) override{
    stringstream killStream;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    killStream << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << ",";
    killStream << player->GetName() << "," << player->getFaction() << "," << player->getLevel() << "," << player->GetMaxHealth() << "," << player->GetHealth() << ",";
    killStream << creature->GetName() << "," << creature->getFaction() << "," << creature->GetMaxHealth() << ",";
    killStream << player->GetZoneId() << "," << player->GetAreaId() << "," << player->IsGameMaster();
    killStream << "\n";

    fullStream << killStream.str();
    StringDump();
  }

 
private:

  int insertCount = 0;

  void StringDump(){

    insertCount++;

    if (insertCount > 1000){
      // Dump to log
      ofstream logFile;
      logFile.open ("deaths.log", ofstream::app);
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

void AddDeathStatTrackerScripts() { new DeathStatTracker(); }
