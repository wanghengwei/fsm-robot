
#include "state_SleepInRoom.h"
#include <stdexcept>
#include <logger.h>
#include <QtCore/QDebug>

namespace state {

    void StateSleepInRoom::perform(std::map<std::string, std::string>& info) {
        // todo
        // throw std::runtime_error{"todo"};
        loggers::TESTCASE().info("zzzzz...");
	}
	
	void StateSleepInRoom::clean() {
		
	}

}
