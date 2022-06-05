#pragma once

#include "dx12Common/d3dApp.h"
#include "dx12Common/d3dUtil.h"
#include "dx12Common/MathHelper.h"
#include "dx12Common/UploadBuffer.h"

class Tex_Shape_Loader : public D3DApp {
public:
    Tex_Shape_Loader();
    ~Tex_Shape_Loader();
private:

    void BuildDescriptorHeaps();
    void BuildConstantBufferViews();
    void BuildRootSignature();
    void BuildShadersAndInputLayout();
    void BuildMaryGeometry();
};