//======================================================================
// ActionManager.hpp
//======================================================================
#pragma once
class Player; //전방선언

class ActionManager {
public:
    void Update(Player& player);

private:
    ///////////////////////////////////////////////////////////////////////////
    void DoIdle(Player& player);
    void DoMove(Player& player);
    void DoJump(Player& player);
    void DoCrouch(Player& player);
    void DoAttackS(Player& player);
    void DoAttackH(Player& player);
    void DoAttackD(Player& player);
    void DoAttackDown(Player& player);
    void DoAttackUp(Player& player);
    void DoAirAttackS(Player& player);
    void DoAirAttackH(Player& player);
    void DoBackDash(Player& player);
    void DoGuardUp(Player& player);
    void DoGuardDown(Player& player);
    void DoHitNormal(Player& player);
    void DoHitDown(Player& player);
    void DoHitAir(Player& player);
    void DoHitUp(Player& player);
    void DoRun(Player& player);
};
