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
    ReversibleHooks::Install("CMatrix", "Attach", 0x59BD10, &CMatrix::Attach, true);
    ReversibleHooks::Install("CMatrix", "Detach", 0x59ACF0, &CMatrix::Detach, true);
    ReversibleHooks::Install("CMatrix", "CopyOnlyMatrix", 0x59ADD0, &CMatrix::CopyOnlyMatrix, true);
    ReversibleHooks::Install("CMatrix", "Update", 0x59BB60, &CMatrix::Update, true);
    ReversibleHooks::Install("CMatrix", "UpdateMatrix", 0x59AD20, &CMatrix::UpdateMatrix, true);
    ReversibleHooks::Install("CMatrix", "UpdateRW", 0x59BBB0, &CMatrix::UpdateRW, true);
    ReversibleHooks::Install("CMatrix", "UpdateRwMatrix", 0x59AD70, &CMatrix::UpdateRwMatrix, true);
    ReversibleHooks::Install("CMatrix", "SetUnity", 0x59AE70, &CMatrix::SetUnity, true);
    ReversibleHooks::Install("CMatrix", "ResetOrientation", 0x59AEA0, &CMatrix::ResetOrientation, true);
    ReversibleHooks::Install("CMatrix", "SetScale_f", 0x59AED0, (void(CMatrix::*)(float))(&CMatrix::SetScale), true);
    ReversibleHooks::Install("CMatrix", "SetScale_fff", 0x59AF00, (void(CMatrix::*)(float, float, float))(&CMatrix::SetScale), true);
    ReversibleHooks::Install("CMatrix", "SetTranslateOnly", 0x59AF80, &CMatrix::SetTranslateOnly, true);
    ReversibleHooks::Install("CMatrix", "SetTranslate", 0x59AF40, &CMatrix::SetTranslate, true);
    ReversibleHooks::Install("CMatrix", "SetRotateXOnly", 0x59AFA0, &CMatrix::SetRotateXOnly, true);
    ReversibleHooks::Install("CMatrix", "SetRotateYOnly", 0x59AFE0, &CMatrix::SetRotateYOnly, true);
    ReversibleHooks::Install("CMatrix", "SetRotateZOnly", 0x59B020, &CMatrix::SetRotateZOnly, true);
    ReversibleHooks::Install("CMatrix", "SetRotateX", 0x59B060, &CMatrix::SetRotateX, true);
    ReversibleHooks::Install("CMatrix", "SetRotateY", 0x59B0A0, &CMatrix::SetRotateY, true);
    ReversibleHooks::Install("CMatrix", "SetRotateZ", 0x59B0E0, &CMatrix::SetRotateZ, true);
    ReversibleHooks::Install("CMatrix", "SetRotate_fff", 0x59B120, (void(CMatrix::*)(float, float, float))(&CMatrix::SetRotate), true);
    ReversibleHooks::Install("CMatrix", "RotateX", 0x59B1E0, &CMatrix::RotateX, true);
    ReversibleHooks::Install("CMatrix", "RotateY", 0x59B2C0, &CMatrix::RotateY, true);
    ReversibleHooks::Install("CMatrix", "RotateZ", 0x59B390, &CMatrix::RotateZ, true);
    ReversibleHooks::Install("CMatrix", "Rotate", 0x59B460, &CMatrix::Rotate, true);
    ReversibleHooks::Install("CMatrix", "Reorthogonalise", 0x59B6A0, &CMatrix::Reorthogonalise, true);
    ReversibleHooks::Install("CMatrix", "CopyToRwMatrix", 0x59B8B0, &CMatrix::CopyToRwMatrix, true);
    ReversibleHooks::Install("CMatrix", "SetRotate_quat", 0x59BBF0, (void(CMatrix::*)(CQuaternion&))(&CMatrix::SetRotate), true);
    ReversibleHooks::Install("CMatrix", "Scale", 0x459350, &CMatrix::Scale, true);
    ReversibleHooks::Install("CMatrix", "ForceUpVector", 0x59B7E0, &CMatrix::ForceUpVector, true);
    ReversibleHooks::Install("CMatrix", "operator=", 0x59BBC0, &CMatrix::operator=, true);
    ReversibleHooks::Install("CMatrix", "operator+=", 0x59ADF0, &CMatrix::operator+=, true);
    ReversibleHooks::Install("CMatrix", "operator*=", 0x411A80, &CMatrix::operator*=, true);
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
    memcpy(this, &matrix, sizeof(RwMatrix));
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

    RwMatrixUpdate(matrix);
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
    auto fSinX = sin(x);
    auto fCosX = cos(x);
    auto fSinY = sin(y);
    auto fCosY = cos(y);
    auto fSinZ = sin(z);
    auto fCosZ = cos(z);

    m_right.Set  ( fCosZ*fCosY-fSinZ*fSinX*fSinY,  fCosZ*fSinX*fSinY+fSinZ*fCosY,  -fSinY*fCosX);
    m_forward.Set(-fSinZ*fCosX,                    fCosZ*fCosX,                     fSinX);
    m_up.Set     ( fCosZ*fSinX+fSinZ*fSinX*fCosY,  fSinZ*fSinY-fCosZ*fSinX*fCosY,   fCosY*fCosX);
    m_pos.Set(0.0F, 0.0F, 0.0F);
}

