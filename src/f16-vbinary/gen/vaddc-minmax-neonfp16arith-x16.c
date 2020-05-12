// Auto-generated file. Do not edit!
//   Template: src/f16-vbinary/vopc-neonfp16arith.c.in
//   Generator: tools/xngen
//
// Copyright 2020 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <arm_neon.h>

#include <xnnpack/common.h>
#include <xnnpack/vbinary.h>


void xnn_f16_vaddc_minmax_ukernel__neonfp16arith_x16(
    size_t n,
    const void* restrict a,
    const void* restrict b,
    void* restrict y,
    const struct xnn_f16_minmax_params params[restrict XNN_MIN_ELEMENTS(1)])
{
  assert(n != 0);
  assert(n % sizeof(__fp16) == 0);

  const __fp16* a0 = a;
  const __fp16* b0 = b;
  __fp16* y0 = y;

  const float16x8_t vy_min = vld1q_dup_f16(&params->min);
  const float16x8_t vy_max = vld1q_dup_f16(&params->max);

  const float16x8_t vb = vld1q_dup_f16(b0);
  for (; n >= 16 * sizeof(__fp16); n -= 16 * sizeof(__fp16)) {
    const float16x8_t va01234567 = vld1q_f16(a0); a0 += 8;
    const float16x8_t va456789AB = vld1q_f16(a0); a0 += 8;

    float16x8_t vy01234567 = vaddq_f16(va01234567, vb);
    float16x8_t vy456789AB = vaddq_f16(va456789AB, vb);

    vy01234567 = vmaxq_f16(vy01234567, vy_min);
    vy456789AB = vmaxq_f16(vy456789AB, vy_min);

    vy01234567 = vminq_f16(vy01234567, vy_max);
    vy456789AB = vminq_f16(vy456789AB, vy_max);

    vst1q_f16(y0, vy01234567); y0 += 8;
    vst1q_f16(y0, vy456789AB); y0 += 8;
  }
  for (; n >= 8 * sizeof(__fp16); n -= 8 * sizeof(__fp16)) {
    const float16x8_t va01234567 = vld1q_f16(a0); a0 += 8;

    float16x8_t vy01234567 = vaddq_f16(va01234567, vb);
    vy01234567 = vmaxq_f16(vy01234567, vy_min);
    vy01234567 = vminq_f16(vy01234567, vy_max);
    vst1q_f16(y0, vy01234567); y0 += 8;
  }
  if XNN_UNLIKELY(n != 0) {
    const float16x8_t va01234567 = vld1q_f16(a0);

    float16x8_t vy01234567 = vaddq_f16(va01234567, vb);
    vy01234567 = vmaxq_f16(vy01234567, vy_min);
    vy01234567 = vminq_f16(vy01234567, vy_max);

    float16x4_t vy0123 = vget_low_f16(vy01234567);
    if (n & (4 * sizeof(__fp16))) {
      vst1_f16(y0, vy0123); y0 += 4;
      vy0123 = vget_high_f16(vy01234567);
    }

    if (n & (2 * sizeof(__fp16))) {
      vst1_lane_u32(__builtin_assume_aligned(y0, 1), vreinterpret_u32_f16(vy0123), 0); y0 += 2;
      vy0123 = vext_f16(vy0123, vy0123, 2);
    }

    if (n & (1 * sizeof(__fp16))) {
      vst1_lane_f16(y0, vy0123, 0);
    }
  }
}