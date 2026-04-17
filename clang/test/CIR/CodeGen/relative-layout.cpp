// RUN: %clang_cc1 -triple x86_64-unknown-linux-gnu -std=c++20 -fclangir -fexperimental-relative-c++-abi-vtables -emit-cir %s -o - | FileCheck %s --check-prefix=CIR
// RUN: %clang_cc1 -triple x86_64-unknown-linux-gnu -std=c++20 -fclangir -fexperimental-relative-c++-abi-vtables -emit-llvm %s -o - | FileCheck %s --check-prefix=LLVM

struct V { int x; };
struct A : virtual V {};
struct B : A {};

// CIR-LABEL: @_Z4testP1B
// LLVM-LABEL: @_Z4testP1B(

int test(B *p) {
// CIR: %[[P:.*]] = cir.load
// CIR: %[[VPTR_ADDR:.*]] = cir.vtable.get_vptr %[[P]]
// CIR: %[[VPTR:.*]] = cir.load align(8) %[[VPTR_ADDR]]
// CIR: %[[VTABLE:.*]] = cir.cast bitcast %[[VPTR]] : !cir.vptr -> !cir.ptr<!u8i>
// CIR: %[[SLOT_DELTA:.*]] = cir.const #cir.int<-12> : !s64i
// CIR: %[[SLOT_ADDR_I8:.*]] = cir.ptr_stride %[[VTABLE]], %[[SLOT_DELTA]]
// CIR: %[[SLOT_ADDR:.*]] = cir.cast bitcast %[[SLOT_ADDR_I8]] : !cir.ptr<!u8i> -> !cir.ptr<!s32i>
// CIR: %[[VBASE_OFF:.*]] = cir.load align(4) %[[SLOT_ADDR]] : !cir.ptr<!s32i>, !s32i
// CIR: %[[OBJ_I8:.*]] = cir.cast bitcast %[[P]] : !cir.ptr<!rec_B> -> !cir.ptr<!u8i>
// CIR: %[[VBASE_I8:.*]] = cir.ptr_stride %[[OBJ_I8]], %[[VBASE_OFF]]
// CIR: cir.get_member %{{.*}}[0] {name = "x"} : !cir.ptr<!rec_V> -> !cir.ptr<!s32i>
// CIR: cir.return %{{.*}} : !s32i

// LLVM-SAME: ptr noundef [[P:%.*]])
// LLVM: store ptr [[P]], ptr %{{.*}}, align 8
// LLVM: [[OBJ:%.*]] = load ptr, ptr %{{.*}}, align 8
// LLVM: [[VTABLE:%.*]] = load ptr, ptr [[OBJ]], align 8
// LLVM: [[OFF_PTR:%.*]] = getelementptr i8, ptr [[VTABLE]], i64 -12
// LLVM: [[OFF32:%.*]] = load i32, ptr [[OFF_PTR]], align 4
// LLVM: [[OFF64:%.*]] = sext i32 [[OFF32]] to i64
// LLVM: [[VBASE:%.*]] = getelementptr i8, ptr [[OBJ]], i64 [[OFF64]]
// LLVM: [[X_ADDR:%.*]] = getelementptr %struct.V, ptr %{{.*}}, i32 0, i32 0
// LLVM: [[X:%.*]] = load i32, ptr [[X_ADDR]], align 4
// LLVM: ret i32 %{{.*}}

  V *vp = p;
  return vp->x;
}