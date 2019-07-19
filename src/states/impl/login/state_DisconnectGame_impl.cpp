
#include <login/state_DisconnectGame.h>
#include <stdexcept>
#include <net_base/basic_robot.h>
#include <net_x51/connection_types.h>

namespace state {
namespace login {
    class StateDisconnectGameImpl final : public StateDisconnectGame {
    public:
        using StateDisconnectGame::StateDisconnectGame;

        void perform() override {
            // todo
            // throw std::runtime_error{"todo"};
            auto& conn = robot().connection(CONN_GAME);
            writeBeginLog();
            conn.close();
            writeEndLogOK();
        }

        void clean() override {}
        };

    StateDisconnectGame* StateDisconnectGame::create(QState* parent) {
        return new StateDisconnectGameImpl{parent};
    }
}
}