//======================================================================
// Player.cpp
//======================================================================

#include "framework.hpp"
#include "Player.hpp"

#include "../GDIEngineLib/InputManager.hpp"
#include "../GDIEngineLib/WinGameApp.hpp"

#undef max
#undef min

Player::Player(ControlType type) : controlType(type) {
	SIZE size = RenderManager::Get().GetBitmapSize(L"StandingLoop", 0);
	colliderWidth = static_cast<int>(size.cx * 0.9f);  // 폭은 조금 좁혀서 판정 예쁘게
	colliderHeight = static_cast<int>(size.cy * 1.0f); // 전체 이미지 높이를 그대로 쓰되
}

Player::~Player() {}

void Player::ResetFlag() {
	isGrounded = false;
	isLeftWall = false;
	isRightWall = false;
	isCollision = false;
	isFacingLeft = false; //기본 왼쪽을 봄	
}

void Player::CheckIsEnclosure() {
	if (Object::IsAABBCollision(this, pLeftWall)) isLeftWall = true;
	if (Object::IsAABBCollision(this, pRightWall)) isRightWall = true;
	if (Object::IsAABBCollision(this, pGround)) isGrounded = true;
	if (Object::IsAABBCollision(this, m_pEnemy)) isCollision = true;
	UpdateFacingDirection(); //false : 오른쪽 / true : 왼쪽
}

