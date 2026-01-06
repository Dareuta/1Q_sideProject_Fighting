
//======================================================================
// FSM.cpp
//======================================================================
#include "FSM.hpp"

void FSM::Update() { //분기, 현재 상태에 맞게 알아서 들어감
	switch (basicState) {
	case BasicIdle:
		HandleIdle(); break;
	case BasicMove:
		HandleMove(); break;
	case BasicJump:
		HandleJump(); break;
	case BasicCrouch:
		HandleCrouch(); break;
	case BasicAttack:
		HandleAttack(); break;
	case BasicHit:
		HandleHit(); break;
	case BasicJumpAttack:
		HandleJumpAttack(); break;
	case BasicGuard:
		HandleGuard(); break;
	case BasicBackDash:   
		HandleBackDash(); break; // ← 추가
	default: break;
	}
}


void FSM::HandleIdle() {
	if (Hit() || Guard())
		return;

	if (basicState != nextState) {
		ChangeTo(nextState);
	}
}

void FSM::HandleMove() {
	if (Hit() || Guard())
		return;

	if (basicState != nextState) {
		ChangeTo(nextState);
	}
}

void FSM::HandleJump() {
	if (Hit())
		return;

	if (step == StepStart) {
		step = StepLoop;
	}
	else if (step == StepLoop) {
		if (nextState != BasicJump)
			step = StepEnd;
	}
	else if (step == StepEnd) {
		ChangeTo(nextState);
	}
}

void FSM::HandleCrouch() {
	if (Hit() || Guard())
		return;

	if (step == StepStart) {
		step = StepLoop;
	}
	else if (step == StepLoop) {
		if (nextState != BasicCrouch)
			step = StepEnd;
	}
	else if (step == StepEnd) {
		ChangeTo(nextState);
	}
}

void FSM::HandleAttack() {
	if (Hit())
		return;

	if (step == StepStart) {
		step = StepLoop;
	}
	else if (step == StepLoop) {
		//Attack 내부에서 애니메이션 끝났는지 검증 후 NextStep

	}
	else if (step == StepEnd) {
		ChangeTo(BasicIdle); //Idle로 복귀
	}
}

void FSM::HandleJumpAttack() {
	if (Hit())
		return;

	if (step == StepStart) {
		step = StepLoop;
	}
	else if (step == StepLoop) {

	}
	else if (step == StepEnd) {
		ChangeTo(BasicJump); // 다시 공중 상태로
	}
}

void FSM::HandleHit() {
	if (step == StepStart) {
		step = StepLoop;
	}
	else if (step == StepLoop) {
		// 경직 애니메이션이 끝났는지 외부에서 체크해서 NextStep 호출 필요
	}
	else if (step == StepEnd) {
		ChangeTo(BasicIdle); //Idle로 복귀
	}
}

void FSM::HandleGuard() {
	if (step == StepStart) {
		step = StepLoop;
	}
	else if (step == StepLoop) {
		// 외부에서 애니메이션 종료를 감지해서 NextStep() 호출 필요
	}
	else if (step == StepEnd) {
		ChangeTo(nextState); // Hit과 동일하게 Idle로 복귀
	}
}

void FSM::HandleBackDash() {
	if (step == StepStart) {
		step = StepLoop;
	}
	else if (step == StepLoop) {
		// 애니메이션 외부에서 NextStep() 호출
	}
	else if (step == StepEnd) {
		ChangeTo(BasicIdle);
	}
}



bool FSM::Hit() {
	if (step == StepHit) {
		ChangeTo(BasicHit);
		return true;
	}

	return false;
}

bool FSM::Guard() {
	if (step == StepGuard) {
		ChangeTo(BasicGuard);
		return true;
	}
	return false;
}

