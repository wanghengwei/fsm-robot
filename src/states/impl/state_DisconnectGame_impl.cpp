
#include "state_DisconnectGame.h"
#include <stdexcept>
#include <robot/basic_robot.h>
#include <robot/connection_types.h>
namespace state {

    void StateDisconnectGame::perform(std::map<std::string, std::string>& info) {
        // todo
        // throw std::runtime_error{"todo"};
        auto& conn = robot().connection(CONN_GAME);
        conn.close();
    }

    void StateDisconnectGame::clean() {}
}
