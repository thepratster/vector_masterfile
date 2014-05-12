; ModuleID = 'prog.opt.o'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64"
target triple = "x86_64-unknown-linux-gnu"

@.str = private constant [8 x i8] c"in_data\00"
@.str1 = private constant [9 x i8] c"out_data\00"

define float @fpmul32f(float %x, float %y) nounwind readnone {
  %1 = fmul float %x, %y
  ret float %1
}

define float @fpadd32f(float %x, float %y) nounwind readnone {
  %1 = fadd float %x, %y
  ret float %1
}

define float @fpsub32f(float %x, float %y) nounwind readnone {
  %1 = fsub float %x, %y
  ret float %1
}

define i32 @fpadd32fi(i32 %x, i32 %y) nounwind readnone {
  %1 = add i32 %y, %x
  ret i32 %1
}

define i32 @fpsub32fi(i32 %x, i32 %y) nounwind readnone {
  %1 = sub i32 %x, %y
  ret i32 %1
}

define i32 @udiv32(i32 %dividend, i32 %divisor) nounwind readnone {
; <label>:0
  switch i32 %divisor, label %2 [
    i32 0, label %.loopexit
    i32 1, label %1
  ]

; <label>:1                                       ; preds = %0
  ret i32 %dividend

; <label>:2                                       ; preds = %0
  %3 = icmp ugt i32 %divisor, %dividend
  %4 = icmp ult i32 %dividend, %divisor
  %or.cond = or i1 %3, %4
  br i1 %or.cond, label %.loopexit, label %bb.nph7.preheader

bb.nph7.preheader:                                ; preds = %2
  br label %bb.nph7

bb.nph7:                                          ; preds = %._crit_edge, %bb.nph7.preheader
  %.016 = phi i32 [ %11, %._crit_edge ], [ %dividend, %bb.nph7.preheader ]
  %quotient.05 = phi i32 [ %10, %._crit_edge ], [ 0, %bb.nph7.preheader ]
  %5 = lshr i32 %.016, 1
  %6 = icmp ugt i32 %5, %divisor
  br i1 %6, label %bb.nph.preheader, label %._crit_edge

bb.nph.preheader:                                 ; preds = %bb.nph7
  br label %bb.nph

bb.nph:                                           ; preds = %bb.nph, %bb.nph.preheader
  %shifted_divisor.03 = phi i32 [ %7, %bb.nph ], [ %divisor, %bb.nph.preheader ]
  %curr_quotient.02 = phi i32 [ %8, %bb.nph ], [ 1, %bb.nph.preheader ]
  %7 = shl i32 %shifted_divisor.03, 1
  %8 = shl i32 %curr_quotient.02, 1
  %9 = icmp ult i32 %7, %5
  br i1 %9, label %bb.nph, label %._crit_edge.loopexit

._crit_edge.loopexit:                             ; preds = %bb.nph
  %.lcssa1 = phi i32 [ %8, %bb.nph ]
  %.lcssa = phi i32 [ %7, %bb.nph ]
  br label %._crit_edge

._crit_edge:                                      ; preds = %._crit_edge.loopexit, %bb.nph7
  %shifted_divisor.0.lcssa = phi i32 [ %divisor, %bb.nph7 ], [ %.lcssa, %._crit_edge.loopexit ]
  %curr_quotient.0.lcssa = phi i32 [ 1, %bb.nph7 ], [ %.lcssa1, %._crit_edge.loopexit ]
  %10 = add i32 %curr_quotient.0.lcssa, %quotient.05
  %11 = sub i32 %.016, %shifted_divisor.0.lcssa
  %12 = icmp ult i32 %11, %divisor
  br i1 %12, label %.loopexit.loopexit, label %bb.nph7

.loopexit.loopexit:                               ; preds = %._crit_edge
  %.lcssa2 = phi i32 [ %10, %._crit_edge ]
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %2, %0
  %.0 = phi i32 [ -1, %0 ], [ 0, %2 ], [ %.lcssa2, %.loopexit.loopexit ]
  ret i32 %.0
}

