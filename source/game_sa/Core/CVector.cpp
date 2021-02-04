/*
Plugin-SDK (Grand Theft Auto San Andreas) source file
Authors: GTA Community. See more here
https://github.com/DK22Pac/plugin-sdk
Do not delete this comment block. Respect others' work!
*/

#include "StdInc.h"

void CVector::InjectHooks()
{
    ReversibleHooks::Install("CVector", "Magnitude", 0x4082C0, &CVector::Magnitude);
    ReversibleHooks::Install("CVector", "Magnitude2D", 0x406D50, &CVector::Magnitude2D);
    ReversibleHooks::Install("CVector", "Normalise", 0x59C910, &CVector::Normalise);
    ReversibleHooks::Install("CVector", "NormaliseAndMag", 0x59C970, &CVector::NormaliseAndMag);
    ReversibleHooks::Install("CVector", "Cross", 0x70F890, &CVector::Cross);
    ReversibleHooks::Install("CVector", "Sum", 0x40FDD0, &CVector::Sum);
    ReversibleHooks::Install("CVector", "Difference", 0x40FE00, &CVector::Difference);
}


CVector::CVector()
{
    x = 0.0f; y = 0.0f; z = 0.0f;
}

CVector::CVector(float X, float Y, float Z)
{
    x = X;
    y = Y;
    z = Z;
}


// Returns length of vector
float CVector::Magnitude()
{
    return sqrt(x * x + y * y + z * z);
}

float CVector::Magnitude2D()
{
    return sqrt(x * x + y * y);
}

// Normalises a vector
void CVector::Normalise()
{
    NormaliseAndMag();
}

// Normalises a vector and returns length
float CVector::NormaliseAndMag()
{
    const auto fDot = x * x + y * y + z * z;
    if (fDot <= 0.0F)
    {
        x = 1.0F;
        return 1.0F;
    }

    const auto fRecip = 1.0F / sqrt(fDot);
    x *= fRecip;
    y *= fRecip;
    z *= fRecip;

    return 1.0F / fRecip;
}

// Performs cross calculation
void CVector::Cross(const CVector& left, const CVector &right)
{
    x = left.y * right.z - left.z * right.y;
    y = left.z * right.x - left.x * right.z;
    z = left.x * right.y - left.y * right.x;
}

// Adds left + right and stores result
void CVector::Sum(const CVector& left, const CVector &right)
{
    x = left.x + right.x;
    y = left.y + right.y;
    z = left.z + right.z;
}

// Subtracts left - right and stores result
void CVector::Difference(const CVector& left, const CVector &right)
{
    x = left.x - right.x;
    y = left.y - right.y;
    z = left.z - right.z;
}

// Assigns value from other vector
void CVector::operator= (const CVector& right)
{
    x = right.x;
    y = right.y;
    z = right.z;
}

// Adds value from the second vector.
void CVector::operator+=(const CVector& right)
{
    x += right.x;
    y += right.y;
    z += right.z;
}

void CVector::operator-=(const CVector& right)
{
    x -= right.x;
    y -= right.y;
    z -= right.z;
}

void CVector::operator*=(const CVector& right)
{
    x *= right.x;
    y *= right.y;
    z *= right.z;
}

// Multiplies vector by a floating point value
void CVector::operator *= (float multiplier)
{
    x *= multiplier;
    y *= multiplier;
    z *= multiplier;
}

// Divides vector by a floating point value
void CVector::operator /= (float divisor)
{
    x /= divisor;
    y /= divisor;
    z /= divisor;
}

void CVector::FromMultiply(CMatrix  const& matrix, CVector const& vector)
{
    ((void(__thiscall *)(CVector *, CMatrix  const&, CVector const&))0x59C670)(this, matrix, vector);
}

void CVector::FromMultiply3x3(CMatrix  const& matrix, CVector const& vector)
{
    ((void(__thiscall *)(CVector *, CMatrix  const&, CVector const&))0x59C6D0)(this, matrix, vector);
}

CVector* CrossProduct(CVector* out, CVector* a, CVector* b)
{
    return plugin::CallAndReturn <CVector*, 0x59C730, CVector*, CVector*, CVector*> (out, a, b);
}

CVector CrossProduct(const CVector& a, const CVector& b)
{
    CVector result;
    plugin::Call<0x59C730, CVector*, const CVector&, const CVector&>(&result, a, b);
    return result;
}

float DotProduct(CVector* v1, CVector* v2)
{
    return plugin::CallAndReturn <float, 0x40FDB0, CVector*, CVector*>(v1, v2);
    //return v1->z * v2->z + v1->y * v2->y + v1->x * v2->x;
}

float DotProduct(const CVector& v1, const CVector& v2)
{
    return v1.z * v2.z + v1.y * v2.y + v1.x * v2.x;
}
