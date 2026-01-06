//======================================================================
// Player.hpp
//======================================================================

#pragma once
#include "../GDIEngineLib/Object.hpp"
#include "FSM.hpp"
#include "StaticObject.hpp"
#include "../GDIEngineLib/RenderManager.hpp"
#include <unordered_map>
//#include "../GDIEngineLib/Scene.hpp"


struct HitBox {
	RECT box;                // 히트박스의 실제 영역
	int startFrame = 0;      // 히트 발생 시작 프레임
	int endFrame = 0;        // 히트 유지 끝 프레임
	int damage = 0;          // 데미지 수치
	bool hasHit = false;     // 중복 타격 방지용
	int knockbackDistance = 0;

	int offsetX = 0;
	int offsetY = 0;
	int width = 0;
	int height = 0;
};

enum class ControlType {
	Player1,
	Player2
};

enum class BackDashInputState {
	Idle,
	FirstTapDetected,
	WaitForSecondTap
};


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

struct RawInput {
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;

	bool attackS = false;
	bool attackH = false;
	bool attackD = false;
};

enum InputMoaveState {
	Neutral = 0,
	InputLeft,
	InputRight,
};

enum InputJumpState {
	InputJumpIdle = 0,
	InputUp,
	InputDown
};

enum InputAttackState {
	InputAttackIdle = 0,
	InputAttackS,
	InputAttackH,
	InputAttackD
};

enum JumpType {
	JumpNone,
	JumpFront,
	JumpBack,
	JumpNeutral
};

enum JumpStep {
	JumpStepUp,        // 상승
	JumpStepPeak,      // 최고점 (무속도 구간)
	JumpStepDown       // 하강
};

enum HitAirStep {
	HitAirRise,
	HitAirLoop,
	HitAirFall
};

enum AttackType {
	AttackNone,
	AttackS,
	AttackH,
	AttackD,
	AttackCrouchD,
	AttackUpD,
	AttackJumpS,
	AttackJumpH,
};

struct InputLogicState {
	CharacterState png;
	InputAttackState attack;
	InputMoaveState move;
	InputJumpState jump;
};

/////////////////////////////////////////////////////////////////////////

class Player : public Object {
public:
	Player(ControlType type);
	~Player() override;

	void Update() override;
	void Render() override;

	void CheckFSM();
	void CheckInput();
	std::wstring Literal(CharacterState state);

	void SetWall(StaticObject* left, StaticObject* right, StaticObject* bottom) {
		pLeftWall = left;
		pRightWall = right;
		pGround = bottom;
	}

	void UpdateState();
	bool GetIsGrounded() const {
		return isGrounded;
	}

	void SetEnemy(Player* enemy) { m_pEnemy = enemy; }
	Player* GetEnemy() const { return m_pEnemy; }

	void SetHP(int value) { hp = std::clamp(value, 0, maxHp); }
	int GetHP() const { return hp; }

	std::wstring GetStateText() const {
		switch (fsm.GetState()) {
		case BasicHit:			return L"Hit";
		case BasicIdle:			return L"Idle";
		case BasicMove:			return L"Move";
		case BasicJump:			return L"Jump";
		case BasicCrouch:		return L"Crouch";
		case BasicAttack:		return L"Attack";
		case BasicJumpAttack:	return L"JumpAttack";
		case BasicGuard:		return L"Guard";
		default:				return L"Unknown";
		}
	}

	FSM& GetFSM() { return fsm; }
	InputLogicState GetInputLogicState() { return inputState; }

	void CreateAttackHitBox();
	void UpdateAttackHitBox();
	void OnHit(int damage);
	void UpdateHitBoxPosition(HitBox& hb);
	void RenderJumpFrame();


	bool GetIsGuarding() const {
		return isGuarding;
	}

	bool GetIsGuardDown() const {
		return isGuardDown;
	}

	bool IsForwardInput() {
		return (inputState.move == InputRight && !isFacingLeft) ||
			(inputState.move == InputLeft && isFacingLeft);
	}