void Player::ApplyGravity() {
	if (isGrounded) {
		gravitySpeed = 0;
		position.y = pGround->GetCollider().top - colliderHeight;

		if (isKnockbackPending &&
			(n == StateHitNormal || n == StateGuardUp || n == StateGuardDown)) {
			if (knockbackAccum < knockbackTotal) {
				float step = std::min(knockbackStep, knockbackTotal - knockbackAccum);
				bool didKnockback = false;

				if (!(isLeftWall && isRightWall)) {
					if (!isRightWall && !knockbackFromLeft) {
						position.x += step;
						didKnockback = true;
					}
					else if (!isLeftWall && knockbackFromLeft) {
						position.x -= step;
						didKnockback = true;
					}
				}

				// 피격자가 막혔을 경우 → 공격자에게 넘기기
				if (!didKnockback && m_pEnemy) {
					Player* attacker = m_pEnemy;
					if (!(attacker->isLeftWall && attacker->isRightWall)) {
						if (!attacker->isRightWall && knockbackFromLeft)
							attacker->position.x += step;
						else if (!attacker->isLeftWall && !knockbackFromLeft)
							attacker->position.x -= step;
					}
				}

				knockbackAccum += step;
			}
			else {
				if (fsm.GetState() == BasicGuard &&
					(n == StateGuardUp || n == StateGuardDown)) {

					fsm.ChangeState(isGuardDown ? BasicCrouch : BasicIdle);
					n = isGuardDown ? StateCrouch : StateIdle;

					isActionLocked = false;
					currentAttackType = AttackNone;
				}
				isKnockbackPending = false;
				knockbackAccum = 0.0f;
				knockbackTotal = 0.0f;
			}
		}
		else if (isCollision) {
			if (!isRightWall && !isLeftWall)
				isFacingLeft ? position.x += moveSpeed : position.x -= moveSpeed;
		}
	}
	else {
		CheckPlayerY();
		gravitySpeed += gravityAccel;
		position.y += gravitySpeed;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
//Update///////////////////////////////////////////////////////////////////////////////////////

void Player::Update() {
	ResetFlag();          // 충돌/상태 플래그 초기화
	CheckIsEnclosure();   // 필드 접촉 여부

	CheckInput();         // 키 입력 상태 수집 → inputState 갱신
	UpdateState();        // inputState 기반 FSM 상태 예약
	fsm.Update();         // FSM 상태 진행 및 전이	

	CheckFSM();           // 애니메이션 등 시각 처리 / 실제로 행동하는 부분

	UpdateAttackHitBox(); // 피격판정

	ApplyGravity();       // 중력 처리 및 위치 이동
	CheckAnimationEnd();
	//if (InputManager::Get().IsKeyPressed(VK_SPACE)) //디버그용	

}

///////////////////////////////////////////////////////////////////////////////////////////////
//Handle///////////////////////////////////////////////////////////////////////////////////////

void Player::HandleIdle() {
	n = StateIdle;

	// 혹시 공중에서 Idle 상태로 잘못 들어온 경우 방지
	if (!isGrounded) {
		fsm.ChangeState(BasicJump);
	}
}

void Player::HandleMove() {
	switch (inputState.move) {
	case InputRight:
		if (!isRightWall) {
			position.x += moveSpeed;
			IsForwardInput() ? n = StateWalk : n = StateWalkBack;
		}
		break;

	case InputLeft:
		if (!isLeftWall) {
			position.x -= moveSpeed;
			IsForwardInput() ? n = StateWalk : n = StateWalkBack;
		}
		break;

	default: break;
	}
}

void Player::HandleJump() {
	// 점프 시작 타이밍에만 초기화 - 위로 올라가는 부분
	if (isGrounded) {
		gravitySpeed -= jumpForce;
		isGrounded = false;
	}

	// 공중 이동 (좌우)
	switch (inputState.move) {
	case InputRight:
		if (!isRightWall)
			position.x += airControlSpeed;
		if (IsForwardInput()) {
			n = StateJumpFront;
			currentJumpType = JumpFront;
		}
		else {
			n = StateJumpBack;
			currentJumpType = JumpBack;
		}

		break;
	case InputLeft:
		if (!isLeftWall)
			position.x -= airControlSpeed;
		if (IsForwardInput()) {
			n = StateJumpFront;
			currentJumpType = JumpFront;
		}
		else {
			n = StateJumpBack;
			currentJumpType = JumpBack;
		}

		break;
	default:
		n = StateJumpNeutral;
		currentJumpType = JumpNeutral;
		break;
	}

}

void Player::HandleCrouch() {
	n = StateCrouch;

}

void Player::HandleAttack() {
	switch (inputState.attack) {
	case InputAttackS:		currentAttackType = AttackS;	n = StateAttackS;	break;
	case InputAttackH:		currentAttackType = AttackH;	n = StateAttackH;	break;
	case InputAttackD:
		if (inputState.jump == InputDown) {
			currentAttackType = AttackCrouchD;
			n = StateAttackDown;
		}
		else if (IsForwardInput()) {
			currentAttackType = AttackUpD;
			n = StateAttackUp;
		}
		else {
			currentAttackType = AttackD;
			n = StateAttackD;
		}
		break;
	default: break;
	}

	LetsAttack();
}

void Player::HandleJumpAttack() {
	switch (inputState.attack) {
	case InputAttackS:
		if (!isAirAttackUsed) {
			currentAttackType = AttackJumpS;
			n = StateAirAttackS;
			isAirAttackUsed = true;
			isJumpSFrameFixed = false;

			LetsAttack();
		}
		break;

	case InputAttackH:
		if (!isAirAttackUsed) {
			currentAttackType = AttackJumpH;
			n = StateAirAttackH;
			isAirAttackUsed = true;

			LetsAttack();
		}
		break;

	default:
		break;
	}
}

void Player::LetsAttack() {
	isActionLocked = true;

	if (fsm.GetStep() == StepStart || currentHitBoxes.empty()) {
		auto& info = m_FrameMap[Literal(CharacterState(n))];
		info.isFirstTime = true;
		info.currentFrame = 0;
		info.localFrameCounter = 0;

		currentHitBoxes.clear(); // 이전 히트박스 제거
		CreateAttackHitBox();
	}
}

void Player::HandleHit() {
	switch (n) {
	case StateHitUp:
		if (fsm.GetStep() == StepStart) {
			fsm.NextStep(true);
		}
		else if (fsm.GetStep() == StepLoop && isCurrentAnimFinished) {
			isActionLocked = false;
			fsm.NextStep(true);
		}
		break;

	default:
		if (fsm.GetStep() == StepStart) {
			fsm.NextStep(true);
		}
		else if (fsm.GetStep() == StepLoop && isCurrentAnimFinished) {
			isActionLocked = false;
			fsm.NextStep(true);
		}
		break;
	}
}


void Player::HandleHitAir() {
	std::wstring key = Literal(StateHitAir);
	auto& info = m_FrameMap[key];	int maxFrame = RenderManager::Get().GetFrameMax(key);

	switch (currentHitAirStep) {
	case HitAirRise:
		info.currentFrame = 0;
		info.localFrameCounter = 0;
		RenderManager::Get().DrawImage(key, info.currentFrame, screenPos.x, screenPos.y, !isFacingLeft);
		currentHitAirStep = HitAirLoop;
		break;

	case HitAirLoop:
		isCurrentAnimFinished = RenderManager::Get().LoopImageInstance(key, info, screenPos.x, screenPos.y, !isFacingLeft);
		if (info.currentFrame >= 4) {  // 마지막 프레임까지 도달하면 반복 전환
			currentHitAirStep = HitAirFall;
			m_HitAirFrameRate = 0;
		}
		break;

	case HitAirFall:
		m_HitAirFrameRate++;
		if (m_HitAirFrameRate >= RenderManager::Get().GetFrameRate()) {
			m_HitAirFrameRate = 0;
			info.currentFrame++;
			if (info.currentFrame >= 5) info.currentFrame = 3;  // 3→4 반복
		}
		RenderManager::Get().DrawImage(key, info.currentFrame, screenPos.x, screenPos.y, !isFacingLeft);

		if (isGrounded) {
			n = StateHitUp;
			currentHitAirStep = HitAirRise;
			fsm.ChangeTo(BasicHit); // 상태 유지하면서 StepStart로 리셋
		}
		break;
	}
}

void Player::HandleGuard() {
	n = isGuardDown ? StateGuardDown : StateGuardUp;

	switch (fsm.GetStep()) {
	case StepStart:
		fsm.NextStep(true); // 바로 Loop로 진입
		break;

	case StepLoop:
		if (!isGuarding && isCurrentAnimFinished) {
			fsm.NextStep(true); // → StepEnd
		}
		break;

	case StepEnd:
		isActionLocked = false;
		fsm.ChangeState(isGuardDown ? BasicCrouch : BasicIdle);
		n = isGuardDown ? StateCrouch : StateIdle;
		break;
	}
}

void Player::HandleBackDash() {
	n = StateBackDash;

	if (fsm.GetStep() == StepStart) {
		// Step 전이만 진행 (애니메이션 초기화는 CheckFSM → LetsDash 같은 데서 할 수도 있음)
		fsm.NextStep(true);
	}
	else if (fsm.GetStep() == StepLoop) {
		float dashSpeed = 18.0f; // 원하는 대쉬 속도 설정

		// 바라보는 방향 기준으로 반대 방향 이동 (뒤로 빠지기)
		if (isFacingLeft && !isRightWall)
			position.x += dashSpeed;
		else if (!isFacingLeft && !isLeftWall)
			position.x -= dashSpeed;

		// 애니메이션이 끝났으면 FSM 전이
		if (isCurrentAnimFinished) {
			isActionLocked = false;
			fsm.NextStep(true);
		}
	}
	else if (fsm.GetStep() == StepEnd) {
		// StepEnd → Idle 복귀
		fsm.ChangeState(BasicIdle);
		n = StateIdle;
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void Player::CheckPlayerY() {
	if (gravitySpeed < -3.0f) {
		currentJumpStep = JumpStepUp;
		isJumpPeekDone = false;
	}
	else if (isJumpPeekDone)
		currentJumpStep = JumpStepDown; //하강
	else
		currentJumpStep = JumpStepPeak;
}

void Player::CheckFSM() {
	switch (fsm.GetState()) {
	case BasicIdle:			HandleIdle();		break;
	case BasicMove:			HandleMove();		break;
	case BasicJump:			HandleJump();		break;
	case BasicCrouch:		HandleCrouch();		break;
	case BasicAttack:		HandleAttack();		break;
	case BasicJumpAttack:	HandleJumpAttack(); break;
	case BasicHit:			HandleHit();		break;
	case BasicGuard:		HandleGuard();		break;
	case BasicBackDash:		HandleBackDash();	break;
	default: break;
	}
}

void Player::UpdateState() {
	if (isGrounded && fsm.GetState() == BasicJumpAttack) {
		fsm.ChangeState(BasicIdle);
		isAirAttackUsed = false;
		isJumpSFrameFixed = false;
		isActionLocked = false;
		currentAttackType = AttackNone;
		return;
	}

	

	bool backKeyNow = IsBackwardInput();

	switch (backDashState) {
	case BackDashInputState::Idle:
		if (backKeyNow && !wasBackPressedLastFrame) {
			backDashState = BackDashInputState::FirstTapDetected;
			backDashTimer = 0;
		}
		break;

	case BackDashInputState::FirstTapDetected:
		if (!backKeyNow) {
			backDashState = BackDashInputState::WaitForSecondTap;
		}
		break;

	case BackDashInputState::WaitForSecondTap:
		backDashTimer++;
		if (backDashTimer > backDashMaxGap) {
			backDashState = BackDashInputState::Idle;
		}
		else if (backKeyNow && !wasBackPressedLastFrame) {
			// 백대쉬 발동
			fsm.ChangeTo(BasicBackDash);
			n = StateBackDash;
			isActionLocked = true;
			backDashState = BackDashInputState::Idle;
		}
		break;
	}

	wasBackPressedLastFrame = backKeyNow;

	if (isGrounded) {
		isAirAttackUsed = false;
		isJumpSFrameFixed = false;
	}

	if (isActionLocked)
		return;

	if (fsm.GetState() == BasicHit || fsm.GetStep() == StepEnd)
		return;

	if (fsm.GetState() == BasicAttack || fsm.GetState() == BasicJumpAttack
		|| fsm.GetState() == BasicHit || fsm.GetState() == BasicGuard)
		return;

	if (isGrounded) {
		if (inputState.attack != InputAttackIdle) {
			fsm.ChangeState(BasicAttack);
			return;
		}
		if (inputState.jump == InputUp) {
			fsm.ChangeState(BasicJump);
			return;
		}
		if (inputState.jump == InputDown) {
			fsm.ChangeState(BasicCrouch);
			return;
		}
		if (inputState.move == InputLeft || inputState.move == InputRight) {
			fsm.ChangeState(BasicMove);
			return;
		}
		fsm.ChangeState(BasicIdle);
	}
	else {
		if (inputState.attack == InputAttackD) {
			fsm.ChangeState(BasicJump);
			return;
		}
		if (inputState.attack != InputAttackIdle) {
			fsm.ChangeState(BasicJumpAttack);
			return;
		}
		fsm.ChangeState(BasicJump);
	}
}

void Player::CheckInput() {
	inputState = {};
	RawInput raw;

	// 키 상태는 항상 갱신
	if (controlType == ControlType::Player2) {
		raw.down = InputManager::Get().IsKeyDown(VK_DOWN);
		raw.up = InputManager::Get().IsKeyDown(VK_UP);
		raw.right = InputManager::Get().IsKeyDown(VK_RIGHT);
		raw.left = InputManager::Get().IsKeyDown(VK_LEFT);

		raw.attackS = InputManager::Get().IsKeyDown(VK_NUMPAD1);
		raw.attackH = InputManager::Get().IsKeyDown(VK_NUMPAD2);
		raw.attackD = InputManager::Get().IsKeyDown(VK_NUMPAD3);
	}
	else if (controlType == ControlType::Player1) {
		raw.down = InputManager::Get().IsKeyDown('S');
		raw.up = InputManager::Get().IsKeyDown('W');
		raw.right = InputManager::Get().IsKeyDown('D');
		raw.left = InputManager::Get().IsKeyDown('A');

		raw.attackS = InputManager::Get().IsKeyDown('G');
		raw.attackH = InputManager::Get().IsKeyDown('H');
		raw.attackD = InputManager::Get().IsKeyDown('J');
	}

	isGuarding = false;
	// 상대가 내 오른쪽에 있음 = 나는 왼쪽 보고 있음
	bool enemyIsRight = m_pEnemy && m_pEnemy->position.x > position.x;

	if ((enemyIsRight && raw.left) || (!enemyIsRight && raw.right)) {
		isGuarding = true;
		isGuardDown = raw.down;
	}

	reservedAttack = InputAttackIdle;

	if (!isActionLocked) {
		

		if (raw.down)		inputState.jump = InputDown;
		else if (raw.up)	inputState.jump = InputUp;
		else				inputState.jump = InputJumpIdle;

		if (raw.right && !raw.left)			inputState.move = InputRight;
		else if (raw.left && !raw.right)	inputState.move = InputLeft;
		else								inputState.move = Neutral;

		if (raw.attackS)		inputState.attack = InputAttackS;
		else if (raw.attackH)	inputState.attack = InputAttackH;
		else if (raw.attackD)	inputState.attack = InputAttackD;
		else					inputState.attack = InputAttackIdle;
	}
	else {
		// 액션 락 중에도 공격 입력이 있으면 예약만 받음
		if (raw.attackS)		reservedAttack = InputAttackS;
		else if (raw.attackH)	reservedAttack = InputAttackH;
		else if (raw.attackD)	reservedAttack = InputAttackD;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Render/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Player::Render() {
	screenPos = RenderManager::Get().WorldToScreen(position);
	std::wstring key = Literal(CharacterState(n));
	auto& info = m_FrameMap[key];

	// 히트 시 공중 피격 처리
	if (fsm.GetState() == BasicHit && n == StateHitAir) {
		HandleHitAir();
	}
	// 히트 시 일반 피격 처리 (넉백 유지용)
	else if (fsm.GetState() == BasicHit && n == StateHitNormal) {
		if (isKnockbackPending) {
			// 프레임 1 고정 + FSM 전이 방지
			info.currentFrame = 1;
			info.localFrameCounter = 0;
			RenderManager::Get().DrawImage(key, info.currentFrame, screenPos.x, screenPos.y, !isFacingLeft);
			isCurrentAnimFinished = false;
		}
		else {
			isCurrentAnimFinished = RenderManager::Get().LoopImageInstance(key, info, screenPos.x, screenPos.y, !isFacingLeft);
		}
	}
	// 점프
	else if (fsm.GetState() == BasicJump) {
		RenderJumpFrame();
	}
	// 공중 공격
	else if (fsm.GetState() == BasicJumpAttack) {
		if (n == StateAirAttackS && isJumpSFrameFixed) {
			RenderManager::Get().DrawImage(key, info.currentFrame, screenPos.x, screenPos.y, !isFacingLeft);
		}
		else {
			isCurrentAnimFinished = RenderManager::Get().LoopImageInstance(key, info, screenPos.x, screenPos.y, !isFacingLeft);
			if (n == StateAirAttackS) {
				int maxFrame = RenderManager::Get().GetFrameMax(key);
				if (info.currentFrame >= maxFrame - 1) {
					isJumpSFrameFixed = true;
				}
			}
		}
	}
	// 가드 전용 애니메이션
	else if (fsm.GetState() == BasicGuard) {
		if (m_GuardFrameRate < RenderManager::Get().GetFrameRate()) {
			info.currentFrame = 0;
			m_GuardFrameRate++;
		}
		else {
			info.currentFrame = 1;
		}
		info.localFrameCounter = 0;
		RenderManager::Get().DrawImage(key, info.currentFrame, screenPos.x, screenPos.y, !isFacingLeft);
	}
	// 일반 애니메이션 처리
	else {
		isCurrentAnimFinished = RenderManager::Get().LoopImageInstance(key, info, screenPos.x, screenPos.y, !isFacingLeft);
	}

	// 콜라이더, 상태표시

	/*
	 
	RenderManager::Get().DrawColliderBox(GetCollider());
	std::wstring stateStr = GetStateText();
	RenderManager::Get().DrawTextSimple(stateStr, static_cast<int>(screenPos.x), static_cast<int>(screenPos.y) - 40);
	// 히트박스
	for (const auto& hb : currentHitBoxes) {
		Gdiplus::Color color = hb.hasHit ? Gdiplus::Color::Gray : Gdiplus::Color::Orange;
		RenderManager::Get().DrawColliderBox(hb.box, color);
	}	
	
	*/

	// 체력바
	int barX = (controlType == ControlType::Player1) ? 180 : 1140;
	bool reverse = (controlType == ControlType::Player2);
	int barY = 50;
	int barWidth = 600;
	int barHeight = 50;
	Gdiplus::Color color = Gdiplus::Color::Red;
	RenderManager::Get().DrawHealthBar(barX, barY, barWidth, barHeight, hp, maxHp, color, reverse);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::CreateAttackHitBox() {
	HitBox hb;
	int boxWidth = 0;
	int boxHeight = 0;
	int boxY = 0;//박스 Y축 이동
	int boxX = 0;//박스 X축 이동	

	switch (currentAttackType) { // 315 * 357 > StandingLoop size
	case AttackS:			hb.startFrame = 4;	hb.endFrame = 4;	hb.damage = 12;
		boxWidth = 360;		boxHeight = 100;	boxY = 120;		boxX = 0;		break;

	case AttackH:			hb.startFrame = 3;	hb.endFrame = 5;	hb.damage = 8;
		boxWidth = 200;		boxHeight = 300;	boxY = -30;		boxX = -60;		break;

	case AttackD:			hb.startFrame = 5;	hb.endFrame = 9;	hb.damage = 17;
		boxWidth = 380;		boxHeight = 370;	boxY = -120;	boxX = -100;	break;

	case AttackCrouchD:		hb.startFrame = 4;	hb.endFrame = 5;	hb.damage = 20;
		boxWidth = 160;		boxHeight = 120;	boxY = 237;		boxX = 0;		break;

	case AttackUpD:			hb.startFrame = 5;	hb.endFrame = 7;	hb.damage = 8;
		boxWidth = 200;		boxHeight = 200;	boxY = 0;		boxX = -130;	break;

	case AttackJumpS:		hb.startFrame = 4;	hb.endFrame = 4;	hb.damage = 18;
		boxWidth = 270;		boxHeight = 315;	boxY = 120;		boxX = -150;	break;

	case AttackJumpH:		hb.startFrame = 2;	hb.endFrame = 5;	hb.damage = 25;
		boxWidth = 300;		boxHeight = 380;	boxY = -10;		boxX = -150;	break;

	default:		return; // 무효한 타입은 무시
	}

	hb.offsetX = std::abs(boxX);   // 방향에 따라 처리하므로 절대값 저장
	hb.offsetY = boxY;
	hb.width = boxWidth;
	hb.height = boxHeight;
	hb.knockbackDistance = boxWidth + std::abs(boxX) + 10;

	currentHitBoxes.push_back(hb);
}

void Player::UpdateAttackHitBox() {
	if (fsm.GetState() != BasicAttack && fsm.GetState() != BasicJumpAttack) //공격을 시도했을때만
		return;

	for (auto& hb : currentHitBoxes) {
		if (hb.hasHit) continue; //이미 판정나면 넘기기

		// 1. 히트박스 위치 재계산
		RECT col = GetCollider();
		if (isFacingLeft) {
			hb.box.right = col.left + hb.offsetX;
			hb.box.left = hb.box.right - hb.width;
		}
		else {
			hb.box.left = col.right - hb.offsetX;
			hb.box.right = hb.box.left + hb.width;
		}
		hb.box.top = col.top + hb.offsetY;
		hb.box.bottom = hb.box.top + hb.height;

		// 2. 유효 프레임 내에서 적과 충돌 체크
		FrameInfo& info = m_FrameMap[Literal(CharacterState(n))];
		if (info.currentFrame >= hb.startFrame && info.currentFrame <= hb.endFrame) { //유효 프레임 범위 안에 있을때만
			if (Object::IsAABBCollisionRect(hb.box, m_pEnemy->GetCollider())) { //히트박스와 상대방 충돌판정				
				if (m_pEnemy->IsInvincibleOnHit()) {
					continue;
				}

				// 면역 체크
				if (m_pEnemy->currentAttackType == AttackUpD && //상대가 UpD 공격중일때
					(currentAttackType == AttackJumpS || currentAttackType == AttackJumpH)) { //공중공격을 한다면
					continue; //면역
				}

				// 가드 유효성 판정
				bool EnemyisGuarding = m_pEnemy->GetIsGuarding(); //이건 상대가 가드하는중인지
				bool EnemyisCrouchGuard = m_pEnemy->GetIsGuardDown(); //아랫방향키 입력했는지

				bool isValidGuard = false;

				switch (currentAttackType) {
				case AttackS:
				case AttackH:
				case AttackUpD:
					isValidGuard = true; // 모두 가드 가능
					break;

				case AttackD:
					isValidGuard = !EnemyisCrouchGuard; // 서서 가드만 가능
					break;

				case AttackCrouchD:
					isValidGuard = EnemyisCrouchGuard; // 앉아 가드만 가능
					break;

				case AttackJumpS:
				case AttackJumpH:
					isValidGuard = !EnemyisCrouchGuard; // 서서 가드만 가능
					break;

				default:
					isValidGuard = false;
					break;
				}

				if (EnemyisGuarding && isValidGuard) {
					if (m_pEnemy->CanGuard()) {
						m_pEnemy->OnGuard(); // 가드가능 상태
					}
					else {
						m_pEnemy->OnHit(hb.damage); // 가드 불가능 상태 → 피격 처리
					}
				}
				else {
					m_pEnemy->OnHit(hb.damage); // 가드 안 함 또는 유효하지 않음 → 피격
				}

				m_pEnemy->isKnockbackPending = true;
				m_pEnemy->knockbackAmount = hb.knockbackDistance;
				m_pEnemy->knockbackTotal = static_cast<float>(hb.knockbackDistance);
				m_pEnemy->knockbackFromLeft = isFacingLeft;
				hb.hasHit = true;

				//여기까지 왔다는건 히트했든 가드했든 했다는거임
				if (reservedAttack != InputAttackIdle) {


					auto& info = m_FrameMap[Literal(CharacterState(n))];
					info.isFirstTime = true;
					info.currentFrame = 0;
					info.localFrameCounter = 0;

					inputState.attack = reservedAttack;
					reservedAttack = InputAttackIdle;
					isAttackWindowOpen = false;
					currentHitBoxes.clear();

					if (isGrounded)
						fsm.ChangeTo(BasicAttack);
					else
						fsm.ChangeTo(BasicJumpAttack);

					// 바로 다음 공격 실행
					fsm.Update();
					CheckFSM();
					return;
				}
				else {
					// 연계 공격 없으면 attack window만 열어줌
					isAttackWindowOpen = true;
				}


			}
		}
	}
}

void Player::OnGuard() {
	isActionLocked = true;
	n = isGuardDown ? StateGuardDown : StateGuardUp;
	m_GuardFrameRate = 0;
	fsm.ChangeState(BasicGuard);

	auto& info = m_FrameMap[Literal(CharacterState(n))];
	info.isFirstTime = true;
	info.currentFrame = 0;
	info.localFrameCounter = 0;

	fsm.Update();   // 상태 갱신 무야호~!
	CheckFSM();
}

void Player::UpdateHitBoxPosition(HitBox& hb) {
	RECT col = GetCollider();
	if (isFacingLeft) {
		hb.box.right = col.left - hb.offsetX;
		hb.box.left = hb.box.right - hb.width;
	}
	else {
		hb.box.left = col.right + hb.offsetX;
		hb.box.right = hb.box.left + hb.width;
	}
	hb.box.top = col.top + hb.offsetY;
	hb.box.bottom = hb.box.top + hb.height;
}

void Player::OnHit(int damage) {

	hp -= damage;
	hp = std::max(0, hp);

	if (fsm.GetState() == BasicHit && fsm.GetStep() != StepHit) {
		if (n == StateHitNormal) {
			n = StateHitDown;
			fsm.ChangeTo(BasicHit); // 상태 유지 + StepStart로 리셋
		}
		return;
	}

	if (!isGrounded) {
		n = StateHitAir;
		currentHitAirStep = HitAirRise;     // 초기화
		m_HitAirFrameRate = 0;
		isHitAirFinished = false;
	}
	else if (m_pEnemy->currentAttackType == AttackCrouchD) {
		n = StateHitDown;
	}
	else {
		n = StateHitNormal;
	}

	isActionLocked = true;
	fsm.SetStepHit();
}

void Player::CheckAnimationEnd() {
	if (fsm.GetStep() != StepLoop) return;

	if (isCurrentAnimFinished) {
		fsm.NextStep(true);

		// 초기화
		FrameInfo& info = m_FrameMap[Literal(CharacterState(n))];
		info.isFirstTime = true;
		info.currentFrame = 0;
		info.localFrameCounter = 0;

		isCurrentAnimFinished = false; // 다시 리셋
		isActionLocked = false;
	}
}

void Player::RenderJumpFrame() {

	std::wstring key = Literal(CharacterState(n));
	auto& info = m_FrameMap[key];
	int maxFrame = RenderManager::Get().GetFrameMax(key);

	switch (currentJumpStep) {
	case JumpStepUp:
		info.currentFrame = 0;
		info.localFrameCounter = 0; // 항상 리셋해서 강제 프레임 고정
		RenderManager::Get().DrawImage(key, info.currentFrame, screenPos.x, screenPos.y, !isFacingLeft);
		break;

	case JumpStepPeak:
		isCurrentAnimFinished = RenderManager::Get().LoopImageInstance(key, m_FrameMap[key], screenPos.x, screenPos.y, !isFacingLeft);
		if (info.currentFrame >= maxFrame - 2) {
			isJumpPeekDone = true;
			m_JumpFrameRate = 0;
		}

		break;

	case JumpStepDown:
		m_JumpFrameRate++; //이유는 모르겠지만, 3번 점프까지는 개빠르게 올라감 !@#$

		if (m_JumpFrameRate >= RenderManager::Get().GetFrameRate()) { // 전역 속도 기준 사용
			m_JumpFrameRate = 0;
			info.currentFrame++;
		}

		if (info.currentFrame >= maxFrame || info.currentFrame < maxFrame - 2)
			info.currentFrame = maxFrame - 2;

		RenderManager::Get().DrawImage(key, info.currentFrame, screenPos.x, screenPos.y, !isFacingLeft);
		break;

	default: break;
	}
}

std::wstring Player::Literal(CharacterState state) {
	switch (state) {
	case StateNeutralOnce: return L"NeutralOnce";
	case StateIdle:        return L"StandingLoop";
	case StateCrouch:      return L"CrouchLoop";
	case StateWalk:        return L"WalkLoop";
	case StateWalkBack:    return L"WalkbackLoop";
	case StateRun:         return L"RunLoop";
	case StateRunEnd:      return L"RunEnd";
	case StateBackDash:    return L"BackDash";
	case StateJumpNeutral: return L"JumpNormal";
	case StateJumpFront:   return L"JumpFront";
	case StateJumpBack:    return L"JumpBack";
	case StateGuardUp:     return L"GuardUp";
	case StateGuardDown:   return L"GuardDown";
	case StateAttackS:     return L"AttackS";
	case StateAttackH:     return L"AttackH";
	case StateAttackD:     return L"AttackD";
	case StateAttackDown:  return L"AttackDown";
	case StateAttackUp:    return L"AttackUp";
	case StateAirAttackS:  return L"AirAttackS";
	case StateAirAttackH:  return L"AirAttackH";
	case StateHitNormal:   return L"HitNormal";
	case StateHitDown:     return L"HitDown";
	case StateHitAir:      return L"HitAir";
	case StateHitUp:       return L"HitUp";
	default:               return L"NeutralOnce";
	}
}