void CMatrix::RotateX(float angle)
{
    auto rotMat = CMatrix();
    rotMat.SetRotateXOnly(angle);
    m_right =   Multiply3x3(&rotMat, &m_right);
    m_forward = Multiply3x3(&rotMat, &m_forward);
    m_up =      Multiply3x3(&rotMat, &m_up);
    m_pos =     Multiply3x3(&rotMat, &m_pos);
}

void CMatrix::RotateY(float angle)
{
    auto rotMat = CMatrix();
    rotMat.SetRotateYOnly(angle);
    m_right =   Multiply3x3(&rotMat, &m_right);
    m_forward = Multiply3x3(&rotMat, &m_forward);
    m_up =      Multiply3x3(&rotMat, &m_up);
    m_pos =     Multiply3x3(&rotMat, &m_pos);
}

void CMatrix::RotateZ(float angle)
{
    auto rotMat = CMatrix();
    rotMat.SetRotateZOnly(angle);
    m_right =   Multiply3x3(&rotMat, &m_right);
    m_forward = Multiply3x3(&rotMat, &m_forward);
    m_up =      Multiply3x3(&rotMat, &m_up);
    m_pos =     Multiply3x3(&rotMat, &m_pos);
}

// rotate on 3 axes
void CMatrix::Rotate(CVector rotation)
{
    auto rotMat = CMatrix();
    rotMat.SetRotate(rotation.x, rotation.y, rotation.z);
    m_right =   Multiply3x3(&rotMat, &m_right);
    m_forward = Multiply3x3(&rotMat, &m_forward);
    m_up =      Multiply3x3(&rotMat, &m_up);
    m_pos =     Multiply3x3(&rotMat, &m_pos);
}

void CMatrix::Reorthogonalise()
{
    auto vecCross = CrossProduct(m_right, m_forward);
    vecCross.Normalise();

    auto vecCross2 = CrossProduct(m_forward, vecCross);
    vecCross2.Normalise();

    auto vecCross3 = CrossProduct(vecCross, vecCross2);

    m_right = vecCross2;
    m_forward = vecCross3;
    m_up = vecCross;
}

// similar to UpdateRW(RwMatrixTag *)
void CMatrix::CopyToRwMatrix(RwMatrix* matrix)
{
    CMatrix::UpdateRwMatrix(matrix);
    RwMatrixUpdate(matrix);
}

void CMatrix::SetRotate(CQuaternion& quat)
{
    quat.Get(this);
}

void CMatrix::Scale(float scale) {
    m_right *= scale;
    m_forward *= scale;
    m_up *= scale;
}

void CMatrix::ForceUpVector(CVector vecUp) {
    auto vecCross = CrossProduct(m_forward, vecUp);
    auto vecCross2 = CrossProduct(vecUp, vecCross);

    m_right = vecCross;
    m_forward = vecCross2;
    m_up = vecUp;
}

void CMatrix::operator=(CMatrix const& rvalue)
{
    CMatrix::CopyOnlyMatrix(rvalue);
    CMatrix::UpdateRW();
}

void CMatrix::operator+=(CMatrix const& rvalue)
{
    m_right += rvalue.m_right;
    m_forward += rvalue.m_forward;
    m_up += rvalue.m_up;
    m_pos += rvalue.m_pos;
}

void CMatrix::operator*=(CMatrix const& rvalue)
{
    *this = (*this * rvalue);
}

CMatrix operator*(CMatrix const& a, CMatrix const& b) {
    CMatrix result;
    plugin::Call<0x59BE30, CMatrix*, CMatrix const&, CMatrix const&>(&result, a, b);
    return result;
}

CVector operator*(CMatrix const& a, CVector const& b) {
    CVector result;
    plugin::Call<0x59C890, CVector*, CMatrix const&, CVector const&>(&result, a, b);
    return result;
}

CMatrix operator+(CMatrix const& a, CMatrix const& b) {
    CMatrix result;
    plugin::Call<0x59BFA0, CMatrix*, CMatrix const&, CMatrix const&>(&result, a, b);
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
