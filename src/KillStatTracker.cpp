#include "Configuration/Config.h"
#include "Util.h"
#include "Player.h"
#include "ScriptMgr.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <fstream>

using std::stringstream;

class KillStatTracker : public PlayerScript {
public:

  stringstream fullStream;
  bool loggingEnabled = sConfigMgr->GetBoolDefault("KillDetailedLogging.enabled", true);
  int logDumpSize = sConfigMgr->GetIntDefault("KillDetailedLogging.dumpSize", 0);

  KillStatTracker() : PlayerScript("KillStatTracker") {

    // If the file doesn't exist we will create it and add the appropriate headers

    ifstream ifile("kills.log");
    if (!ifile) {
       fullStream << "timestamp,player,faction,level,maxhealth,currenthealth,creature,creaturefaction,creaturemaxhealth,zoneid,areaid,isgamemaster\n";
       StringDump();
    }

  }

  void OnCreatureKill(Player *player, Creature *killed) override{

    if (loggingEnabled){
      stringstream killStream;


      time_t t = time(nullptr);
      killStream << TimeToTimestampStr(t) << ",";
      killStream << player->GetName() << "," << player->getFaction() << "," << player->getLevel() << "," << player->GetMaxHealth() << "," << player->GetHealth() << ",";
      killStream << killed->GetName() << "," << killed->getFaction() << "," << killed->GetMaxHealth() << ",";
      killStream << player->GetZoneId() << "," << player->GetAreaId() << "," << player->IsGameMaster();
      killStream << "\n";

      fullStream << killStream.str();
      StringDump();
    }
  }

  void OnCreatureKilledByPet(Player *petOwner, Creature *killed) override{

    if (loggingEnabled){
      stringstream killStream;


      time_t t = time(nullptr);
      killStream << TimeToTimestampStr(t) << ",";
      killStream << petOwner->GetName() << "," << petOwner->getFaction() << "," << petOwner->getLevel() << "," << petOwner->GetMaxHealth() << "," << petOwner->GetHealth() << ",";
      killStream << killed->GetName() << "," << killed->getFaction() << "," << killed->GetMaxHealth() << ",";
      killStream << petOwner->GetZoneId() << "," << petOwner->GetAreaId() << "," << petOwner->IsGameMaster();
      killStream << "\n";

      fullStream << killStream.str();
      StringDump();
    }
  }

private:

  int insertCount = 0;

  void StringDump(){

    insertCount++;

    if (insertCount > logDumpSize){
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

class kill_logging_conf : public WorldScript
{
public:
    kill_logging_conf() : WorldScript("kill_logging_conf") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        if (!reload) {
            std::string conf_path = _CONF_DIR;
            std::string cfg_file = conf_path + "/detail_logging.conf";
#ifdef WIN32
            cfg_file = "detail_logging.conf";
#endif
            std::string cfg_def_file = cfg_file + ".dist";

            sConfigMgr->LoadMore(cfg_def_file.c_str());

            sConfigMgr->LoadMore(cfg_file.c_str());
        }
    }
};

void AddKillStatTrackerScripts() {
  new KillStatTracker();
  new kill_logging_conf();
}