	bool IsBackwardInput() {
		return !IsForwardInput() && inputState.move != Neutral;
	}

	void LetsAttack();
	void CheckAnimationEnd();
	void CheckPlayerY();
	void OnGuard();
	bool CanGuard() const {
		BasicState s = fsm.GetState();
		return (s == BasicIdle || s == BasicMove || s == BasicCrouch);
	}

	bool IsInvincibleOnHit() const {
		// 백대쉬 상태는 기본적으로 무적
		if (fsm.GetState() == BasicBackDash)
			return true;

		// 피격 상태 중 무적 상태들
		if (fsm.GetState() != BasicHit)
			return false;

		switch (n) {
		case StateHitDown:
		case StateHitAir:
		case StateHitUp:
			return true;
		default:
			return false;
		}
	}

private:
	float gravitySpeed = 0.0f;
	float gravityAccel = 0.97f;
	float moveSpeed = 10.0f;
	float jumpForce = 43.0f;
	//float jumpForce = 200.0f; //테스트용
	float airControlSpeed = 3.0f;
	float m_center = 0.0f;

	int hp = 100;
	int maxHp = 100;

	int m_JumpFrameRate = 0;
	int m_HitAirFrameRate = 0;
	int m_GuardFrameRate = 0;
	int n;

	int knockbackAmount = 0; // 누적 넉백 이동 거리

	FSM fsm;
	ControlType controlType = ControlType::Player1;
	//크아악 이 수많은 플그는 뭐냐!
	bool isGrounded = false;
	bool isLeftWall = false;
	bool isRightWall = false;
	bool isCollision = false;
	bool isFacingLeft = false;
	bool isJumpPeekDone = false;
	bool isHitAirFinished = false;
	bool isActionLocked = false;
	bool isAirAttackUsed = false;
	bool isJumpSAttack = false;
	bool isGuarding = false;
	bool isGuardDown = false;
	bool isAttackWindowOpen = false;  // 히트 시 잠깐 열림
	bool wasBackPressedLastFrame = false;

	BackDashInputState backDashState = BackDashInputState::Idle;
	int backDashTimer = 0;
	const int backDashMaxGap = 6;

	bool isKnockbackPending = false;
	float knockbackTotal = 0.0f;      // 목표 넉백 거리
	float knockbackAccum = 0.0f;      // 현재까지 누적된 넉백
	float knockbackStep = 30.0f;       // 프레임당 넉백량
	bool knockbackFromLeft = true;   // 왼쪽에서 맞았는지
	bool isJumpSFrameFixed = false;

	bool isCurrentAnimFinished = false;

	StaticObject* pLeftWall = nullptr;
	StaticObject* pRightWall = nullptr;
	StaticObject* pGround = nullptr;

	Player* m_pEnemy = nullptr;
	Vector2 screenPos = { 0,0 };

	InputLogicState inputState;
	InputAttackState reservedAttack;

	std::vector<HitBox> currentHitBoxes;
	AttackType currentAttackType = AttackNone;
	JumpType currentJumpType = JumpNone;
	JumpStep currentJumpStep = JumpStepUp;
	HitAirStep currentHitAirStep = HitAirRise;

	std::unordered_map<std::wstring, FrameInfo> m_FrameMap;
	int currentAttackFrame = 0;




private:
	void HandleIdle(); //FSM상태와 1대1 대응되는 함수
	void HandleMove();
	void HandleJump();
	void HandleCrouch();
	void HandleAttack();
	void HandleJumpAttack();
	void HandleHit();
	void HandleHitAir();
	void HandleGuard();
	void HandleBackDash();




	void ResetFlag();
	void CheckIsEnclosure();
	void ApplyGravity();

	void UpdateFacingDirection() { //기본 왼쪽을 바라보고 있음.
		m_center = RenderManager::Get().GetCenterX();
		RECT playerCollider = GetCollider();


		if (m_center < (playerCollider.right + playerCollider.left) / 2.0f) {
			isFacingLeft = true;
			return;
		}

		isFacingLeft = false;
	}
};
