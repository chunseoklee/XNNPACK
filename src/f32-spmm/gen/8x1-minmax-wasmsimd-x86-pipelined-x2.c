// Auto-generated file. Do not edit!
//   Template: src/f32-spmm/wasmsimd-pipelined.c.in
//   Generator: tools/xngen
//
// Copyright 2020 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <wasm_simd128.h>

#include <xnnpack/spmm.h>


void xnn_f32_spmm_minmax_ukernel_8x1__wasmsimd_x86_pipelined_x2(
    size_t batch_size,
    size_t output_channels,
    const float*restrict input,
    const float*restrict weights,
    const int32_t*restrict widx_dmap,
    const uint32_t*restrict nidx_nnzmap,
    float*restrict output,
    const union xnn_f32_minmax_params params[restrict XNN_MIN_ELEMENTS(1)])
{
  assert(batch_size != 0);
  assert(batch_size % sizeof(float) == 0);
  assert(output_channels != 0);

  const v128_t vmin = wasm_v32x4_load_splat(&params->scalar.min);
  const v128_t vmax = wasm_v32x4_load_splat(&params->scalar.max);
  size_t output_decrement = batch_size * output_channels - 8 * sizeof(float);
  size_t n = batch_size;
  while XNN_LIKELY(n >= 8 * sizeof(float)) {
    const float*restrict w = weights;
    const int32_t* dmap = widx_dmap;
    const uint32_t* nnzmap = nidx_nnzmap;
    v128_t vw = wasm_v32x4_load_splat(w); w += 1;
    intptr_t diff = *dmap++;
    v128_t vi0123 = wasm_v128_load(input + 0);
    v128_t vi4567 = wasm_v128_load(input + 4);
    size_t c = output_channels;
    do {
      uint32_t nnz = *nnzmap++;
       v128_t vacc0123 = vw;
       v128_t vacc4567 = vw;
      vw = wasm_v32x4_load_splat(w); w += 1;

      for (; nnz >= 2; nnz -= 2) {
        vacc0123 = wasm_f32x4_add(vacc0123, wasm_f32x4_mul(vi0123,   vw));
        vacc4567 = wasm_f32x4_add(vacc4567, wasm_f32x4_mul(vi4567,   vw));
        input = (const float*restrict) ((uintptr_t) input + (uintptr_t) diff);
        diff = *dmap++;
        vw = wasm_v32x4_load_splat(w); w += 1;
        vi0123 = wasm_v128_load(input + 0);
        vi4567 = wasm_v128_load(input + 4);
        vacc0123 = wasm_f32x4_add(vacc0123, wasm_f32x4_mul(vi0123,   vw));
        vacc4567 = wasm_f32x4_add(vacc4567, wasm_f32x4_mul(vi4567,   vw));
        input = (const float*restrict) ((uintptr_t) input + (uintptr_t) diff);
        diff = *dmap++;
        vw = wasm_v32x4_load_splat(w); w += 1;
        vi0123 = wasm_v128_load(input + 0);
        vi4567 = wasm_v128_load(input + 4);
      }

      if XNN_LIKELY(nnz != 0) {
        do {
          vacc0123 = wasm_f32x4_add(vacc0123, wasm_f32x4_mul(vi0123, vw));
          vacc4567 = wasm_f32x4_add(vacc4567, wasm_f32x4_mul(vi4567, vw));
          input = (const float*restrict) ((uintptr_t) input + (uintptr_t) diff);

          diff = *dmap++;
          vw = wasm_v32x4_load_splat(w); w += 1;
          vi0123 = wasm_v128_load(input + 0);
          vi4567 = wasm_v128_load(input + 4);
        } while (--nnz != 0);
      }
      v128_t vout0123 = wasm_v128_bitselect(vacc0123, vmax, wasm_f32x4_le(vacc0123, vmax));
      v128_t vout4567 = wasm_v128_bitselect(vacc4567, vmax, wasm_f32x4_le(vacc4567, vmax));
      vout0123 = wasm_v128_bitselect(vmin, vout0123, wasm_f32x4_lt(vout0123, vmin));
      vout4567 = wasm_v128_bitselect(vmin, vout4567, wasm_f32x4_lt(vout4567, vmin));
      wasm_v128_store(output, vout0123);
      wasm_v128_store(output + 4, vout4567);
      output = (float*restrict) ((uintptr_t) output + batch_size);
    } while (--c != 0);
    output = (float*restrict) ((uintptr_t) output - output_decrement);
    input += 8;
    n -= 8 * sizeof(float);
  }
  if XNN_UNLIKELY(n != 0) {
    output_decrement += 4 * sizeof(float);
    if (n & (4 * sizeof(float))) {
      const float*restrict w = weights;
      const int32_t* dmap = widx_dmap;
      const uint32_t* nnzmap = nidx_nnzmap;
      size_t c = output_channels;
      do {
        uint32_t nnz = *nnzmap++;
        v128_t vacc0123 = wasm_v32x4_load_splat(w); w += 1;
        if XNN_LIKELY(nnz != 0) {
          do {
            const intptr_t diff = *dmap++;
            const v128_t vi0123 = wasm_v128_load(input);
            input = (const float*restrict) ((uintptr_t) input + (uintptr_t) diff);
            const v128_t vw = wasm_v32x4_load_splat(w); w += 1;
            vacc0123 = wasm_f32x4_add(vacc0123, wasm_f32x4_mul(vi0123, vw));
          } while (--nnz != 0);
        }
        v128_t vout0123 = wasm_v128_bitselect(vacc0123, vmax, wasm_f32x4_le(vacc0123, vmax));
        vout0123 = wasm_v128_bitselect(vmin, vout0123, wasm_f32x4_lt(vout0123, vmin));
        wasm_v128_store(output, vout0123);

        output = (float*restrict) ((uintptr_t) output + batch_size);
      } while (--c != 0);
      output = (float*restrict) ((uintptr_t) output - output_decrement);
      input += 4;
    }
    output_decrement += 2 * sizeof(float);
    if (n & (2 * sizeof(float))) {
      const float*restrict w = weights;
      const int32_t* dmap = widx_dmap;
      const uint32_t* nnzmap = nidx_nnzmap;
      size_t c = output_channels;
      do {
        uint32_t nnz = *nnzmap++;
        v128_t vacc01 = wasm_v32x4_load_splat(w); w += 1;
        if XNN_LIKELY(nnz != 0) {
          do {
            const intptr_t diff = *dmap++;
            const v128_t vi01 = wasm_v64x2_load_splat(input);
            input = (const float*restrict) ((uintptr_t) input + (uintptr_t) diff);
            const v128_t vw = wasm_v32x4_load_splat(w); w += 1;
            vacc01 = wasm_f32x4_add(vacc01, wasm_f32x4_mul(vi01, vw));
          } while (--nnz != 0);
        }
        v128_t vout01 = wasm_v128_bitselect(vacc01, vmax, wasm_f32x4_le(vacc01, vmax));
        vout01 = wasm_v128_bitselect(vmin, vout01, wasm_f32x4_lt(vout01, vmin));
        *((double*) output) = wasm_f64x2_extract_lane(vout01, 0);

        output = (float*restrict) ((uintptr_t) output + batch_size);
      } while (--c != 0);
      output = (float*restrict) ((uintptr_t) output - output_decrement);
      input += 2;
    }
    output_decrement += 1 * sizeof(float);
    if (n & (1 * sizeof(float))) {
      const float*restrict w = weights;
      const int32_t* dmap = widx_dmap;
      const uint32_t* nnzmap = nidx_nnzmap;
      size_t c = output_channels;
      do {
        uint32_t nnz = *nnzmap++;
        v128_t vacc0 = wasm_v32x4_load_splat(w); w += 1;
        if XNN_LIKELY(nnz != 0) {
          do {
            const intptr_t diff = *dmap++;
            const v128_t vi0 = wasm_v32x4_load_splat(input);
            input = (const float*restrict) ((uintptr_t) input + (uintptr_t) diff);
            const v128_t vw = wasm_v32x4_load_splat(w); w += 1;
            vacc0 = wasm_f32x4_add(vacc0, wasm_f32x4_mul(vi0, vw));
          } while (--nnz != 0);
        }
        v128_t vout0 = wasm_v128_bitselect(vacc0, vmax, wasm_f32x4_le(vacc0, vmax));
        vout0 = wasm_v128_bitselect(vmin, vout0, wasm_f32x4_lt(vout0, vmin));
        *output = wasm_f32x4_extract_lane(vout0, 0);

        output = (float*restrict) ((uintptr_t) output + batch_size);
      } while (--c != 0);
      output = (float*restrict) ((uintptr_t) output - output_decrement);
      input += 1;
    }
  }
}