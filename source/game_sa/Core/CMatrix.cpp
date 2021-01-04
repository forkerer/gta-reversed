/*
    Plugin-SDK (Grand Theft Auto San Andreas) source file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/

#include "StdInc.h"

int32_t& numMatrices = *(int32_t*)0xB74238;
CMatrix& gDummyMatrix = *(CMatrix*)0xB74240;

void CMatrix::InjectHooks()
{
    ReversibleHooks::Install("CMatrix", "Attach", 0x59BD10, &CMatrix::Attach);
    ReversibleHooks::Install("CMatrix", "Detach", 0x59ACF0, &CMatrix::Detach);
    ReversibleHooks::Install("CMatrix", "CopyOnlyMatrix", 0x59ADD0, &CMatrix::CopyOnlyMatrix);
    ReversibleHooks::Install("CMatrix", "Update", 0x59BB60, &CMatrix::Update);
    ReversibleHooks::Install("CMatrix", "UpdateMatrix", 0x59AD20, &CMatrix::UpdateMatrix);
    ReversibleHooks::Install("CMatrix", "UpdateRW", 0x59BBB0, &CMatrix::UpdateRW);
    ReversibleHooks::Install("CMatrix", "UpdateRwMatrix", 0x59AD70, &CMatrix::UpdateRwMatrix);
    ReversibleHooks::Install("CMatrix", "SetUnity", 0x59AE70, &CMatrix::SetUnity);
    ReversibleHooks::Install("CMatrix", "ResetOrientation", 0x59AEA0, &CMatrix::ResetOrientation);
    ReversibleHooks::Install("CMatrix", "SetScale", 0x59AED0, (void(CMatrix::*)(float))(&CMatrix::SetScale));
    ReversibleHooks::Install("CMatrix", "SetScale", 0x59AF00, (void(CMatrix::*)(float, float, float))(&CMatrix::SetScale));
    ReversibleHooks::Install("CMatrix", "SetTranslateOnly", 0x59AF80, &CMatrix::SetTranslateOnly);
    ReversibleHooks::Install("CMatrix", "SetTranslate", 0x59AF40, &CMatrix::SetTranslate);
    ReversibleHooks::Install("CMatrix", "SetRotateXOnly", 0x59AFA0, &CMatrix::SetRotateXOnly);
    ReversibleHooks::Install("CMatrix", "SetRotateYOnly", 0x59AFE0, &CMatrix::SetRotateYOnly);
    ReversibleHooks::Install("CMatrix", "SetRotateZOnly", 0x59B020, &CMatrix::SetRotateZOnly);
    ReversibleHooks::Install("CMatrix", "SetRotateX", 0x59B060, &CMatrix::SetRotateX);
    ReversibleHooks::Install("CMatrix", "SetRotateY", 0x59B0A0, &CMatrix::SetRotateY);
    ReversibleHooks::Install("CMatrix", "SetRotateZ", 0x59B0E0, &CMatrix::SetRotateZ);
}

CMatrix::CMatrix(CMatrix const& matrix)
{
    m_pAttachMatrix = nullptr;
    m_bOwnsAttachedMatrix = false;
    CMatrix::CopyOnlyMatrix(matrix);
}

// like previous + attach
CMatrix::CMatrix(RwMatrix* matrix, bool temporary)
{
    m_pAttachMatrix = nullptr;
    CMatrix::Attach(matrix, temporary);
}

// destructor detaches matrix if attached 
CMatrix::~CMatrix()
{
    CMatrix::Detach();
}

void CMatrix::Attach(RwMatrix* matrix, bool bOwnsMatrix)
{
    CMatrix::Detach();

    m_pAttachMatrix = matrix;
    m_bOwnsAttachedMatrix = bOwnsMatrix;
    CMatrix::Update();
}

void CMatrix::Detach()
{
    if (m_bOwnsAttachedMatrix && m_pAttachMatrix)
        RwMatrixDestroy(m_pAttachMatrix);

    m_pAttachMatrix = nullptr;
}

// copy base RwMatrix to another matrix
void CMatrix::CopyOnlyMatrix(CMatrix const& matrix)
{
    memcpy(this, &matrix, 0x40);
}

// update RwMatrix with attaching matrix. This doesn't check if attaching matrix is present, so use it only if you know it is present.
// Using UpdateRW() is more safe since it perform this check.
void CMatrix::Update()
{
    CMatrix::UpdateMatrix(m_pAttachMatrix);
}

// update RwMatrix with attaching matrix.
void CMatrix::UpdateRW()
{
    if (!m_pAttachMatrix)
        return;

    CMatrix::UpdateRwMatrix(m_pAttachMatrix);
}

// update RwMatrix with this matrix
void CMatrix::UpdateRwMatrix(RwMatrix* matrix)
{
    *RwMatrixGetRight(matrix) = m_right;
    *RwMatrixGetUp(matrix) = m_forward;
    *RwMatrixGetAt(matrix) = m_up;
    *RwMatrixGetPos(matrix) = m_pos;
}

void CMatrix::UpdateMatrix(RwMatrixTag* rwMatrix)
{
    m_right = *RwMatrixGetRight(rwMatrix);
    m_forward = *RwMatrixGetUp(rwMatrix);
    m_up = *RwMatrixGetAt(rwMatrix);
    m_pos = *RwMatrixGetPos(rwMatrix);
}

void CMatrix::SetUnity()
{
    CMatrix::ResetOrientation();
    m_pos.Set(0.0F, 0.0F, 0.0F);
}

void CMatrix::ResetOrientation()
{
    m_right.Set  (1.0F, 0.0F, 0.0F);
    m_forward.Set(0.0F, 1.0F, 0.0F);
    m_up.Set     (0.0F, 0.0F, 1.0F);
}

void CMatrix::SetScale(float scale)
{
    m_right.Set  (scale, 1.0F,  1.0F);
    m_forward.Set(0.0F,  scale, 0.0F);
    m_up.Set     (0.0F,  0.0F,  scale);
    m_pos.Set    (0.0F,  0.0F,  0.0F);
}

// scale on three axes
void CMatrix::SetScale(float x, float y, float z)
{
    m_right.Set  (x,     1.0F,  1.0F);
    m_forward.Set(0.0F,  y,     0.0F);
    m_up.Set     (0.0F,  0.0F,  z   );
    m_pos.Set    (0.0F,  0.0F,  0.0F);
}

void CMatrix::SetTranslateOnly(CVector translation)
{
    m_pos = translation;
}

// like previous + reset orientation
void CMatrix::SetTranslate(CVector translation)
{
    CMatrix::ResetOrientation();
    CMatrix::SetTranslateOnly(translation);
}

void CMatrix::SetRotateXOnly(float angle)
{
    auto fSin = sin(angle);
    auto fCos = cos(angle);

    m_right.Set  (1.0F,  0.0F,  0.0F);
    m_forward.Set(0.0F,  fCos,  fSin);
    m_up.Set     (0.0F, -fSin,  fCos);
}

void CMatrix::SetRotateYOnly(float angle)
{
    auto fSin = sin(angle);
    auto fCos = cos(angle);

    m_right.Set  (fCos,  0.0F, -fSin);
    m_forward.Set(0.0F,  1.0F,  0.0F);
    m_up.Set     (fSin,  0.0F,  fCos);
}

void CMatrix::SetRotateZOnly(float angle)
{
    auto fSin = sin(angle);
    auto fCos = cos(angle);

    m_right.Set  ( fCos, fSin, 0.0F);
    m_forward.Set(-fSin, fCos, 0.0F);
    m_up.Set     ( 0.0F, 0.0F, 1.0F);
}

void CMatrix::SetRotateX(float angle)
{
    CMatrix::SetRotateXOnly(angle);
    m_pos.Set(0.0F, 0.0F, 0.0F);
}

void CMatrix::SetRotateY(float angle)
{
    CMatrix::SetRotateYOnly(angle);
    m_pos.Set(0.0F, 0.0F, 0.0F);
}

void CMatrix::SetRotateZ(float angle)
{
    CMatrix::SetRotateZOnly(angle);
    m_pos.Set(0.0F, 0.0F, 0.0F);
}

// set rotate on 3 axes
void CMatrix::SetRotate(float x, float y, float z)
{
    ((void(__thiscall*)(CMatrix*, float, float, float))0x59B120)(this, x, y, z);
}

void CMatrix::RotateX(float angle)
{
    ((void(__thiscall*)(CMatrix*, float))0x59B1E0)(this, angle);
}

void CMatrix::RotateY(float angle)
{
    ((void(__thiscall*)(CMatrix*, float))0x59B2C0)(this, angle);
}

void CMatrix::RotateZ(float angle)
{
    ((void(__thiscall*)(CMatrix*, float))0x59B390)(this, angle);
}

// rotate on 3 axes
void CMatrix::Rotate(CVector rotation)
{
    ((void(__thiscall*)(CMatrix*, CVector))0x59B460)(this, rotation);
}

void CMatrix::Reorthogonalise()
{
    ((void(__thiscall*)(CMatrix*))0x59B6A0)(this);
}

// similar to UpdateRW(RwMatrixTag *)
void CMatrix::CopyToRwMatrix(RwMatrix* matrix)
{
    ((void(__thiscall*)(CMatrix*, RwMatrix*))0x59B8B0)(this, matrix);
}

void CMatrix::SetRotate(CQuaternion  const& quat)
{
    ((void(__thiscall*)(CMatrix*, CQuaternion  const&))0x59BBF0)(this, quat);
}

void CMatrix::Scale(float scale) {
    plugin::CallMethod<0x459350, CMatrix*, float>(this, scale);
}

void CMatrix::Scale(float x, float y, float z) {
    plugin::CallMethod<0x459350, CMatrix*, float, float, float>(this, x, y, z);
}

void CMatrix::ForceUpVector(float x, float y, float z) {
    plugin::CallMethod<0x59B7E0, CMatrix*, float, float, float>(this, x, y, z);
}

void CMatrix::operator=(CMatrix const& rvalue)
{
    ((void(__thiscall*)(CMatrix*, CMatrix const&))0x59BBC0)(this, rvalue);
}

void CMatrix::operator+=(CMatrix const& rvalue)
{
    ((void(__thiscall*)(CMatrix*, CMatrix const&))0x59ADF0)(this, rvalue);
}

void CMatrix::operator*=(CMatrix const& rvalue)
{
    ((void(__thiscall*)(CMatrix*, CMatrix const&))0x411A80)(this, rvalue);
}

CMatrix operator*(CMatrix const& a, CMatrix const& b) {
    CMatrix result;
    ((void(__cdecl*)(CMatrix*, CMatrix const&, CMatrix const&))0x59BE30)(&result, a, b);
    return result;
}

CVector operator*(CMatrix const& a, CVector const& b) {
    CVector result;
    ((void(__cdecl*)(CVector*, CMatrix const&, CVector const&))0x59C890)(&result, a, b);
    return result;
}

CMatrix operator+(CMatrix const& a, CMatrix const& b) {
    CMatrix result;
    ((void(__cdecl*)(CMatrix*, CMatrix const&, CMatrix const&))0x59BFA0)(&result, a, b);
    return result;
}

CMatrix* Invert(CMatrix* a1, CMatrix* out)
{
    return plugin::CallAndReturn<CMatrix*, 0x59B920, CMatrix*, CMatrix*>(a1, out);
}

CMatrix* InvertMatrix(CMatrix* out, CMatrix* a2)
{
    return plugin::CallAndReturn<CMatrix*, 0x59BDD0, CMatrix*, CMatrix*>(out, a2);
}
