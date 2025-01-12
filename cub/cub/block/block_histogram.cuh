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
 * The cub::BlockHistogram class provides [<em>collective</em>](index.html#sec0) methods for
 * constructing block-wide histograms from data samples partitioned across a CUDA thread block.
 */

#pragma once

#include "../config.cuh"

#if defined(_CCCL_COMPILER_NVHPC) && defined(_CCCL_USE_IMPLICIT_SYSTEM_DEADER)
#pragma GCC system_header
#else // ^^^ _CCCL_COMPILER_NVHPC ^^^ / vvv !_CCCL_COMPILER_NVHPC vvv
_CCCL_IMPLICIT_SYSTEM_HEADER
#endif // !_CCCL_COMPILER_NVHPC

#include "specializations/block_histogram_sort.cuh"
#include "specializations/block_histogram_atomic.cuh"
#include "../util_ptx.cuh"

CUB_NAMESPACE_BEGIN


/******************************************************************************
 * Algorithmic variants
 ******************************************************************************/

/**
 * @brief BlockHistogramAlgorithm enumerates alternative algorithms for the parallel construction of
 *        block-wide histograms.
 */
enum BlockHistogramAlgorithm
{

    /**
     * @par Overview
     * Sorting followed by differentiation.  Execution is comprised of two phases:
     * -# Sort the data using efficient radix sort
     * -# Look for "runs" of same-valued keys by detecting discontinuities; the run-lengths are histogram bin counts.
     *
     * @par Performance Considerations
     * Delivers consistent throughput regardless of sample bin distribution.
     */
    BLOCK_HISTO_SORT,


    /**
     * @par Overview
     * Use atomic addition to update byte counts directly
     *
     * @par Performance Considerations
     * Performance is strongly tied to the hardware implementation of atomic
     * addition, and may be significantly degraded for non uniformly-random
     * input distributions where many concurrent updates are likely to be
     * made to the same bin counter.
     */
    BLOCK_HISTO_ATOMIC,
};



/******************************************************************************
 * Block histogram
 ******************************************************************************/

/**
 * @brief The BlockHistogram class provides [<em>collective</em>](index.html#sec0) methods for
 *        constructing block-wide histograms from data samples partitioned across a CUDA thread
 *        block. ![](histogram_logo.png)
 *
 * @ingroup BlockModule
 *
 * @tparam T
 *   The sample type being histogrammed (must be castable to an integer bin identifier)
 *
 * @tparam BLOCK_DIM_X
 *   The thread block length in threads along the X dimension
 *
 * @tparam ITEMS_PER_THREAD
 *   The number of items per thread
 *
 * @tparam BINS
 *   The number bins within the histogram
 *
 * @tparam ALGORITHM
 *   <b>[optional]</b> cub::BlockHistogramAlgorithm enumerator specifying the underlying algorithm
 *   to use (default: cub::BLOCK_HISTO_SORT)
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
 * - A <a href="http://en.wikipedia.org/wiki/Histogram"><em>histogram</em></a>
 *   counts the number of observations that fall into each of the disjoint categories (known as
 *   <em>bins</em>).
 * - The `T` type must be implicitly castable to an integer type.
 * - BlockHistogram expects each integral `input[i]` value to satisfy
 *   `0 <= input[i] < BINS`. Values outside of this range result in undefined
 *   behavior.
 * - BlockHistogram can be optionally specialized to use different algorithms:
 *   -# <b>cub::BLOCK_HISTO_SORT</b>.  Sorting followed by differentiation. [More...](\ref
 *      cub::BlockHistogramAlgorithm)
 *   -# <b>cub::BLOCK_HISTO_ATOMIC</b>.  Use atomic addition to update byte counts directly.
 *      [More...](\ref cub::BlockHistogramAlgorithm)
 *
 * @par Performance Considerations
 * - @granularity
 *
 * @par A Simple Example
 * @blockcollective{BlockHistogram}
 * @par
 * The code snippet below illustrates a 256-bin histogram of 512 integer samples that
 * are partitioned across 128 threads where each thread owns 4 samples.
 * @par
 * @code
 * #include <cub/cub.cuh>   // or equivalently <cub/block/block_histogram.cuh>
 *
 * __global__ void ExampleKernel(...)
 * {
 *     // Specialize a 256-bin BlockHistogram type for a 1D block of 128 threads having 4 character
 * samples each typedef cub::BlockHistogram<unsigned char, 128, 4, 256> BlockHistogram;
 *
 *     // Allocate shared memory for BlockHistogram
 *     __shared__ typename BlockHistogram::TempStorage temp_storage;
 *
 *     // Allocate shared memory for block-wide histogram bin counts
 *     __shared__ unsigned int smem_histogram[256];
 *
 *     // Obtain input samples per thread
 *     unsigned char data[4];
 *     ...
 *
 *     // Compute the block-wide histogram
 *     BlockHistogram(temp_storage).Histogram(data, smem_histogram);
 *
 * @endcode
 *
 * @par Performance and Usage Considerations
 * - All input values must fall between [0, BINS), or behavior is undefined.
 * - The histogram output can be constructed in shared or device-accessible memory
 * - See cub::BlockHistogramAlgorithm for performance details regarding algorithmic alternatives
 *
 * @par Re-using dynamically allocating shared memory
 * The following example under the examples/block folder illustrates usage of
 * dynamically shared memory with BlockReduce and how to re-purpose
 * the same memory region:
 * <a
 * href="../../examples/block/example_block_reduce_dyn_smem.cu">example_block_reduce_dyn_smem.cu</a>
 *
 * This example can be easily adapted to the storage required by BlockHistogram.
 */
template <
    typename                T,
    int                     BLOCK_DIM_X,
    int                     ITEMS_PER_THREAD,
    int                     BINS,
    BlockHistogramAlgorithm ALGORITHM           = BLOCK_HISTO_SORT,
    int                     BLOCK_DIM_Y         = 1,
    int                     BLOCK_DIM_Z         = 1,
    int                     LEGACY_PTX_ARCH     = 0>
class BlockHistogram
{
private:

    /******************************************************************************
     * Constants and type definitions
     ******************************************************************************/

    /// Constants
    enum
    {
        /// The thread block size in threads
        BLOCK_THREADS = BLOCK_DIM_X * BLOCK_DIM_Y * BLOCK_DIM_Z,
    };

    /// Internal specialization.
    using InternalBlockHistogram =
      cub::detail::conditional_t<ALGORITHM == BLOCK_HISTO_SORT,
                                 BlockHistogramSort<T,
                                                    BLOCK_DIM_X,
                                                    ITEMS_PER_THREAD,
                                                    BINS,
                                                    BLOCK_DIM_Y,
                                                    BLOCK_DIM_Z>,
                                 BlockHistogramAtomic<BINS>>;

    /// Shared memory storage layout type for BlockHistogram
    typedef typename InternalBlockHistogram::TempStorage _TempStorage;


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

    /// @smemstorage{BlockHistogram}
    struct TempStorage : Uninitialized<_TempStorage> {};


    /******************************************************************//**
     * @name Collective constructors
     *********************************************************************/
    //@{

    /**
     * @brief Collective constructor using a private static allocation of shared memory as temporary storage.
     */
    __device__ __forceinline__ BlockHistogram()
    :
        temp_storage(PrivateStorage()),
        linear_tid(RowMajorTid(BLOCK_DIM_X, BLOCK_DIM_Y, BLOCK_DIM_Z))
    {}

    /**
     * @brief Collective constructor using the specified memory allocation as temporary storage.
     *
     * @param[in] temp_storage
     *   Reference to memory allocation having layout type TempStorage
     */
    __device__ __forceinline__ BlockHistogram(TempStorage &temp_storage)
        : temp_storage(temp_storage.Alias())
        , linear_tid(RowMajorTid(BLOCK_DIM_X, BLOCK_DIM_Y, BLOCK_DIM_Z))
    {}


    //@}  end member group
    /******************************************************************//**
     * @name Histogram operations
     *********************************************************************/
    //@{


    /**
     * @brief Initialize the shared histogram counters to zero.
     *
     * @par Snippet
     * The code snippet below illustrates a the initialization and update of a
     * histogram of 512 integer samples that are partitioned across 128 threads
     * where each thread owns 4 samples.
     * @par
     * @code
     * #include <cub/cub.cuh>   // or equivalently <cub/block/block_histogram.cuh>
     *
     * __global__ void ExampleKernel(...)
     * {
     *     // Specialize a 256-bin BlockHistogram type for a 1D block of 128 threads having 4 character samples each
     *     typedef cub::BlockHistogram<unsigned char, 128, 4, 256> BlockHistogram;
     *
     *     // Allocate shared memory for BlockHistogram
     *     __shared__ typename BlockHistogram::TempStorage temp_storage;
     *
     *     // Allocate shared memory for block-wide histogram bin counts
     *     __shared__ unsigned int smem_histogram[256];
     *
     *     // Obtain input samples per thread
     *     unsigned char thread_samples[4];
     *     ...
     *
     *     // Initialize the block-wide histogram
     *     BlockHistogram(temp_storage).InitHistogram(smem_histogram);
     *
     *     // Update the block-wide histogram
     *     BlockHistogram(temp_storage).Composite(thread_samples, smem_histogram);
     *
     * @endcode
     *
     * @tparam CounterT              
     *   <b>[inferred]</b> Histogram counter type
     */
    template <typename CounterT>
    __device__ __forceinline__ void InitHistogram(CounterT histogram[BINS])
    {
        // Initialize histogram bin counts to zeros
        int histo_offset = 0;

        #pragma unroll
        for(; histo_offset + BLOCK_THREADS <= BINS; histo_offset += BLOCK_THREADS)
        {
            histogram[histo_offset + linear_tid] = 0;
        }
        // Finish up with guarded initialization if necessary
        if ((BINS % BLOCK_THREADS != 0) && (histo_offset + linear_tid < BINS))
        {
            histogram[histo_offset + linear_tid] = 0;
        }
    }

    /**
     * @brief Constructs a block-wide histogram in shared/device-accessible memory.  
     *        Each thread contributes an array of input elements.
     *
     * @par
     * - @granularity
     * - @smemreuse
     *
     * @par Snippet
     * The code snippet below illustrates a 256-bin histogram of 512 integer samples that
     * are partitioned across 128 threads where each thread owns 4 samples.
     * @par
     * @code
     * #include <cub/cub.cuh>   // or equivalently <cub/block/block_histogram.cuh>
     *
     * __global__ void ExampleKernel(...)
     * {
     *     // Specialize a 256-bin BlockHistogram type for a 1D block of 128 threads having 4
     * character samples each typedef cub::BlockHistogram<unsigned char, 128, 4, 256>
     * BlockHistogram;
     *
     *     // Allocate shared memory for BlockHistogram
     *     __shared__ typename BlockHistogram::TempStorage temp_storage;
     *
     *     // Allocate shared memory for block-wide histogram bin counts
     *     __shared__ unsigned int smem_histogram[256];
     *
     *     // Obtain input samples per thread
     *     unsigned char thread_samples[4];
     *     ...
     *
     *     // Compute the block-wide histogram
     *     BlockHistogram(temp_storage).Histogram(thread_samples, smem_histogram);
     *
     * @endcode
     *
     * @tparam CounterT
     *   <b>[inferred]</b> Histogram counter type
     *
     * @param[in] items
     *   Calling thread's input values to histogram
     *
     * @param[out] histogram
     *   Reference to shared/device-accessible memory histogram
     */
    template <typename CounterT>
    __device__ __forceinline__ void Histogram(T (&items)[ITEMS_PER_THREAD],
                                              CounterT histogram[BINS])
    {
        // Initialize histogram bin counts to zeros
        InitHistogram(histogram);

        CTA_SYNC();

        // Composite the histogram
        InternalBlockHistogram(temp_storage).Composite(items, histogram);
    }

    /**
     * @brief Updates an existing block-wide histogram in shared/device-accessible memory.
     *        Each thread composites an array of input elements.
     *
     * @par
     * - @granularity
     * - @smemreuse
     *
     * @par Snippet
     * The code snippet below illustrates a the initialization and update of a
     * histogram of 512 integer samples that are partitioned across 128 threads
     * where each thread owns 4 samples.
     * @par
     * @code
     * #include <cub/cub.cuh>   // or equivalently <cub/block/block_histogram.cuh>
     *
     * __global__ void ExampleKernel(...)
     * {
     *     // Specialize a 256-bin BlockHistogram type for a 1D block of 128 threads having 4
     * character samples each typedef cub::BlockHistogram<unsigned char, 128, 4, 256>
     * BlockHistogram;
     *
     *     // Allocate shared memory for BlockHistogram
     *     __shared__ typename BlockHistogram::TempStorage temp_storage;
     *
     *     // Allocate shared memory for block-wide histogram bin counts
     *     __shared__ unsigned int smem_histogram[256];
     *
     *     // Obtain input samples per thread
     *     unsigned char thread_samples[4];
     *     ...
     *
     *     // Initialize the block-wide histogram
     *     BlockHistogram(temp_storage).InitHistogram(smem_histogram);
     *
     *     // Update the block-wide histogram
     *     BlockHistogram(temp_storage).Composite(thread_samples, smem_histogram);
     *
     * @endcode
     *
     * @tparam CounterT
     *   <b>[inferred]</b> Histogram counter type
     *
     * @param[in] items
     *   Calling thread's input values to histogram
     *
     * @param[out] histogram
     *   Reference to shared/device-accessible memory histogram
     */
    template <typename CounterT>
    __device__ __forceinline__ void Composite(T (&items)[ITEMS_PER_THREAD],
                                              CounterT histogram[BINS])
    {
        InternalBlockHistogram(temp_storage).Composite(items, histogram);
    }

};

CUB_NAMESPACE_END

