/******************************************************************************
 * Copyright (c) 2011, Duane Merrill.  All rights reserved.
 * Copyright (c) 2011-2018, NVIDIA CORPORATION.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the NVIDIA CORPORATION nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NVIDIA CORPORATION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

/**
 * @file
 * The cub::BlockShuffle class provides [<em>collective</em>](index.html#sec0) methods for shuffling
 * data partitioned across a CUDA thread block.
 */

#pragma once

#include "../config.cuh"

#if defined(_CCCL_COMPILER_NVHPC) && defined(_CCCL_USE_IMPLICIT_SYSTEM_DEADER)
#pragma GCC system_header
#else // ^^^ _CCCL_COMPILER_NVHPC ^^^ / vvv !_CCCL_COMPILER_NVHPC vvv
_CCCL_IMPLICIT_SYSTEM_HEADER
#endif // !_CCCL_COMPILER_NVHPC

#include "../util_ptx.cuh"
#include "../util_type.cuh"

CUB_NAMESPACE_BEGIN

/**
 * @brief The BlockShuffle class provides [<em>collective</em>](index.html#sec0) 
 *        methods for shuffling data partitioned across a CUDA thread block.
 *
 * @ingroup BlockModule
 *
 * @tparam T                    
 *   The data type to be exchanged.
 *
 * @tparam BLOCK_DIM_X          
 *   The thread block length in threads along the X dimension
 *
 * @tparam BLOCK_DIM_Y          
 *   <b>[optional]</b> The thread block length in threads along the Y dimension (default: 1)
 *
 * @tparam BLOCK_DIM_Z          
 *   <b>[optional]</b> The thread block length in threads along the Z dimension (default: 1)
 *
 * @tparam LEGACY_PTX_ARCH      
 *   <b>[optional]</b> Unused.
 *
 * @par Overview
 * It is commonplace for blocks of threads to rearrange data items between
 * threads.  The BlockShuffle abstraction allows threads to efficiently shift items
 * either (a) up to their successor or (b) down to their predecessor.
 *
 */
template <
    typename            T,
    int                 BLOCK_DIM_X,
    int                 BLOCK_DIM_Y         = 1,
    int                 BLOCK_DIM_Z         = 1,
    int                 LEGACY_PTX_ARCH     = 0>
class BlockShuffle
{
private:

    /******************************************************************************
     * Constants
     ******************************************************************************/

    enum
    {
        BLOCK_THREADS               = BLOCK_DIM_X * BLOCK_DIM_Y * BLOCK_DIM_Z,

        LOG_WARP_THREADS            = CUB_LOG_WARP_THREADS(0),
        WARP_THREADS                = 1 << LOG_WARP_THREADS,
        WARPS                       = (BLOCK_THREADS + WARP_THREADS - 1) / WARP_THREADS,
    };

    /******************************************************************************
     * Type definitions
     ******************************************************************************/

    /// Shared memory storage layout type (last element from each thread's input)
    typedef T _TempStorage[BLOCK_THREADS];


public:

    /// \smemstorage{BlockShuffle}
    struct TempStorage : Uninitialized<_TempStorage> {};

private:


    /******************************************************************************
     * Thread fields
     ******************************************************************************/

    /// Shared storage reference
    _TempStorage &temp_storage;

    /// Linear thread-id
    unsigned int linear_tid;


    /******************************************************************************
     * Utility methods
     ******************************************************************************/

    /// Internal storage allocator
    __device__ __forceinline__ _TempStorage& PrivateStorage()
    {
        __shared__ _TempStorage private_storage;
        return private_storage;
    }


public:

    /******************************************************************//**
     * @name Collective constructors
     *********************************************************************/
    //@{

    /**
     * @brief Collective constructor using a private static allocation of 
     *        shared memory as temporary storage.
     */
    __device__ __forceinline__ BlockShuffle()
    :
        temp_storage(PrivateStorage()),
        linear_tid(RowMajorTid(BLOCK_DIM_X, BLOCK_DIM_Y, BLOCK_DIM_Z))
    {}

    /**
     * @brief Collective constructor using the specified memory allocation
     *        as temporary storage.
     *
     * @param[in] temp_storage
     *   Reference to memory allocation having layout type TempStorage
     */
    __device__ __forceinline__ BlockShuffle(TempStorage &temp_storage)
        : temp_storage(temp_storage.Alias())
        , linear_tid(RowMajorTid(BLOCK_DIM_X, BLOCK_DIM_Y, BLOCK_DIM_Z))
    {}


    //@}  end member group
    /******************************************************************//**
     * @name Shuffle movement
     *********************************************************************/
    //@{

    /**
     * @brief Each <em>thread<sub>i</sub></em> obtains the @p input provided by
     *        <em>thread</em><sub><em>i</em>+<tt>distance</tt></sub>.
     *        The offset @p distance may be negative.
     *
     * @par
     * - @smemreuse
     *
     * @param[in] input
     *   The input item from the calling thread (<em>thread<sub>i</sub></em>)
     *
     * @param[out] output
     *   The @p input item from the successor (or predecessor) thread
     *   <em>thread</em><sub><em>i</em>+<tt>distance</tt></sub> (may be aliased to @p input).
     *   This value is only updated for for <em>thread<sub>i</sub></em> when
     *   0 <= (<em>i</em> + \p distance) < <tt>BLOCK_THREADS-1</tt>
     *
     * @param[in] distance
     *   Offset distance (may be negative)
     */
    __device__ __forceinline__ void Offset(T input, T &output, int distance = 1)
    {
        temp_storage[linear_tid] = input;

        CTA_SYNC();

        const int offset_tid = static_cast<int>(linear_tid) + distance;
        if ((offset_tid >= 0) && (offset_tid < BLOCK_THREADS))
        {
            output = temp_storage[static_cast<size_t>(offset_tid)];
        }
    }