define float @fdiv32(float %a, float %b) nounwind readnone {
  %tmp10 = bitcast float %a to i32
  %tmp6 = bitcast float %b to i32
  %1 = fcmp oeq float %a, 0.000000e+00
  br i1 %1, label %37, label %2

; <label>:2                                       ; preds = %0
  %3 = lshr i32 %tmp10, 23
  %4 = and i32 %3, 255
  %5 = lshr i32 %tmp6, 23
  %6 = and i32 %5, 255
  %7 = shl i32 %tmp10, 7
  %8 = and i32 %7, 1073741696
  %9 = or i32 %8, 1073741824
  %10 = lshr i32 %tmp6, 7
  %11 = and i32 %10, 65535
  %12 = or i32 %11, 65536
  %13 = xor i32 %tmp6, %tmp10
  %14 = and i32 %13, -2147483648
  %15 = sub i32 %4, %6
  switch i32 %12, label %bb.nph7.i.preheader [
    i32 0, label %.loopexit.i
    i32 1, label %udiv32.exit.preheader
  ]

bb.nph7.i.preheader:                              ; preds = %2
  br label %bb.nph7.i

bb.nph7.i:                                        ; preds = %._crit_edge.i, %bb.nph7.i.preheader
  %.016.i = phi i32 [ %22, %._crit_edge.i ], [ %9, %bb.nph7.i.preheader ]
  %quotient.05.i = phi i32 [ %21, %._crit_edge.i ], [ 0, %bb.nph7.i.preheader ]
  %16 = lshr i32 %.016.i, 1
  %17 = icmp ugt i32 %16, %12
  br i1 %17, label %bb.nph.i.preheader, label %._crit_edge.i

bb.nph.i.preheader:                               ; preds = %bb.nph7.i
  br label %bb.nph.i

bb.nph.i:                                         ; preds = %bb.nph.i, %bb.nph.i.preheader
  %shifted_divisor.03.i = phi i32 [ %18, %bb.nph.i ], [ %12, %bb.nph.i.preheader ]
  %curr_quotient.02.i = phi i32 [ %19, %bb.nph.i ], [ 1, %bb.nph.i.preheader ]
  %18 = shl i32 %shifted_divisor.03.i, 1
  %19 = shl i32 %curr_quotient.02.i, 1
  %20 = icmp ult i32 %18, %16
  br i1 %20, label %bb.nph.i, label %._crit_edge.i.loopexit

._crit_edge.i.loopexit:                           ; preds = %bb.nph.i
  %.lcssa2 = phi i32 [ %19, %bb.nph.i ]
  %.lcssa1 = phi i32 [ %18, %bb.nph.i ]
  br label %._crit_edge.i

._crit_edge.i:                                    ; preds = %._crit_edge.i.loopexit, %bb.nph7.i
  %shifted_divisor.0.lcssa.i = phi i32 [ %12, %bb.nph7.i ], [ %.lcssa1, %._crit_edge.i.loopexit ]
  %curr_quotient.0.lcssa.i = phi i32 [ 1, %bb.nph7.i ], [ %.lcssa2, %._crit_edge.i.loopexit ]
  %21 = add i32 %curr_quotient.0.lcssa.i, %quotient.05.i
  %22 = sub i32 %.016.i, %shifted_divisor.0.lcssa.i
  %23 = icmp ult i32 %22, %12
  br i1 %23, label %.loopexit.i.loopexit, label %bb.nph7.i

.loopexit.i.loopexit:                             ; preds = %._crit_edge.i
  %.lcssa3 = phi i32 [ %21, %._crit_edge.i ]
  br label %.loopexit.i

.loopexit.i:                                      ; preds = %.loopexit.i.loopexit, %2
  %.0.i = phi i32 [ -1, %2 ], [ %.lcssa3, %.loopexit.i.loopexit ]
  br label %udiv32.exit.preheader

udiv32.exit.preheader:                            ; preds = %.loopexit.i, %2
  %temp.0.ph = phi i32 [ %.0.i, %.loopexit.i ], [ %9, %2 ]
  %24 = and i32 %temp.0.ph, 8388608
  %25 = icmp eq i32 %24, 0
  %26 = icmp ne i32 %temp.0.ph, 0
  %or.cond11 = and i1 %25, %26
  br i1 %or.cond11, label %bb.nph, label %.critedge

bb.nph:                                           ; preds = %udiv32.exit.preheader
  %tmp21 = add i32 %4, -1
  br label %udiv32.exit

udiv32.exit:                                      ; preds = %udiv32.exit, %bb.nph
  %27 = phi i32 [ 0, %bb.nph ], [ %indvar.next, %udiv32.exit ]
  %temp.012 = phi i32 [ %temp.0.ph, %bb.nph ], [ %28, %udiv32.exit ]
  %28 = shl i32 %temp.012, 1
  %29 = and i32 %28, 8388608
  %30 = icmp eq i32 %29, 0
  %31 = icmp ne i32 %28, 0
  %or.cond = and i1 %30, %31
  %indvar.next = add i32 %27, 1
  br i1 %or.cond, label %udiv32.exit, label %udiv32.exit..critedge_crit_edge

udiv32.exit..critedge_crit_edge:                  ; preds = %udiv32.exit
  %.lcssa = phi i32 [ %28, %udiv32.exit ]
  %indvar.lcssa = phi i32 [ %27, %udiv32.exit ]
  %tmp25 = sub i32 %tmp21, %6
  %tmp26 = sub i32 %tmp25, %indvar.lcssa
  br label %.critedge

.critedge:                                        ; preds = %udiv32.exit..critedge_crit_edge, %udiv32.exit.preheader
  %exp.0.lcssa = phi i32 [ %tmp26, %udiv32.exit..critedge_crit_edge ], [ %15, %udiv32.exit.preheader ]
  %temp.0.lcssa = phi i32 [ %.lcssa, %udiv32.exit..critedge_crit_edge ], [ %temp.0.ph, %udiv32.exit.preheader ]
  %32 = and i32 %temp.0.lcssa, 8388607
  %33 = shl i32 %exp.0.lcssa, 23
  %34 = add i32 %33, 1140850688
  %35 = or i32 %32, %14
  %36 = or i32 %35, %34
  %tmp3 = bitcast i32 %36 to float
  ret float %tmp3

; <label>:37                                      ; preds = %0
  ret float 0.000000e+00
}

define float @rotor_flux_calc(float %id, float %flux_rotor_prev) nounwind readnone {
  %1 = fmul float %flux_rotor_prev, 0x3FEFFC1160000000
  %2 = fmul float %id, 0x3F3977C2A0000000
  %3 = fadd float %2, %1
  ret float %3
}

