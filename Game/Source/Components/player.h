#pragma once
#include "engineMath.h"
#include "Components/character.h"


class Player : public Character
{
public:
    Player(SkinnedObj *pObj, class Game* game);

    void Update(float deltaTime) override;

protected:
    enum State
    {
        STATE_UNKNOWN,
        STATE_IDLE,
        STATE_FALL,
        STATE_WALK,
        STATE_JUMP,
        STATE_LAND,
    };

    bool CheckGround(float* groundHeight);
    void SetState(State newState);
    void ChangeState();
    void UpdateState(float deltaTime);

    State mState;
	float mMoveSpeed;
	float mHeading;
    Vector3 mPos;
    Vector3 mVel;
    float mPrevAnimTime;
	class Game* mGame;
};