    /**
     * @brief Each <em>thread<sub>i</sub></em> obtains the @p input
     *        provided by <em>thread</em><sub><em>i</em>+<tt>distance</tt></sub>.
     *
     * @par
     * - @smemreuse
     *
     * @param[in] input
     *   The calling thread's input item
     *
     * @param[out] output
     *   The @p input item from thread
     *   <em>thread</em><sub>(<em>i</em>+<tt>distance></tt>)%<tt>BLOCK_THREADS</tt></sub>
     *   (may be aliased to @p input). This value is not updated for
     *   <em>thread</em><sub>BLOCK_THREADS-1</sub>
     *
     * @param[in] distance
     *   Offset distance (0 < @p distance < <tt>BLOCK_THREADS</tt>)
     */
    __device__ __forceinline__ void Rotate(T input, T &output, unsigned int distance = 1)
    {
        temp_storage[linear_tid] = input;

        CTA_SYNC();

        unsigned int offset = linear_tid + distance;
        if (offset >= BLOCK_THREADS)
            offset -= BLOCK_THREADS;

        output = temp_storage[offset];
    }

    /**
     * @brief The thread block rotates its
     *        [<em>blocked arrangement</em>](index.html#sec5sec3) of
     *        @p input items, shifting it up by one item.
     *
     * @par
     * - @blocked
     * - @granularity
     * - @smemreuse
     *
     * @param[in] input
     *   The calling thread's input items
     *
     * @param[out] prev
     *   The corresponding predecessor items (may be aliased to @p input).
     *   The item @p prev[0] is not updated for <em>thread</em><sub>0</sub>.
     */
    template <int ITEMS_PER_THREAD>
    __device__ __forceinline__ void Up(T (&input)[ITEMS_PER_THREAD], T (&prev)[ITEMS_PER_THREAD])
    {
        temp_storage[linear_tid] = input[ITEMS_PER_THREAD - 1];

        CTA_SYNC();

        #pragma unroll
        for (int ITEM = ITEMS_PER_THREAD - 1; ITEM > 0; --ITEM)
            prev[ITEM] = input[ITEM - 1];

        if (linear_tid > 0)
            prev[0] = temp_storage[linear_tid - 1];
    }


    /**
     * \brief The thread block rotates its [<em>blocked arrangement</em>](index.html#sec5sec3) of \p input items, shifting it up by one item.  All threads receive the \p input provided by <em>thread</em><sub><tt>BLOCK_THREADS-1</tt></sub>.
     *
     * \par
     * - \blocked
     * - \granularity
     * - \smemreuse
     */
    template <int ITEMS_PER_THREAD>
    __device__ __forceinline__ void Up(
        T (&input)[ITEMS_PER_THREAD],   ///< [in] The calling thread's input items
        T (&prev)[ITEMS_PER_THREAD],    ///< [out] The corresponding predecessor items (may be aliased to \p input).  The item \p prev[0] is not updated for <em>thread</em><sub>0</sub>.
        T &block_suffix)                ///< [out] The item \p input[ITEMS_PER_THREAD-1] from <em>thread</em><sub><tt>BLOCK_THREADS-1</tt></sub>, provided to all threads
    {
        Up(input, prev);
        block_suffix = temp_storage[BLOCK_THREADS - 1];
    }

    /**
     * @brief The thread block rotates its
     *        [<em>blocked arrangement</em>](index.html#sec5sec3) of
     *        @p input items, shifting it down by one item.
     *
     * @par
     * - @blocked
     * - @granularity
     * - @smemreuse
     *
     * @param[in] input
     *   The calling thread's input items
     *
     * @param[out] prev
     *   The corresponding predecessor items (may be aliased to @p input).
     *   The value @p prev[0] is not updated for <em>thread</em><sub>BLOCK_THREADS-1</sub>.
     */
    template <int ITEMS_PER_THREAD>
    __device__ __forceinline__ void Down(T (&input)[ITEMS_PER_THREAD], T (&prev)[ITEMS_PER_THREAD])
    {
        temp_storage[linear_tid] = input[0];

        CTA_SYNC();

        #pragma unroll
        for (int ITEM = 0; ITEM < ITEMS_PER_THREAD - 1; ITEM++)
            prev[ITEM] = input[ITEM + 1];

        if (linear_tid < BLOCK_THREADS - 1)
            prev[ITEMS_PER_THREAD - 1] = temp_storage[linear_tid + 1];
    }

    /**
     * @brief The thread block rotates its
     *        [<em>blocked arrangement</em>](index.html#sec5sec3) of input items,
     *        shifting it down by one item. All threads receive @p input[0]
     *        provided by <em>thread</em><sub><tt>0</tt></sub>.
     *
     * @par
     * - @blocked
     * - @granularity
     * - @smemreuse
     *
     * @param[in] input
     *   The calling thread's input items
     *
     * @param[out] prev
     *   The corresponding predecessor items (may be aliased to @p input).
     *   The value @p prev[0] is not updated for <em>thread</em><sub>BLOCK_THREADS-1</sub>.
     *
     * @param[out] block_prefix
     *   The item @p input[0] from <em>thread</em><sub><tt>0</tt></sub>, provided to all threads
     */
    template <int ITEMS_PER_THREAD>
    __device__ __forceinline__ void Down(T (&input)[ITEMS_PER_THREAD],
                                         T (&prev)[ITEMS_PER_THREAD],
                                         T &block_prefix)
    {
        Down(input, prev);
        block_prefix = temp_storage[0];
    }

    //@}  end member group


};

CUB_NAMESPACE_END

