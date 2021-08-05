/******************************************************************************
 * @file     matrix_functions.h
 * @brief    Public header file for CMSIS DSP Library
 * @version  V1.9.0
 * @date     23 April 2021
 * Target Processor: Cortex-M and Cortex-A cores
 ******************************************************************************/
/*
 * Copyright (c) 2010-2020 Arm Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _WW_MATRIX_FUNCTIONS_H_
#define _WW_MATRIX_FUNCTIONS_H_

#include "math.h"
#include "arm_math.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define ARM_MATH_DECOMPOSITION_FAILURE (arm_status)(-7) /**< Decomposition Failed */

    /**
   * @brief Floating-point Cholesky decomposition of Symmetric Positive Definite Matrix.
   * @param[in]  src   points to the instance of the input floating-point matrix structure.
   * @param[out] dst   points to the instance of the output floating-point matrix structure.
   * @return The function returns ARM_MATH_SIZE_MISMATCH, if the dimensions do not match.
   * If the input matrix does not have a decomposition, then the algorithm terminates and returns error status ARM_MATH_DECOMPOSITION_FAILURE.
   * If the matrix is ill conditioned or only semi-definite, then it is better using the LDL^t decomposition.
   * The decomposition is returning a lower triangular matrix.
   */
    arm_status
    arm_mat_cholesky_f64(
        const arm_matrix_instance_f64 *src,
        arm_matrix_instance_f64 *dst);

    /**
   * @brief Floating-point Cholesky decomposition of Symmetric Positive Definite Matrix.
   * @param[in]  src   points to the instance of the input floating-point matrix structure.
   * @param[out] dst   points to the instance of the output floating-point matrix structure.
   * @return The function returns ARM_MATH_SIZE_MISMATCH, if the dimensions do not match.
   * If the input matrix does not have a decomposition, then the algorithm terminates and returns error status ARM_MATH_DECOMPOSITION_FAILURE.
   * If the matrix is ill conditioned or only semi-definite, then it is better using the LDL^t decomposition.
   * The decomposition is returning a lower triangular matrix.
   */
    arm_status arm_mat_cholesky_f32(
        const arm_matrix_instance_f32 *src,
        arm_matrix_instance_f32 *dst);

    /**
   * @brief Solve UT . X = A where UT is an upper triangular matrix
   * @param[in]  ut  The upper triangular matrix
   * @param[in]  a  The matrix a
   * @param[out] dst The solution X of UT . X = A
   * @return The function returns ARM_MATH_SINGULAR, if the system can't be solved.
  */
    arm_status arm_mat_solve_upper_triangular_f32(
        const arm_matrix_instance_f32 *ut,
        const arm_matrix_instance_f32 *a,
        arm_matrix_instance_f32 *dst);

    /**
   * @brief Solve LT . X = A where LT is a lower triangular matrix
   * @param[in]  lt  The lower triangular matrix
   * @param[in]  a  The matrix a
   * @param[out] dst The solution X of LT . X = A
   * @return The function returns ARM_MATH_SINGULAR, if the system can't be solved.
   */
    arm_status arm_mat_solve_lower_triangular_f32(
        const arm_matrix_instance_f32 *lt,
        const arm_matrix_instance_f32 *a,
        arm_matrix_instance_f32 *dst);

    /**
   * @brief Solve UT . X = A where UT is an upper triangular matrix
   * @param[in]  ut  The upper triangular matrix
   * @param[in]  a  The matrix a
   * @param[out] dst The solution X of UT . X = A
   * @return The function returns ARM_MATH_SINGULAR, if the system can't be solved.
  */
    arm_status arm_mat_solve_upper_triangular_f64(
        const arm_matrix_instance_f64 *ut,
        const arm_matrix_instance_f64 *a,
        arm_matrix_instance_f64 *dst);

    /**
   * @brief Solve LT . X = A where LT is a lower triangular matrix
   * @param[in]  lt  The lower triangular matrix
   * @param[in]  a  The matrix a
   * @param[out] dst The solution X of LT . X = A
   * @return The function returns ARM_MATH_SINGULAR, if the system can't be solved.
   */
    arm_status arm_mat_solve_lower_triangular_f64(
        const arm_matrix_instance_f64 *lt,
        const arm_matrix_instance_f64 *a,
        arm_matrix_instance_f64 *dst);

    /**
   * @brief Floating-point LDL decomposition of Symmetric Positive Semi-Definite Matrix.
   * @param[in]  src   points to the instance of the input floating-point matrix structure.
   * @param[out] l   points to the instance of the output floating-point triangular matrix structure.
   * @param[out] d   points to the instance of the output floating-point diagonal matrix structure.
   * @param[out] p   points to the instance of the output floating-point permutation vector.
   * @return The function returns ARM_MATH_SIZE_MISMATCH, if the dimensions do not match.
   * If the input matrix does not have a decomposition, then the algorithm terminates and returns error status ARM_MATH_DECOMPOSITION_FAILURE.
   * The decomposition is returning a lower triangular matrix.
   */
    arm_status arm_mat_ldlt_f32(
        const arm_matrix_instance_f32 *src,
        arm_matrix_instance_f32 *l,
        arm_matrix_instance_f32 *d,
        uint16_t *pp);

    /**
   * @brief Floating-point LDL decomposition of Symmetric Positive Semi-Definite Matrix.
   * @param[in]  src   points to the instance of the input floating-point matrix structure.
   * @param[out] l   points to the instance of the output floating-point triangular matrix structure.
   * @param[out] d   points to the instance of the output floating-point diagonal matrix structure.
   * @param[out] p   points to the instance of the output floating-point permutation vector.
   * @return The function returns ARM_MATH_SIZE_MISMATCH, if the dimensions do not match.
   * If the input matrix does not have a decomposition, then the algorithm terminates and returns error status ARM_MATH_DECOMPOSITION_FAILURE.
   * The decomposition is returning a lower triangular matrix.
   */
    arm_status arm_mat_ldlt_f64(
        const arm_matrix_instance_f64 *src,
        arm_matrix_instance_f64 *l,
        arm_matrix_instance_f64 *d,
        uint16_t *pp);

#ifdef __cplusplus
}
#endif

#endif /* ifndef _WW_MATRIX_FUNCTIONS_H_ */