define float @omega_calc(float %iq, float %flux_rotor) nounwind readnone {
  %1 = fmul float %iq, 0x401F299A00000000
  %tmp10.i = bitcast float %1 to i32
  %tmp6.i = bitcast float %flux_rotor to i32
  %2 = fcmp oeq float %1, 0.000000e+00
  br i1 %2, label %fdiv32.exit, label %3

; <label>:3                                       ; preds = %0
  %4 = lshr i32 %tmp10.i, 23
  %5 = and i32 %4, 255
  %6 = lshr i32 %tmp6.i, 23
  %7 = and i32 %6, 255
  %8 = shl i32 %tmp10.i, 7
  %9 = and i32 %8, 1073741696
  %10 = or i32 %9, 1073741824
  %11 = lshr i32 %tmp6.i, 7
  %12 = and i32 %11, 65535
  %13 = or i32 %12, 65536
  %14 = xor i32 %tmp6.i, %tmp10.i
  %15 = and i32 %14, -2147483648
  %16 = sub i32 %5, %7
  switch i32 %13, label %bb.nph7.i.i.preheader [
    i32 0, label %.loopexit.i.i
    i32 1, label %udiv32.exit.preheader.i
  ]

bb.nph7.i.i.preheader:                            ; preds = %3
  br label %bb.nph7.i.i

bb.nph7.i.i:                                      ; preds = %._crit_edge.i.i, %bb.nph7.i.i.preheader
  %.016.i.i = phi i32 [ %23, %._crit_edge.i.i ], [ %10, %bb.nph7.i.i.preheader ]
  %quotient.05.i.i = phi i32 [ %22, %._crit_edge.i.i ], [ 0, %bb.nph7.i.i.preheader ]
  %17 = lshr i32 %.016.i.i, 1
  %18 = icmp ugt i32 %17, %13
  br i1 %18, label %bb.nph.i.i.preheader, label %._crit_edge.i.i

bb.nph.i.i.preheader:                             ; preds = %bb.nph7.i.i
  br label %bb.nph.i.i

bb.nph.i.i:                                       ; preds = %bb.nph.i.i, %bb.nph.i.i.preheader
  %shifted_divisor.03.i.i = phi i32 [ %19, %bb.nph.i.i ], [ %13, %bb.nph.i.i.preheader ]
  %curr_quotient.02.i.i = phi i32 [ %20, %bb.nph.i.i ], [ 1, %bb.nph.i.i.preheader ]
  %19 = shl i32 %shifted_divisor.03.i.i, 1
  %20 = shl i32 %curr_quotient.02.i.i, 1
  %21 = icmp ult i32 %19, %17
  br i1 %21, label %bb.nph.i.i, label %._crit_edge.i.i.loopexit

._crit_edge.i.i.loopexit:                         ; preds = %bb.nph.i.i
  %.lcssa3 = phi i32 [ %20, %bb.nph.i.i ]
  %.lcssa2 = phi i32 [ %19, %bb.nph.i.i ]
  br label %._crit_edge.i.i

._crit_edge.i.i:                                  ; preds = %._crit_edge.i.i.loopexit, %bb.nph7.i.i
  %shifted_divisor.0.lcssa.i.i = phi i32 [ %13, %bb.nph7.i.i ], [ %.lcssa2, %._crit_edge.i.i.loopexit ]
  %curr_quotient.0.lcssa.i.i = phi i32 [ 1, %bb.nph7.i.i ], [ %.lcssa3, %._crit_edge.i.i.loopexit ]
  %22 = add i32 %curr_quotient.0.lcssa.i.i, %quotient.05.i.i
  %23 = sub i32 %.016.i.i, %shifted_divisor.0.lcssa.i.i
  %24 = icmp ult i32 %23, %13
  br i1 %24, label %.loopexit.i.i.loopexit, label %bb.nph7.i.i

.loopexit.i.i.loopexit:                           ; preds = %._crit_edge.i.i
  %.lcssa4 = phi i32 [ %22, %._crit_edge.i.i ]
  br label %.loopexit.i.i

.loopexit.i.i:                                    ; preds = %.loopexit.i.i.loopexit, %3
  %.0.i.i = phi i32 [ -1, %3 ], [ %.lcssa4, %.loopexit.i.i.loopexit ]
  br label %udiv32.exit.preheader.i

udiv32.exit.preheader.i:                          ; preds = %.loopexit.i.i, %3
  %temp.0.ph.i = phi i32 [ %.0.i.i, %.loopexit.i.i ], [ %10, %3 ]
  %25 = and i32 %temp.0.ph.i, 8388608
  %26 = icmp eq i32 %25, 0
  %27 = icmp ne i32 %temp.0.ph.i, 0
  %or.cond11.i = and i1 %26, %27
  br i1 %or.cond11.i, label %udiv32.exit.i.preheader, label %.critedge.i

udiv32.exit.i.preheader:                          ; preds = %udiv32.exit.preheader.i
  br label %udiv32.exit.i

udiv32.exit.i:                                    ; preds = %udiv32.exit.i, %udiv32.exit.i.preheader
  %28 = phi i32 [ %indvar.next.i, %udiv32.exit.i ], [ 0, %udiv32.exit.i.preheader ]
  %temp.012.i = phi i32 [ %29, %udiv32.exit.i ], [ %temp.0.ph.i, %udiv32.exit.i.preheader ]
  %29 = shl i32 %temp.012.i, 1
  %30 = and i32 %29, 8388608
  %31 = icmp eq i32 %30, 0
  %32 = icmp ne i32 %29, 0
  %or.cond.i = and i1 %31, %32
  %indvar.next.i = add i32 %28, 1
  br i1 %or.cond.i, label %udiv32.exit.i, label %udiv32.exit..critedge_crit_edge.i

udiv32.exit..critedge_crit_edge.i:                ; preds = %udiv32.exit.i
  %.lcssa1 = phi i32 [ %29, %udiv32.exit.i ]
  %.lcssa = phi i32 [ %28, %udiv32.exit.i ]
  %.not = or i32 %6, -256
  %tmp21.i = xor i32 %.not, 255
  %tmp25.i = add i32 %tmp21.i, %5
  %tmp26.i = sub i32 %tmp25.i, %.lcssa
  br label %.critedge.i

.critedge.i:                                      ; preds = %udiv32.exit..critedge_crit_edge.i, %udiv32.exit.preheader.i
  %exp.0.lcssa.i = phi i32 [ %tmp26.i, %udiv32.exit..critedge_crit_edge.i ], [ %16, %udiv32.exit.preheader.i ]
  %temp.0.lcssa.i = phi i32 [ %.lcssa1, %udiv32.exit..critedge_crit_edge.i ], [ %temp.0.ph.i, %udiv32.exit.preheader.i ]
  %33 = and i32 %temp.0.lcssa.i, 8388607
  %34 = shl i32 %exp.0.lcssa.i, 23
  %35 = add i32 %34, 1140850688
  %36 = or i32 %35, %15
  %37 = or i32 %36, %33
  %tmp3.i = bitcast i32 %37 to float
  ret float %tmp3.i

fdiv32.exit:                                      ; preds = %0
  ret float 0.000000e+00
}

define float @theta_calc(float %omega_r, float %omega_m, float %theta_prev, float %del_t) nounwind readnone {
  %1 = fadd float %omega_r, %omega_m
  %2 = fmul float %1, %del_t
  %3 = fadd float %2, %theta_prev
  ret float %3
}

