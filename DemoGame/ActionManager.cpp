//======================================================================
// ActionManager.cpp
//======================================================================
#include "framework.hpp"
#include "ActionManager.hpp"
#include "Player.hpp"
#include "../GDIEngineLib/Object.hpp"

void ActionManager::Update(Player& player) {
    FSM& fsm = player.GetFSM();
    // 기존 FSM state 처리 여기에 필요 시 연결

}

void ActionManager::DoIdle(Player& player) {

}
void ActionManager::DoMove(Player& player) {

}
void ActionManager::DoJump(Player& player) {

}
void ActionManager::DoCrouch(Player& player) {

}
void ActionManager::DoAttackS(Player& player) {

}
void ActionManager::DoAttackH(Player& player) {

}
void ActionManager::DoAttackD(Player& player) {

}
void ActionManager::DoAttackDown(Player& player) {

}
void ActionManager::DoAttackUp(Player& player) {

}
void ActionManager::DoAirAttackS(Player& player) {

}
void ActionManager::DoAirAttackH(Player& player) {

}
void ActionManager::DoBackDash(Player& player) {

}
void ActionManager::DoGuardUp(Player& player) {

}
void ActionManager::DoGuardDown(Player& player) {

}
void ActionManager::DoHitNormal(Player& player) {

}
void ActionManager::DoHitDown(Player& player) {

}
void ActionManager::DoHitAir(Player& player) {

}
void ActionManager::DoHitUp(Player& player) {

}
void ActionManager::DoRun(Player& player) {

}

