target datalayout = "e-p:64:64-p1:64:64-p2:32:32-p3:32:32-p4:64:64-p5:32:32-p6:32:32-p7:160:256:256:32-p8:128:128-p9:192:256:256:32-i64:64-v16:16-v24:32-v32:32-v48:64-v96:128-v192:256-v256:256-v512:512-v1024:1024-v2048:2048-n32:64-S32-A5-G1-ni:7:8:9"
target triple = "amdgcn-amd-amdhsa"

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare <1 x float> @llvm.maxnum.v1f32(<1 x float>, <1 x float>) #0

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.maxnum.f32(float, float) #0

define void @foo.bb374(<1 x float> %i466, ptr %out) {
newFuncRoot:
  %i497 = tail call <1 x float> @llvm.maxnum.v1f32(<1 x float> %i466, <1 x float> zeroinitializer)
  %i503 = extractelement <1 x float> %i497, i64 0
  %i507 = tail call float @llvm.maxnum.f32(float %i503, float 0.000000e+00)
  store float %i507, ptr %out
  ret void
}

attributes #0 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