define float @iq_err_calc(float %torque_ref, float %flux_rotor) nounwind readnone {
  %1 = fmul float %torque_ref, 0x3FD62E3540000000
  %tmp10.i = bitcast float %1 to i32
  %tmp6.i = bitcast float %flux_rotor to i32
  %2 = fcmp oeq float %1, 0.000000e+00
  br i1 %2, label %fdiv32.exit, label %3

; <label>:3                                       ; preds = %0
  %4 = lshr i32 %tmp10.i, 23
  %5 = and i32 %4, 255
  %6 = lshr i32 %tmp6.i, 23
  %7 = and i32 %6, 255
  %8 = shl i32 %tmp10.i, 7
  %9 = and i32 %8, 1073741696
  %10 = or i32 %9, 1073741824
  %11 = lshr i32 %tmp6.i, 7
  %12 = and i32 %11, 65535
  %13 = or i32 %12, 65536
  %14 = xor i32 %tmp6.i, %tmp10.i
  %15 = and i32 %14, -2147483648
  %16 = sub i32 %5, %7
  switch i32 %13, label %bb.nph7.i.i.preheader [
    i32 0, label %.loopexit.i.i
    i32 1, label %udiv32.exit.preheader.i
  ]

bb.nph7.i.i.preheader:                            ; preds = %3
  br label %bb.nph7.i.i

bb.nph7.i.i:                                      ; preds = %._crit_edge.i.i, %bb.nph7.i.i.preheader
  %.016.i.i = phi i32 [ %23, %._crit_edge.i.i ], [ %10, %bb.nph7.i.i.preheader ]
  %quotient.05.i.i = phi i32 [ %22, %._crit_edge.i.i ], [ 0, %bb.nph7.i.i.preheader ]
  %17 = lshr i32 %.016.i.i, 1
  %18 = icmp ugt i32 %17, %13
  br i1 %18, label %bb.nph.i.i.preheader, label %._crit_edge.i.i

bb.nph.i.i.preheader:                             ; preds = %bb.nph7.i.i
  br label %bb.nph.i.i

bb.nph.i.i:                                       ; preds = %bb.nph.i.i, %bb.nph.i.i.preheader
  %shifted_divisor.03.i.i = phi i32 [ %19, %bb.nph.i.i ], [ %13, %bb.nph.i.i.preheader ]
  %curr_quotient.02.i.i = phi i32 [ %20, %bb.nph.i.i ], [ 1, %bb.nph.i.i.preheader ]
  %19 = shl i32 %shifted_divisor.03.i.i, 1
  %20 = shl i32 %curr_quotient.02.i.i, 1
  %21 = icmp ult i32 %19, %17
  br i1 %21, label %bb.nph.i.i, label %._crit_edge.i.i.loopexit

._crit_edge.i.i.loopexit:                         ; preds = %bb.nph.i.i
  %.lcssa3 = phi i32 [ %20, %bb.nph.i.i ]
  %.lcssa2 = phi i32 [ %19, %bb.nph.i.i ]
  br label %._crit_edge.i.i

._crit_edge.i.i:                                  ; preds = %._crit_edge.i.i.loopexit, %bb.nph7.i.i
  %shifted_divisor.0.lcssa.i.i = phi i32 [ %13, %bb.nph7.i.i ], [ %.lcssa2, %._crit_edge.i.i.loopexit ]
  %curr_quotient.0.lcssa.i.i = phi i32 [ 1, %bb.nph7.i.i ], [ %.lcssa3, %._crit_edge.i.i.loopexit ]
  %22 = add i32 %curr_quotient.0.lcssa.i.i, %quotient.05.i.i
  %23 = sub i32 %.016.i.i, %shifted_divisor.0.lcssa.i.i
  %24 = icmp ult i32 %23, %13
  br i1 %24, label %.loopexit.i.i.loopexit, label %bb.nph7.i.i

.loopexit.i.i.loopexit:                           ; preds = %._crit_edge.i.i
  %.lcssa4 = phi i32 [ %22, %._crit_edge.i.i ]
  br label %.loopexit.i.i

.loopexit.i.i:                                    ; preds = %.loopexit.i.i.loopexit, %3
  %.0.i.i = phi i32 [ -1, %3 ], [ %.lcssa4, %.loopexit.i.i.loopexit ]
  br label %udiv32.exit.preheader.i

udiv32.exit.preheader.i:                          ; preds = %.loopexit.i.i, %3
  %temp.0.ph.i = phi i32 [ %.0.i.i, %.loopexit.i.i ], [ %10, %3 ]
  %25 = and i32 %temp.0.ph.i, 8388608
  %26 = icmp eq i32 %25, 0
  %27 = icmp ne i32 %temp.0.ph.i, 0
  %or.cond11.i = and i1 %26, %27
  br i1 %or.cond11.i, label %udiv32.exit.i.preheader, label %.critedge.i

udiv32.exit.i.preheader:                          ; preds = %udiv32.exit.preheader.i
  br label %udiv32.exit.i

udiv32.exit.i:                                    ; preds = %udiv32.exit.i, %udiv32.exit.i.preheader
  %28 = phi i32 [ %indvar.next.i, %udiv32.exit.i ], [ 0, %udiv32.exit.i.preheader ]
  %temp.012.i = phi i32 [ %29, %udiv32.exit.i ], [ %temp.0.ph.i, %udiv32.exit.i.preheader ]
  %29 = shl i32 %temp.012.i, 1
  %30 = and i32 %29, 8388608
  %31 = icmp eq i32 %30, 0
  %32 = icmp ne i32 %29, 0
  %or.cond.i = and i1 %31, %32
  %indvar.next.i = add i32 %28, 1
  br i1 %or.cond.i, label %udiv32.exit.i, label %udiv32.exit..critedge_crit_edge.i

udiv32.exit..critedge_crit_edge.i:                ; preds = %udiv32.exit.i
  %.lcssa1 = phi i32 [ %29, %udiv32.exit.i ]
  %.lcssa = phi i32 [ %28, %udiv32.exit.i ]
  %.not = or i32 %6, -256
  %tmp21.i = xor i32 %.not, 255
  %tmp25.i = add i32 %tmp21.i, %5
  %tmp26.i = sub i32 %tmp25.i, %.lcssa
  br label %.critedge.i

.critedge.i:                                      ; preds = %udiv32.exit..critedge_crit_edge.i, %udiv32.exit.preheader.i
  %exp.0.lcssa.i = phi i32 [ %tmp26.i, %udiv32.exit..critedge_crit_edge.i ], [ %16, %udiv32.exit.preheader.i ]
  %temp.0.lcssa.i = phi i32 [ %.lcssa1, %udiv32.exit..critedge_crit_edge.i ], [ %temp.0.ph.i, %udiv32.exit.preheader.i ]
  %33 = and i32 %temp.0.lcssa.i, 8388607
  %34 = shl i32 %exp.0.lcssa.i, 23
  %35 = add i32 %34, 1140850688
  %36 = or i32 %35, %15
  %37 = or i32 %36, %33
  %tmp3.i = bitcast i32 %37 to float
  ret float %tmp3.i

fdiv32.exit:                                      ; preds = %0
  ret float 0.000000e+00
}

