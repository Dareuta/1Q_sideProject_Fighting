//======================================================================
// FSM.hpp
//======================================================================
#pragma once

enum CharacterStep {
	StepStart = 0,
	StepLoop,
	StepEnd,
	StepHit,
	StepGuard
};

enum BasicState {
	BasicIdle = 0,
	BasicJump,
	BasicMove,
	BasicCrouch,
	BasicAttack,
	BasicHit,
	BasicJumpAttack,
	BasicGuard,
	BasicBackDash
};

class FSM {
public:
	FSM() : basicState(BasicIdle), nextState(BasicIdle), step(StepStart) {}
	~FSM() = default;

	void Update();

	BasicState GetState() const {
		return basicState;
	}

	void ChangeState(BasicState newState) {
		if (basicState != newState) { //현재 상태와 다를 경우에만 작동
			nextState = newState; //다음 상태로 입력 > 예약 걸어두는 느낌
			step = StepEnd; // 현재 상태를 종료함(다르니까)
		}
	}

	void ChangeTo(BasicState newState) { //강제 변경
		basicState = newState;
		nextState = newState;
		step = StepStart;
	}

	void NextStep(bool flag) {
		if (flag) {
			switch (step) {
			case StepStart:
				step = StepLoop;
				break;
			case StepLoop:
				step = StepEnd;
				break;
			case StepEnd:
				ChangeTo(nextState);
				break;
			default:
				break;
			}
		}
	}

	CharacterStep GetStep() const {
		return step;
	}
	void SetStep(CharacterStep newStep) {
		step = newStep;
	}


	void SetStepHit() { //히트박스 검사 후 호출되는 부분
		step = StepHit;
	}

	void SetStepGuard() { //히트박스 검사 후 호출되는 부분
		step = StepGuard;
	}

	
private:
	void HandleIdle();
	void HandleMove();
	void HandleJump();
	void HandleCrouch();
	void HandleAttack();
	void HandleJumpAttack();
	void HandleHit();
	void HandleGuard();
	void HandleBackDash();
	bool Hit(); // 히트 검증 - 스탭
	bool Guard(); // 히트 검증 - 스탭

private:
	BasicState basicState;
	BasicState nextState;
	CharacterStep step;
};
