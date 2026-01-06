//======================================================================
//Animation.hpp
//======================================================================
#pragma once
enum CharacterState //png폴더와 맵핑되는 enum
{
	//Basic////////////////////////////////////////////////////////////////////////////
	StateNeutralOnce = 0,// 1회성 중립 모션
	StateIdle,			//
	StateCrouch,		// 앉기 (CrouchLoop)
	//Move////////////////////////////////////////////////////////////////////////////
	StateWalk,			// 걷기
	StateWalkBack,		// 뒤로 걷기    
	StateRun,			// 달리기
	StateRunEnd,		// 달리기 끝
	StateBackDash,		// 뒤로 대시
	//Jump////////////////////////////////////////////////////////////////////////////
	StateJumpNeutral,	// 제자리 점프
	StateJumpFront,		// 앞으로 점프
	StateJumpBack,		// 뒤로 점프	
	//Guard////////////////////////////////////////////////////////////////////////////
	StateGuardUp,		// 서서 가드
	StateGuardDown,		// 앉아서 가드
	//Attack////////////////////////////////////////////////////////////////////////////
	StateAttackS,		// 지상 약공격
	StateAttackH,		// 지상 강공격
	StateAttackD,		// 지상 특수공격
	StateAttackDown,	// 다운 공격
	StateAttackUp,		// 대공 공격
	StateAirAttackS,	// 공중 약공격
	StateAirAttackH,	// 공중 강공격    
	//Hit////////////////////////////////////////////////////////////////////////////
	StateHitNormal,		// 약경직
	StateHitDown,		// 다운 피격
	StateHitAir,		// 공중 피격
	StateHitUp,			// 공중 피격 후 기상	
	////////////////////////////////////////////////////////////////////////////
	StateNone			// 에러 방지용
};

enum CharacterStep {
	StepStart = 0,
	StepLoop,
	StepEnd,
	StepHit,
}; // render 이동

class Animation {
public:
	Animation() = default;
	~Animation() = default;

	void NeutralOnce();
	void Idle();
	void Crouch();

	void Walk();
	void WalkBack();
	void Run();
	void RunEnd();
	void BackDash();

	void JumpNeutral();
	void JumpFront();
	void JumpBack();

	void GuardUp();
	void GuardDown();

	void AttackS();
	void AttackH();
	void AttackD();
	void AttackDown();
	void AttackUp();
	void AirAttackS();
	void AirAttackH();

	void HitNormal();
	void HitDown();
	void HitAir();
	void HitUp();

	void None();
private:
	CharacterStep m_step;
};