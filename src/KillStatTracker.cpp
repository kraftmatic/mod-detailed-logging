

#include "Configuration/Config.h"
#include "Player.h"
#include "ScriptMgr.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <fstream>

using std::stringstream;

class KillStatTracker : public PlayerScript {
public:
  KillStatTracker() : PlayerScript("KillStatTracker") {}
  stringstream fullStream;

  void OnCreatureKill(Player *player, Creature *killed) override{
    stringstream killStream;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    killStream << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << ",";
    killStream << player->GetName() << "," << player->getFaction() << "," << player->getLevel() << "," << player->GetMaxHealth() << "," << player->GetHealth() << ",";
    killStream << killed->GetName() << "," << killed->getFaction() << "," << killed->GetMaxHealth() << ",";
    killStream << player->GetZoneId() << "," << player->GetAreaId() << "," << player->IsGameMaster();
    killStream << "\n";

    fullStream << killStream.str();
    StringDump();
  }

  void OnCreatureKilledByPet(Player *petOwner, Creature *killed) override{
    stringstream killStream;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    killStream << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << ",";
    killStream << petOwner->GetName() << "," << petOwner->getFaction() << "," << petOwner->getLevel() << "," << petOwner->GetMaxHealth() << "," << petOwner->GetHealth() << ",";
    killStream << killed->GetName() << "," << killed->getFaction() << "," << killed->GetMaxHealth() << ",";
    killStream << petOwner->GetZoneId() << "," << petOwner->GetAreaId() << "," << petOwner->IsGameMaster();
    killStream << "\n";

    fullStream << killStream.str();
    StringDump();
  }

private:

  int insertCount = 0;

  void StringDump(){

    insertCount++;

    if (insertCount > 1){
      // Dump to log
      ofstream logFile;
      logFile.open ("kills.log", ofstream::app);
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

void AddKillStatTrackerScripts() { new KillStatTracker(); }
