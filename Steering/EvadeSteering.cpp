#include "EvadeSteering.h"

#include "DebugNew.h"
#include "FleeSteering.h"

CEvadeSteering::CEvadeSteering (
    SEntity* const  owner,
    const float     secondsAhead,
    const float     radius,
    const float     limitX,
    const float     limitY,
    const CVector2D referencePoint)
    : ISteering    { owner }
    , mSecondsAhead{ secondsAhead }
    , mRadius      {radius}
{
    mLimitX         = limitX;
    mLimitY         = limitY;
    mReferencePoint = referencePoint;
}

CEvadeSteering::~CEvadeSteering (void)
{
}

void CEvadeSteering::SetSecondsAhead (const float secondsAhead)
{
    mSecondsAhead = secondsAhead;
}

void CEvadeSteering::SetRadius (const float radius)
{
    mRadius = radius;
}

float CEvadeSteering::GetSecondsAhead (void) const
{
    return mSecondsAhead;
}

float CEvadeSteering::GetRadius (void) const
{
    return mRadius;
}

ISteering* CEvadeSteering::GetSteering (void)
{
    SEntity* aheadTarget{ new SEntity{} };

    aheadTarget->position =
        mTarget->position
        + (mTarget->linearVelocity * mSecondsAhead);

    // Move away from the ahead target.
    CFleeSteering* flee{ new CFleeSteering{ mOwner, mRadius } };
    flee->SetTarget (aheadTarget);
    flee->GetSteering ();
    mWantedLinearVelocity = flee->GetWantedLinearVelocity ();
    mLinearAcceleration   = flee->GetLinearAcceleration ();

    delete flee;
    delete aheadTarget;

    // Check out of limits.
    BackIfOutOfLimits (mOwner->position);

    return this;
}

void CEvadeSteering::DrawDebug (void) const
{
#pragma warning(push)
#pragma warning(disable: 4244)
    CVector2D debugWLV  { mOwner->position + mWantedLinearVelocity };
    CVector2D debugLA   { mOwner->position + mLinearAcceleration };
    CVector2D prediction{
        mTarget->position
        + (mTarget->linearVelocity * mSecondsAhead) };

    DrawLineEx         (
        Vector2{ mOwner->position.mX,  mOwner->position.mY }
        , Vector2{ debugWLV.mX, debugWLV.mY }
        , 3.f
        , BLUE);
    DrawLineEx         (
        Vector2{ mOwner->position.mX,  mOwner->position.mY }
        , Vector2{ debugLA.mX, debugLA.mY }
        , 3.f
        , RED);
    DrawCircleLines    (
        mOwner->position.mX
        , mOwner->position.mY
        , mRadius
        , DARKGRAY);
    DrawLine           (
        mOwner->position.mX
        , mOwner->position.mY
        , prediction.mX
        , prediction.mY
        , BURGUNDY);
    DrawCircle         (
        prediction.mX
        , prediction.mY
        , 5.f
        , BURGUNDY);
    DrawText           (
        "PREDICTION"
        , prediction.mX - 45.f
        , prediction.mY + 5.f
        , 10
        , BURGUNDY);
    DrawRectangleLines (
        mReferencePoint.mX - mLimitX
        , mReferencePoint.mY - mLimitY
        , mLimitX * 2
        , mLimitY * 2
        , DARKGRAY);
    DrawText           (
        "LIMIT X"
        , mReferencePoint.mX - mLimitX - 45.f
        , mReferencePoint.mY
        , 10
        , DARKGRAY);
    DrawText           (
        "LIMIT X"
        , mReferencePoint.mX + mLimitX + 5.f
        , mReferencePoint.mY
        , 10
        , DARKGRAY);
    DrawText           (
        "LIMIT Y"
        , mReferencePoint.mX - 35.f
        , mReferencePoint.mY - mLimitY - 15.f
        , 10
        , DARKGRAY);
    DrawText           (
        "LIMIT Y"
        , mReferencePoint.mX - 35.f
        , mReferencePoint.mY + mLimitY + 5.f
        , 10
        , DARKGRAY);
#pragma warning(pop)
}