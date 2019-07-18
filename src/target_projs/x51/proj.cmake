### 分支
set(PROJ_BRANCH_BASE http://172.17.100.22/svn/starx52)
set(PROJ_BRANCH trunc CACHE STRING "svn repo")
message("PROJ_BRANCH=${PROJ_BRANCH}")
set(PROJ_BRANCH ${PROJ_BRANCH_BASE}/${PROJ_BRANCH})

### 版本号
set(PROJ_REV HEAD CACHE STRING "svn repo")
message("PROJ_REV=${PROJ_REV}")

# 需要包含的项目的源代码。一般来说是消息的注册的cpp
set(PROJ_SOURCES
# modules/chat/shared/EventRegister.cpp
# modules/guild/shared/EventRegister.cpp
# modules/shop/shared/EventRegister.cpp
game_proxy/h5/H5ConvertEvent.cpp
global_service/global_lord_logic_server/src/global_lord_helper.cpp
global_service/global_lord_logic_server/src/global_lord_logic_def.cpp
global_service/multi_snakesmatch_server/Logic/snakes_event_reg.cpp
modules/achievement/shared/player_title_info.cpp
modules/anni_feedback/share/anni_feedback_info.cpp
modules/anni_feedback/share/event/anni_feedback_event_register.cpp
modules/channel/shared/EventRegisterChannel.cpp
modules/chat/shared/event_register_chat.cpp
modules/chat/shared/event_register_chat.cpp
modules/cloth_collocation/share/cloth_collocation_event_imp.cpp
#
modules/magic_world/magic_sanxiao/share/magic_sanxiao_info_def.cpp
modules/magic_world/share/magic_world_event_reg.cpp
modules/new_quest/share/event/new_quest_event_reg.cpp
#modules/magic_world/magic_sanxiao/share/magic_sanxiao_event_wrapper_reg.cpp
#
#new garden
modules/activity_center/x5_dreamer/share/x5_dreamer_event_reg.cpp
modules/new_garden/share/events/new_garden_event_reg.cpp
modules/community_new/entity_event/entity_event_inline_function.cpp
modules/community_new/entity_event/entity_event_register.cpp
modules/community_new/entity_event/entity_event.cpp
modules/community/entity_event/entity_event.cpp
modules/community/shared/events/community_event_register.cpp
modules/contacts/shared/EventRegisterContacts.cpp
modules/contacts/shared/StarFriendInfo.cpp
modules/global_async_pk/share/global_async_pk_event_reg.cpp
modules/global_community/share/event/global_community_event_reg.cpp
modules/global_dance/share/event/global_dance_event_reg.cpp
modules/global_dance/share/event/global_dance_match_event_reg.cpp
modules/global_dance/share/global_dance_info_def.cpp
modules/global_pk/share/event/globalpk_event_reg.cpp
modules/global_team/share/global_team_event_reg.cpp
modules/global_team/share/global_team_predef.cpp
modules/global_wedding/shared/events/event_global_wedding_reg.cpp
modules/guild/shared/event_register_guild.cpp
modules/hide_and_seek/share/hide_and_seek_def.cpp
modules/hide_and_seek/share/hide_and_seek_event_reg.cpp
modules/hide_and_seek/share/hide_and_seek_game_def.cpp
modules/homeland/shared/events/homeland_events_reg.cpp
modules/homeland/shared/homeland_info.cpp
modules/ipgw/shared/events/eventdef.cpp
modules/item/shared/events/event_register.cpp
modules/item/inventorysys/BasicItem.cpp
modules/item/inventorysys/item/BasicFitmentItem.cpp
#modules/item/inventorysys/stringtable/ItemDataTable.cpp
#modules/item/inventorysys/ItemManager.cpp
modules/login/shared/events/event_register.cpp
modules/match_select/shared/match_select_event_reg.cpp
modules/pet/shared/events/pet_events_register.cpp
modules/playermanager/interface/CharacterInfo.cpp
modules/playermanager/shared/events/playermanager_event_register.cpp
modules/quest/shared/events/EventRegister.cpp
modules/quest/shared/QuestInstance_BaseImp.cpp
modules/quest/shared/QuestTemplateEvtImp.cpp
modules/RaffleCity/capsule_toys_lottery/share/CapsuleToysEventRegister.cpp
modules/RaffleCity/user_define_lottery/share/UserDefineLotteryRegister.cpp
modules/ring_team/share/RingTeamEventReg.cpp
modules/shop/shared/event_register_shop.cpp
modules/snake_game_out/share/snake_game_out_event_reg.cpp
modules/sns/event/event_register.cpp
modules/story_mode/share/story_mode_event_reg.cpp
share/community/community_point.cpp
share/date_time.cpp
share/game_event/EventFragmentManager.cpp
share/game_event/star/gamelogic/GLEvent_EndGame.cpp
share/game_event/star/gamelogic/GLEvent_GameActionResult.cpp
share/game_event/star/gamelogic/GLEvent_LoadingProgress.cpp
share/game_event/star/gamelogic/GLEvent_LoginVerify.cpp
share/game_event/star/gamelogic/GLEvent_LoginVerifyReply.cpp
share/game_event/star/gamelogic/GLEvent_PrepareReady.cpp
share/third_part/cjson/cJSON.cpp
zone_server/GuildServer/Guild/super_guild/share/event/event_super_guild_reg.cpp
zone_server/GuildServer/Guild/super_guild/share/event/event_super_guild_reg.cpp
modules/contacts/shared/EventRegisterContacts.cpp
modules/channel/shared/EventRegisterChannel.cpp
)

# 必须包含的include path
set(PROJ_INCLUDE_DIRS
    /
    /share
)

# 获取项目的代码
message("fetch project source from ${PROJ_BRANCH}@${PROJ_REV}")
FetchContent_Declare(proj
    SVN_REPOSITORY ${PROJ_BRANCH}
    SVN_USERNAME shentongle
    SVN_PASSWORD stl769
    SVN_REVISION -r${PROJ_REV}
)
FetchContent_GetProperties(proj)
if(NOT proj_POPULATED)
    FetchContent_Populate(proj)
    # 需要打一下补丁
    execute_process(COMMAND svn patch ${CMAKE_SOURCE_DIR}/target_projs/${PROJ}/proj.patch WORKING_DIRECTORY ${proj_SOURCE_DIR})
endif()
# 把实际的rev写入一个文件以便别的地方使用
execute_process(COMMAND svn info --show-item revision WORKING_DIRECTORY ${proj_SOURCE_DIR} OUTPUT_FILE ${CMAKE_BINARY_DIR}/svn_rev.txt)

# 拼接出绝对路径
list(TRANSFORM PROJ_SOURCES PREPEND "${proj_SOURCE_DIR}/")
list(TRANSFORM PROJ_INCLUDE_DIRS PREPEND "${proj_SOURCE_DIR}")
list(APPEND PROJ_SOURCES target_projs/x51/dummy_src/QuestTemplate.cpp)

set_source_files_properties(${PROJ_SOURCES} PROPERTIES COMPILE_OPTIONS "-w")

# file(GLOB PROJECT_DEPENDENT_SOURCES "${CMAKE_SOURCE_DIR}/src/${TARGET_PROJ}/*.cpp")