define void @vector_control_daemon() noreturn nounwind {
; <label>:0
  br label %1

; <label>:1                                       ; preds = %fdiv32.exit, %0
  %flux_rotor_lpf_prev.0 = phi float [ 0.000000e+00, %0 ], [ %phitmp33, %fdiv32.exit ]
  %int_flux_err_temp_2.0 = phi float [ 0.000000e+00, %0 ], [ %122, %fdiv32.exit ]
  %speed_err_prev.0 = phi float [ 0.000000e+00, %0 ], [ %21, %fdiv32.exit ]
  %theta_prev.0 = phi float [ 0.000000e+00, %0 ], [ %80, %fdiv32.exit ]
  %flux_rotor_prev.0 = phi float [ 0.000000e+00, %0 ], [ %phitmp31, %fdiv32.exit ]
  %spd_lpf_prev.0 = phi float [ 0.000000e+00, %0 ], [ %phitmp, %fdiv32.exit ]
  %int_speed_err_prev.0 = phi float [ 0.000000e+00, %0 ], [ %24, %fdiv32.exit ]
  %speed_ref.1 = phi float [ 0.000000e+00, %0 ], [ %speed_ref.0, %fdiv32.exit ]
  %2 = tail call float @read_float32(i8* getelementptr inbounds ([8 x i8]* @.str, i64 0, i64 0)) nounwind
  %3 = tail call float @read_float32(i8* getelementptr inbounds ([8 x i8]* @.str, i64 0, i64 0)) nounwind
  %4 = tail call float @read_float32(i8* getelementptr inbounds ([8 x i8]* @.str, i64 0, i64 0)) nounwind
  %5 = tail call float @read_float32(i8* getelementptr inbounds ([8 x i8]* @.str, i64 0, i64 0)) nounwind
  %6 = tail call float @read_float32(i8* getelementptr inbounds ([8 x i8]* @.str, i64 0, i64 0)) nounwind
  %7 = fcmp olt float %speed_ref.1, %5
  br i1 %7, label %8, label %12

; <label>:8                                       ; preds = %1
  %9 = fpext float %speed_ref.1 to double
  %10 = fadd double %9, 5.000000e-02
  %11 = fptrunc double %10 to float
  br label %18

; <label>:12                                      ; preds = %1
  %13 = fcmp ogt float %speed_ref.1, %5
  br i1 %13, label %14, label %18

; <label>:14                                      ; preds = %12
  %15 = fpext float %speed_ref.1 to double
  %16 = fadd double %15, -5.000000e-02
  %17 = fptrunc double %16 to float
  br label %18

; <label>:18                                      ; preds = %14, %12, %8
  %speed_ref.0 = phi float [ %11, %8 ], [ %17, %14 ], [ %speed_ref.1, %12 ]
  %19 = fmul float %4, 0x3FE6666660000000
  %20 = fadd float %19, %spd_lpf_prev.0
  %21 = fsub float %speed_ref.0, %20
  %22 = fadd float %21, %speed_err_prev.0
  %23 = fmul float %22, 0x3F30624DE0000000
  %24 = fadd float %23, %int_speed_err_prev.0
  %25 = fpext float %24 to double
  %26 = fcmp olt double %25, -1.000000e+01
  br i1 %26, label %30, label %27

; <label>:27                                      ; preds = %18
  %28 = fcmp ogt double %25, 1.000000e+01
  br i1 %28, label %30, label %29

; <label>:29                                      ; preds = %27
  br label %30

; <label>:30                                      ; preds = %29, %27, %18
  %int_speed_err.0 = phi float [ %24, %29 ], [ -1.000000e+01, %18 ], [ 1.000000e+01, %27 ]
  %31 = fmul float %21, 5.000000e+00
  %32 = fadd float %int_speed_err.0, %31
  %33 = fcmp olt float %32, -2.000000e+01
  br i1 %33, label %37, label %34

; <label>:34                                      ; preds = %30
  %35 = fcmp ogt float %32, 2.000000e+01
  br i1 %35, label %37, label %36

; <label>:36                                      ; preds = %34
  %phitmp32 = fmul float %32, 0x3FD62E3540000000
  br label %37

; <label>:37                                      ; preds = %36, %34, %30
  %torque_ref.0 = phi float [ %phitmp32, %36 ], [ 0xC01BB9C280000000, %30 ], [ 0x401BB9C280000000, %34 ]
  %38 = fmul float %2, 0x3F3977C2A0000000
  %39 = fadd float %38, %flux_rotor_prev.0
  %40 = fmul float %3, 0x401F299A00000000
  %tmp10.i.i1 = bitcast float %40 to i32
  %tmp6.i.i2 = bitcast float %39 to i32
  %41 = fcmp oeq float %40, 0.000000e+00
  br i1 %41, label %omega_calc.exit, label %42

; <label>:42                                      ; preds = %37
  %43 = lshr i32 %tmp10.i.i1, 23
  %44 = and i32 %43, 255
  %45 = lshr i32 %tmp6.i.i2, 23
  %46 = and i32 %45, 255
  %47 = shl i32 %tmp10.i.i1, 7
  %48 = and i32 %47, 1073741696
  %49 = or i32 %48, 1073741824
  %50 = lshr i32 %tmp6.i.i2, 7
  %51 = and i32 %50, 65535
  %52 = or i32 %51, 65536
  %53 = xor i32 %tmp6.i.i2, %tmp10.i.i1
  %54 = and i32 %53, -2147483648
  %55 = sub i32 %44, %46
  switch i32 %52, label %bb.nph7.i.i.i5.preheader [
    i32 0, label %.loopexit.i.i.i13
    i32 1, label %udiv32.exit.preheader.i.i16
  ]

bb.nph7.i.i.i5.preheader:                         ; preds = %42
  br label %bb.nph7.i.i.i5

bb.nph7.i.i.i5:                                   ; preds = %._crit_edge.i.i.i11, %bb.nph7.i.i.i5.preheader
  %.016.i.i.i3 = phi i32 [ %62, %._crit_edge.i.i.i11 ], [ %49, %bb.nph7.i.i.i5.preheader ]
  %quotient.05.i.i.i4 = phi i32 [ %61, %._crit_edge.i.i.i11 ], [ 0, %bb.nph7.i.i.i5.preheader ]
  %56 = lshr i32 %.016.i.i.i3, 1
  %57 = icmp ugt i32 %56, %52
  br i1 %57, label %bb.nph.i.i.i8.preheader, label %._crit_edge.i.i.i11

bb.nph.i.i.i8.preheader:                          ; preds = %bb.nph7.i.i.i5
  br label %bb.nph.i.i.i8

bb.nph.i.i.i8:                                    ; preds = %bb.nph.i.i.i8, %bb.nph.i.i.i8.preheader
  %shifted_divisor.03.i.i.i6 = phi i32 [ %58, %bb.nph.i.i.i8 ], [ %52, %bb.nph.i.i.i8.preheader ]
  %curr_quotient.02.i.i.i7 = phi i32 [ %59, %bb.nph.i.i.i8 ], [ 1, %bb.nph.i.i.i8.preheader ]
  %58 = shl i32 %shifted_divisor.03.i.i.i6, 1
  %59 = shl i32 %curr_quotient.02.i.i.i7, 1
  %60 = icmp ult i32 %58, %56
  br i1 %60, label %bb.nph.i.i.i8, label %._crit_edge.i.i.i11.loopexit

._crit_edge.i.i.i11.loopexit:                     ; preds = %bb.nph.i.i.i8
  %.lcssa13 = phi i32 [ %59, %bb.nph.i.i.i8 ]
  %.lcssa12 = phi i32 [ %58, %bb.nph.i.i.i8 ]
  br label %._crit_edge.i.i.i11

._crit_edge.i.i.i11:                              ; preds = %._crit_edge.i.i.i11.loopexit, %bb.nph7.i.i.i5
  %shifted_divisor.0.lcssa.i.i.i9 = phi i32 [ %52, %bb.nph7.i.i.i5 ], [ %.lcssa12, %._crit_edge.i.i.i11.loopexit ]
  %curr_quotient.0.lcssa.i.i.i10 = phi i32 [ 1, %bb.nph7.i.i.i5 ], [ %.lcssa13, %._crit_edge.i.i.i11.loopexit ]
  %61 = add i32 %curr_quotient.0.lcssa.i.i.i10, %quotient.05.i.i.i4
  %62 = sub i32 %.016.i.i.i3, %shifted_divisor.0.lcssa.i.i.i9
  %63 = icmp ult i32 %62, %52
  br i1 %63, label %.loopexit.i.i.i13.loopexit, label %bb.nph7.i.i.i5

.loopexit.i.i.i13.loopexit:                       ; preds = %._crit_edge.i.i.i11
  %.lcssa14 = phi i32 [ %61, %._crit_edge.i.i.i11 ]
  br label %.loopexit.i.i.i13

.loopexit.i.i.i13:                                ; preds = %.loopexit.i.i.i13.loopexit, %42
  %.0.i.i.i12 = phi i32 [ -1, %42 ], [ %.lcssa14, %.loopexit.i.i.i13.loopexit ]
  br label %udiv32.exit.preheader.i.i16

udiv32.exit.preheader.i.i16:                      ; preds = %.loopexit.i.i.i13, %42
  %temp.0.ph.i.i14 = phi i32 [ %.0.i.i.i12, %.loopexit.i.i.i13 ], [ %49, %42 ]
  %64 = and i32 %temp.0.ph.i.i14, 8388608
  %65 = icmp eq i32 %64, 0
  %66 = icmp ne i32 %temp.0.ph.i.i14, 0
  %or.cond11.i.i15 = and i1 %65, %66
  br i1 %or.cond11.i.i15, label %udiv32.exit.i.i20.preheader, label %.critedge.i.i29

udiv32.exit.i.i20.preheader:                      ; preds = %udiv32.exit.preheader.i.i16
  br label %udiv32.exit.i.i20

udiv32.exit.i.i20:                                ; preds = %udiv32.exit.i.i20, %udiv32.exit.i.i20.preheader
  %67 = phi i32 [ %indvar.next.i.i19, %udiv32.exit.i.i20 ], [ 0, %udiv32.exit.i.i20.preheader ]
  %temp.012.i.i17 = phi i32 [ %68, %udiv32.exit.i.i20 ], [ %temp.0.ph.i.i14, %udiv32.exit.i.i20.preheader ]
  %68 = shl i32 %temp.012.i.i17, 1
  %69 = and i32 %68, 8388608
  %70 = icmp eq i32 %69, 0
  %71 = icmp ne i32 %68, 0
  %or.cond.i.i18 = and i1 %70, %71
  %indvar.next.i.i19 = add i32 %67, 1
  br i1 %or.cond.i.i18, label %udiv32.exit.i.i20, label %udiv32.exit..critedge_crit_edge.i.i25

udiv32.exit..critedge_crit_edge.i.i25:            ; preds = %udiv32.exit.i.i20
  %.lcssa11 = phi i32 [ %68, %udiv32.exit.i.i20 ]
  %.lcssa10 = phi i32 [ %67, %udiv32.exit.i.i20 ]
  %.not.i21 = or i32 %45, -256
  %tmp21.i.i22 = xor i32 %.not.i21, 255
  %tmp25.i.i23 = add i32 %tmp21.i.i22, %44
  %tmp26.i.i24 = sub i32 %tmp25.i.i23, %.lcssa10
  br label %.critedge.i.i29

.critedge.i.i29:                                  ; preds = %udiv32.exit..critedge_crit_edge.i.i25, %udiv32.exit.preheader.i.i16
  %exp.0.lcssa.i.i26 = phi i32 [ %tmp26.i.i24, %udiv32.exit..critedge_crit_edge.i.i25 ], [ %55, %udiv32.exit.preheader.i.i16 ]
  %temp.0.lcssa.i.i27 = phi i32 [ %.lcssa11, %udiv32.exit..critedge_crit_edge.i.i25 ], [ %temp.0.ph.i.i14, %udiv32.exit.preheader.i.i16 ]
  %72 = and i32 %temp.0.lcssa.i.i27, 8388607
  %73 = shl i32 %exp.0.lcssa.i.i26, 23
  %74 = add i32 %73, 1140850688
  %75 = or i32 %72, %54
  %76 = or i32 %75, %74
  %tmp3.i.i28 = bitcast i32 %76 to float
  br label %omega_calc.exit

omega_calc.exit:                                  ; preds = %.critedge.i.i29, %37
  %77 = phi float [ %tmp3.i.i28, %.critedge.i.i29 ], [ 0.000000e+00, %37 ]
  %78 = fadd float %77, %6
  %79 = fmul float %78, 0x3F0A36E2E0000000
  %80 = fadd float %79, %theta_prev.0
  %tmp10.i.i = bitcast float %torque_ref.0 to i32
  %81 = fcmp oeq float %torque_ref.0, 0.000000e+00
  br i1 %81, label %iq_err_calc.exit, label %82

; <label>:82                                      ; preds = %omega_calc.exit
  %83 = lshr i32 %tmp10.i.i, 23
  %84 = and i32 %83, 255
  %85 = lshr i32 %tmp6.i.i2, 23
  %86 = and i32 %85, 255
  %87 = shl i32 %tmp10.i.i, 7
  %88 = and i32 %87, 1073741696
  %89 = or i32 %88, 1073741824
  %90 = lshr i32 %tmp6.i.i2, 7
  %91 = and i32 %90, 65535
  %92 = or i32 %91, 65536
  %93 = xor i32 %tmp10.i.i, %tmp6.i.i2
  %94 = and i32 %93, -2147483648
  %95 = sub i32 %84, %86
  switch i32 %92, label %bb.nph7.i.i.i.preheader [
    i32 0, label %.loopexit.i.i.i
    i32 1, label %udiv32.exit.preheader.i.i
  ]

bb.nph7.i.i.i.preheader:                          ; preds = %82
  br label %bb.nph7.i.i.i

bb.nph7.i.i.i:                                    ; preds = %._crit_edge.i.i.i, %bb.nph7.i.i.i.preheader
  %.016.i.i.i = phi i32 [ %102, %._crit_edge.i.i.i ], [ %89, %bb.nph7.i.i.i.preheader ]
  %quotient.05.i.i.i = phi i32 [ %101, %._crit_edge.i.i.i ], [ 0, %bb.nph7.i.i.i.preheader ]
  %96 = lshr i32 %.016.i.i.i, 1
  %97 = icmp ugt i32 %96, %92
  br i1 %97, label %bb.nph.i.i.i.preheader, label %._crit_edge.i.i.i

bb.nph.i.i.i.preheader:                           ; preds = %bb.nph7.i.i.i
  br label %bb.nph.i.i.i

bb.nph.i.i.i:                                     ; preds = %bb.nph.i.i.i, %bb.nph.i.i.i.preheader
  %shifted_divisor.03.i.i.i = phi i32 [ %98, %bb.nph.i.i.i ], [ %92, %bb.nph.i.i.i.preheader ]
  %curr_quotient.02.i.i.i = phi i32 [ %99, %bb.nph.i.i.i ], [ 1, %bb.nph.i.i.i.preheader ]
  %98 = shl i32 %shifted_divisor.03.i.i.i, 1
  %99 = shl i32 %curr_quotient.02.i.i.i, 1
  %100 = icmp ult i32 %98, %96
  br i1 %100, label %bb.nph.i.i.i, label %._crit_edge.i.i.i.loopexit

._crit_edge.i.i.i.loopexit:                       ; preds = %bb.nph.i.i.i
  %.lcssa8 = phi i32 [ %99, %bb.nph.i.i.i ]
  %.lcssa7 = phi i32 [ %98, %bb.nph.i.i.i ]
  br label %._crit_edge.i.i.i

._crit_edge.i.i.i:                                ; preds = %._crit_edge.i.i.i.loopexit, %bb.nph7.i.i.i
  %shifted_divisor.0.lcssa.i.i.i = phi i32 [ %92, %bb.nph7.i.i.i ], [ %.lcssa7, %._crit_edge.i.i.i.loopexit ]
  %curr_quotient.0.lcssa.i.i.i = phi i32 [ 1, %bb.nph7.i.i.i ], [ %.lcssa8, %._crit_edge.i.i.i.loopexit ]
  %101 = add i32 %curr_quotient.0.lcssa.i.i.i, %quotient.05.i.i.i
  %102 = sub i32 %.016.i.i.i, %shifted_divisor.0.lcssa.i.i.i
  %103 = icmp ult i32 %102, %92
  br i1 %103, label %.loopexit.i.i.i.loopexit, label %bb.nph7.i.i.i

.loopexit.i.i.i.loopexit:                         ; preds = %._crit_edge.i.i.i
  %.lcssa9 = phi i32 [ %101, %._crit_edge.i.i.i ]
  br label %.loopexit.i.i.i

.loopexit.i.i.i:                                  ; preds = %.loopexit.i.i.i.loopexit, %82
  %.0.i.i.i = phi i32 [ -1, %82 ], [ %.lcssa9, %.loopexit.i.i.i.loopexit ]
  br label %udiv32.exit.preheader.i.i

udiv32.exit.preheader.i.i:                        ; preds = %.loopexit.i.i.i, %82
  %temp.0.ph.i.i = phi i32 [ %.0.i.i.i, %.loopexit.i.i.i ], [ %89, %82 ]
  %104 = and i32 %temp.0.ph.i.i, 8388608
  %105 = icmp eq i32 %104, 0
  %106 = icmp ne i32 %temp.0.ph.i.i, 0
  %or.cond11.i.i = and i1 %105, %106
  br i1 %or.cond11.i.i, label %udiv32.exit.i.i.preheader, label %.critedge.i.i

udiv32.exit.i.i.preheader:                        ; preds = %udiv32.exit.preheader.i.i
  br label %udiv32.exit.i.i

udiv32.exit.i.i:                                  ; preds = %udiv32.exit.i.i, %udiv32.exit.i.i.preheader
  %107 = phi i32 [ %indvar.next.i.i, %udiv32.exit.i.i ], [ 0, %udiv32.exit.i.i.preheader ]
  %temp.012.i.i = phi i32 [ %108, %udiv32.exit.i.i ], [ %temp.0.ph.i.i, %udiv32.exit.i.i.preheader ]
  %108 = shl i32 %temp.012.i.i, 1
  %109 = and i32 %108, 8388608
  %110 = icmp eq i32 %109, 0
  %111 = icmp ne i32 %108, 0
  %or.cond.i.i = and i1 %110, %111
  %indvar.next.i.i = add i32 %107, 1
  br i1 %or.cond.i.i, label %udiv32.exit.i.i, label %udiv32.exit..critedge_crit_edge.i.i

udiv32.exit..critedge_crit_edge.i.i:              ; preds = %udiv32.exit.i.i
  %.lcssa6 = phi i32 [ %108, %udiv32.exit.i.i ]
  %.lcssa5 = phi i32 [ %107, %udiv32.exit.i.i ]
  %.not.i = or i32 %85, -256
  %tmp21.i.i = xor i32 %.not.i, 255
  %tmp25.i.i = add i32 %84, %tmp21.i.i
  %tmp26.i.i = sub i32 %tmp25.i.i, %.lcssa5
  br label %.critedge.i.i

.critedge.i.i:                                    ; preds = %udiv32.exit..critedge_crit_edge.i.i, %udiv32.exit.preheader.i.i
  %exp.0.lcssa.i.i = phi i32 [ %tmp26.i.i, %udiv32.exit..critedge_crit_edge.i.i ], [ %95, %udiv32.exit.preheader.i.i ]
  %temp.0.lcssa.i.i = phi i32 [ %.lcssa6, %udiv32.exit..critedge_crit_edge.i.i ], [ %temp.0.ph.i.i, %udiv32.exit.preheader.i.i ]
  %112 = and i32 %temp.0.lcssa.i.i, 8388607
  %113 = shl i32 %exp.0.lcssa.i.i, 23
  %114 = add i32 %113, 1140850688
  %115 = or i32 %112, %94
  %116 = or i32 %115, %114
  %tmp3.i.i = bitcast i32 %116 to float
  br label %iq_err_calc.exit

iq_err_calc.exit:                                 ; preds = %.critedge.i.i, %omega_calc.exit
  %117 = phi float [ %tmp3.i.i, %.critedge.i.i ], [ 0.000000e+00, %omega_calc.exit ]
  %118 = fmul float %39, 0x3F74898F60000000
  %119 = fadd float %118, %flux_rotor_lpf_prev.0
  %120 = fsub float 0x3FD3333340000000, %119
  %121 = fmul float %120, 0x3F0A36E2E0000000
  %122 = fadd float %121, %int_flux_err_temp_2.0
  %123 = fmul float %122, 5.000000e+01
  %124 = fcmp olt float %123, -1.000000e+00
  br i1 %124, label %128, label %125

; <label>:125                                     ; preds = %iq_err_calc.exit
  %126 = fcmp ogt float %123, 1.000000e+00
  br i1 %126, label %128, label %127

; <label>:127                                     ; preds = %125
  br label %128

; <label>:128                                     ; preds = %127, %125, %iq_err_calc.exit
  %int_flux_err.0 = phi float [ %123, %127 ], [ -1.000000e+00, %iq_err_calc.exit ], [ 1.000000e+00, %125 ]
  %129 = fmul float %120, 4.000000e+01
  %130 = fadd float %int_flux_err.0, %129
  %131 = fcmp olt float %130, -2.000000e+00
  br i1 %131, label %.thread, label %132

; <label>:132                                     ; preds = %128
  %133 = fcmp ogt float %130, 2.000000e+00
  br i1 %133, label %.thread, label %134

; <label>:134                                     ; preds = %132
  %135 = fcmp oeq float %130, 0.000000e+00
  br i1 %135, label %fdiv32.exit, label %.thread

.thread:                                          ; preds = %134, %132, %128
  %tmp10.i35.in = phi float [ %130, %134 ], [ -2.000000e+00, %128 ], [ 2.000000e+00, %132 ]
  %tmp10.i35 = bitcast float %tmp10.i35.in to i32
  %136 = shl i32 %tmp10.i35, 7
  %137 = and i32 %136, 1073741696
  %138 = or i32 %137, 1073741824
  br label %bb.nph7.i.i

bb.nph7.i.i:                                      ; preds = %._crit_edge.i.i, %.thread
  %.016.i.i = phi i32 [ %145, %._crit_edge.i.i ], [ %138, %.thread ]
  %quotient.05.i.i = phi i32 [ %144, %._crit_edge.i.i ], [ 0, %.thread ]
  %139 = lshr i32 %.016.i.i, 1
  %140 = icmp ugt i32 %139, 106115
  br i1 %140, label %bb.nph.i.i.preheader, label %._crit_edge.i.i

bb.nph.i.i.preheader:                             ; preds = %bb.nph7.i.i
  br label %bb.nph.i.i

bb.nph.i.i:                                       ; preds = %bb.nph.i.i, %bb.nph.i.i.preheader
  %shifted_divisor.03.i.i = phi i32 [ %141, %bb.nph.i.i ], [ 106115, %bb.nph.i.i.preheader ]
  %curr_quotient.02.i.i = phi i32 [ %142, %bb.nph.i.i ], [ 1, %bb.nph.i.i.preheader ]
  %141 = shl i32 %shifted_divisor.03.i.i, 1
  %142 = shl i32 %curr_quotient.02.i.i, 1
  %143 = icmp ult i32 %141, %139
  br i1 %143, label %bb.nph.i.i, label %._crit_edge.i.i.loopexit

._crit_edge.i.i.loopexit:                         ; preds = %bb.nph.i.i
  %.lcssa3 = phi i32 [ %142, %bb.nph.i.i ]
  %.lcssa2 = phi i32 [ %141, %bb.nph.i.i ]
  br label %._crit_edge.i.i

._crit_edge.i.i:                                  ; preds = %._crit_edge.i.i.loopexit, %bb.nph7.i.i
  %shifted_divisor.0.lcssa.i.i = phi i32 [ 106115, %bb.nph7.i.i ], [ %.lcssa2, %._crit_edge.i.i.loopexit ]
  %curr_quotient.0.lcssa.i.i = phi i32 [ 1, %bb.nph7.i.i ], [ %.lcssa3, %._crit_edge.i.i.loopexit ]
  %144 = add i32 %curr_quotient.0.lcssa.i.i, %quotient.05.i.i
  %145 = sub i32 %.016.i.i, %shifted_divisor.0.lcssa.i.i
  %146 = icmp ult i32 %145, 106115
  br i1 %146, label %udiv32.exit.preheader.i, label %bb.nph7.i.i

udiv32.exit.preheader.i:                          ; preds = %._crit_edge.i.i
  %.lcssa4 = phi i32 [ %144, %._crit_edge.i.i ]
  %147 = lshr i32 %tmp10.i35, 23
  %148 = and i32 %tmp10.i35, -2147483648
  %149 = and i32 %147, 255
  %150 = add i32 %149, -126
  %151 = and i32 %.lcssa4, 8388608
  %152 = icmp eq i32 %151, 0
  %153 = icmp ne i32 %.lcssa4, 0
  %or.cond11.i = and i1 %152, %153
  br i1 %or.cond11.i, label %udiv32.exit.i.preheader, label %.critedge.i

udiv32.exit.i.preheader:                          ; preds = %udiv32.exit.preheader.i
  br label %udiv32.exit.i

udiv32.exit.i:                                    ; preds = %udiv32.exit.i, %udiv32.exit.i.preheader
  %154 = phi i32 [ %indvar.next.i, %udiv32.exit.i ], [ 0, %udiv32.exit.i.preheader ]
  %temp.012.i = phi i32 [ %155, %udiv32.exit.i ], [ %.lcssa4, %udiv32.exit.i.preheader ]
  %155 = shl i32 %temp.012.i, 1
  %156 = and i32 %155, 8388608
  %157 = icmp eq i32 %156, 0
  %158 = icmp ne i32 %155, 0
  %or.cond.i = and i1 %157, %158
  %indvar.next.i = add i32 %154, 1
  br i1 %or.cond.i, label %udiv32.exit.i, label %udiv32.exit..critedge_crit_edge.i

udiv32.exit..critedge_crit_edge.i:                ; preds = %udiv32.exit.i
  %.lcssa1 = phi i32 [ %155, %udiv32.exit.i ]
  %.lcssa = phi i32 [ %154, %udiv32.exit.i ]
  %tmp25.i = add i32 %149, -127
  %tmp26.i = sub i32 %tmp25.i, %.lcssa
  br label %.critedge.i

.critedge.i:                                      ; preds = %udiv32.exit..critedge_crit_edge.i, %udiv32.exit.preheader.i
  %exp.0.lcssa.i = phi i32 [ %tmp26.i, %udiv32.exit..critedge_crit_edge.i ], [ %150, %udiv32.exit.preheader.i ]
  %temp.0.lcssa.i = phi i32 [ %.lcssa1, %udiv32.exit..critedge_crit_edge.i ], [ %.lcssa4, %udiv32.exit.preheader.i ]
  %159 = and i32 %temp.0.lcssa.i, 8388607
  %160 = shl i32 %exp.0.lcssa.i, 23
  %161 = add i32 %160, 1140850688
  %162 = or i32 %161, %148
  %163 = or i32 %162, %159
  %tmp3.i = bitcast i32 %163 to float
  br label %fdiv32.exit

fdiv32.exit:                                      ; preds = %.critedge.i, %134
  %164 = phi float [ %tmp3.i, %.critedge.i ], [ 0.000000e+00, %134 ]
  tail call void @write_float32(i8* getelementptr inbounds ([9 x i8]* @.str1, i64 0, i64 0), float %164) nounwind
  tail call void @write_float32(i8* getelementptr inbounds ([9 x i8]* @.str1, i64 0, i64 0), float %117) nounwind
  tail call void @write_float32(i8* getelementptr inbounds ([9 x i8]* @.str1, i64 0, i64 0), float %80) nounwind
  tail call void @write_float32(i8* getelementptr inbounds ([9 x i8]* @.str1, i64 0, i64 0), float %39) nounwind
  %phitmp = fmul float %20, 0x3FD3333340000000
  %phitmp31 = fmul float %39, 0x3FEFFC1160000000
  %phitmp33 = fmul float %119, 0x3FEFD6ECE0000000
  br label %1
}

declare float @read_float32(i8*)

declare void @write_float32(i8*, float)
