#include "../RobotExecutorApplication.h"
#include <modules/community_new/entity_event/entity_event.h>

class RobotExecutorApplicationImpl : public RobotExecutorApplication {
public:
    RobotExecutorApplicationImpl(TestCaseManager& tcm, ConnectionFactoryManager& cfm): RobotExecutorApplication{tcm, cfm} {
        
    }

    void init() override {
        // 注册entity event
        NSCommunityNew::InitializeEntityEventRegister();
    }
};

std::shared_ptr <RobotExecutorApplication> createRobotExecutorApplication(TestCaseManager& tcm, ConnectionFactoryManager& cfm){
    return std::shared_ptr<RobotExecutorApplication>( new RobotExecutorApplicationImpl{tcm,cfm});
}