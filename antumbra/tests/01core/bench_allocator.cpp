
// #include "framework/01core/memory/memory.h"
// #include "framework/01core/utils/utils.h"
// #include <catch2/catch_test_macros.hpp>
// #include <iostream>

// #include "framework/01core/memory/allocators/linear_allocator.h"
// #include "framework/01core/memory/allocators/pool_allocator.h"
// #include "framework/01core/memory/allocators/stack_allocator.h"

// using namespace ant::memory;
// using namespace ant;

// const u64 A = static_cast<u64>(1e9);
// const u64 B = static_cast<u64>(1e8);

// const std::vector<u64> ALLOCATION_SIZES{32, 64, 256, 512, 1024, 2048, 4096};
// const std::vector<u64> ALIGNMENTS{8, 8, 8, 8, 8, 8, 8};

// #if 0
// #define OPERATIONS (m_nOperations)
// #else
// #define OPERATIONS (10)
// #endif

// struct BenchmarkResults {
//     std::size_t Operations;
//     std::chrono::milliseconds Milliseconds;
//     double OperationsPerSec;
//     double TimePerOperation;
//     std::size_t MemoryPeak;
// };

// class Benchmark {
//   public:
//     Benchmark() = delete;

//     Benchmark(const unsigned int nOperations) : m_nOperations{nOperations} {}

//     void SingleAllocation(allocator_base *allocator, const std::size_t size, const std::size_t alignment);
//     void SingleFree(allocator_base *allocator, const std::size_t size, const std::size_t alignment);

//     void MultipleAllocation(allocator_base *allocator, const std::vector<std::size_t> &allocationSizes,
//                             const std::vector<std::size_t> &alignments);
//     void MultipleFree(allocator_base *allocator, const std::vector<std::size_t> &allocationSizes,
//                       const std::vector<std::size_t> &alignments);

//     void RandomAllocation(allocator_base *allocator, const std::vector<std::size_t> &allocationSizes,
//                           const std::vector<std::size_t> &alignments);
//     void RandomFree(allocator_base *allocator, const std::vector<std::size_t> &allocationSizes,
//                     const std::vector<std::size_t> &alignments);

//   private:
//     void PrintResults(const BenchmarkResults &results) const;

//     void RandomAllocationAttr(const std::vector<std::size_t> &allocationSizes,
//                               const std::vector<std::size_t> &alignments, std::size_t &size, std::size_t &alignment);

//     const BenchmarkResults buildResults(std::size_t nOperations, std::chrono::milliseconds &&ellapsedTime,
//                                         const std::size_t memoryUsed) const;

//     void SetStartTime() noexcept { Start = std::chrono::high_resolution_clock::now(); }

//     void SetFinishTime() noexcept { Finish = std::chrono::high_resolution_clock::now(); }

//     void SetElapsedTime() noexcept {
//         TimeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(Finish - Start);
//     }

//     void StartRound() noexcept { SetStartTime(); }

//     void FinishRound() noexcept {
//         SetFinishTime();
//         SetElapsedTime();
//     }

//   private:
//     std::size_t m_nOperations;

//     std::chrono::time_point<std::chrono::high_resolution_clock> Start;
//     std::chrono::time_point<std::chrono::high_resolution_clock> Finish;

//     std::chrono::milliseconds TimeElapsed;
// };
// void Benchmark::SingleAllocation(allocator_base *allocator, const std::size_t size, const std::size_t alignment) {
//     std::cout << "BENCHMARK: ALLOCATION" << std::endl;
//     std::cout << "\tSize:     \t" << size << std::endl;
//     std::cout << "\tAlignment\t" << alignment << std::endl;

//     StartRound();

//     //allocator->Init();

//     auto operations = 0u;

//     while (operations < m_nOperations) {
//         allocator->allocate(size, alignment);
//         ++operations;
//     }

//     FinishRound();

//     BenchmarkResults results = buildResults(m_nOperations, std::move(TimeElapsed), 0);

//     PrintResults(results);
// }

// void Benchmark::SingleFree(allocator_base *allocator, const std::size_t size, const std::size_t alignment) {
//     std::cout << "BENCHMARK: ALLOCATION/FREE" << std::endl;
//     std::cout << "\tSize:     \t" << size << std::endl;
//     std::cout << "\tAlignment\t" << alignment << std::endl;

//     // BUG: (https://github.com/mtrebi/memory-allocators/issues/6)
//     // void* addresses[m_nOperations];
//     void *addresses[OPERATIONS];

//     StartRound();

//     //allocator->Init();

//     auto operations = 0u;

//     while (operations < m_nOperations) {
//         addresses[operations] = allocator->allocate(size, alignment);
//         ++operations;
//     }

//     while (operations) {
//         allocator->deallocate(addresses[--operations], alignment);
//     }

//     FinishRound();

//     BenchmarkResults results = buildResults(m_nOperations, std::move(TimeElapsed), 0);

//     PrintResults(results);
// }

// void Benchmark::MultipleAllocation(allocator_base *allocator, const std::vector<std::size_t> &allocationSizes,
//                                    const std::vector<std::size_t> &alignments) {
//     assert(allocationSizes.size() == alignments.size() && "Allocation sizes and Alignments must have same length");

//     for (auto i = 0u; i < allocationSizes.size(); ++i) {
//         SingleAllocation(allocator, allocationSizes[i], alignments[i]);
//     }
// }

// void Benchmark::MultipleFree(allocator_base *allocator, const std::vector<std::size_t> &allocationSizes,
//                              const std::vector<std::size_t> &alignments) {
//     assert(allocationSizes.size() == alignments.size() && "Allocation sizes and Alignments must have same length");

//     for (auto i = 0u; i < allocationSizes.size(); ++i) {
//         SingleFree(allocator, allocationSizes[i], alignments[i]);
//     }
// }

// void Benchmark::RandomAllocation(allocator_base *allocator, const std::vector<std::size_t> &allocationSizes,
//                                  const std::vector<std::size_t> &alignments) {

//     // NOTE: Is this actually initializing the RNG? Jose Fernando Lopez Fernandez 11/07/2018 @ 12:54am (UTC)
//     srand(1);

//     std::cout << "\tBENCHMARK: ALLOCATION" << std::endl;

//     StartRound();

//     std::size_t allocation_size;
//     std::size_t alignment;

//     //allocator->Init();

//     auto operations = 0u;

//     while (operations < m_nOperations) {
//         this->RandomAllocationAttr(allocationSizes, alignments, allocation_size, alignment);
//         allocator->allocate(allocation_size, alignment);
//         ++operations;
//     }

//     FinishRound();

//     BenchmarkResults results = buildResults(m_nOperations, std::move(TimeElapsed), 0);

//     PrintResults(results);
// }

// void Benchmark::RandomFree(allocator_base *allocator, const std::vector<std::size_t> &allocationSizes,
//                            const std::vector<std::size_t> &alignments) {

//     // NOTE: Is this actually initializing the RNG? Jose Fernando Lopez Fernandez 11/07/2018 @ 1:51am (UTC)
//     srand(1);

//     std::cout << "\tBENCHMARK: ALLOCATION/FREE" << std::endl;

//     StartRound();

//     void *addresses[OPERATIONS];

//     std::size_t allocation_size;
//     std::size_t alignment;

//     //allocator->Init();

//     auto operations = 0u;

//     while (operations < m_nOperations) {
//         this->RandomAllocationAttr(allocationSizes, alignments, allocation_size, alignment);
//         addresses[operations] = allocator->allocate(allocation_size, alignment);
//         ++operations;
//     }

//     while (operations) {
//         allocator->deallocate(addresses[--operations], alignment);
//     }

//     FinishRound();

//     BenchmarkResults results = buildResults(m_nOperations, std::move(TimeElapsed), 0);

//     PrintResults(results);
// }

// void Benchmark::PrintResults(const BenchmarkResults &results) const {
//     std::cout << "\tRESULTS:" << std::endl;
//     std::cout << "\t\tOperations:    \t" << results.Operations << std::endl;
//     std::cout << "\t\tTime elapsed: \t" << results.Milliseconds.count() << " ms" << std::endl;
//     std::cout << "\t\tOp per sec:    \t" << results.OperationsPerSec << " ops/ms" << std::endl;
//     std::cout << "\t\tTimer per op:  \t" << results.TimePerOperation << " ms/ops" << std::endl;
//     std::cout << "\t\tMemory peak:   \t" << results.MemoryPeak << " bytes" << std::endl;

//     std::cout << std::endl;
// }

// const BenchmarkResults Benchmark::buildResults(std::size_t nOperations, std::chrono::milliseconds &&elapsedTime,
//                                                const std::size_t memoryPeak) const {
//     BenchmarkResults results;

//     results.Operations = nOperations;
//     results.Milliseconds = std::move(elapsedTime);
//     results.OperationsPerSec = results.Operations / static_cast<double>(results.Milliseconds.count());
//     results.TimePerOperation =
//         static_cast<double>(results.Milliseconds.count()) / static_cast<double>(results.Operations);
//     results.MemoryPeak = memoryPeak;

//     return results;
// }

// void Benchmark::RandomAllocationAttr(const std::vector<std::size_t> &allocationSizes,
//                                      const std::vector<std::size_t> &alignments, std::size_t &size,
//                                      std::size_t &alignment) {
//     const int r = rand() % allocationSizes.size();
//     size = allocationSizes[r];
//     alignment = alignments[r];
// }

// class c_allocator final : public allocator_base {
//   public:
//     c_allocator() noexcept;
//     ~c_allocator() noexcept;
//     void *do_allocate(u64 bytes, u64 alignment = alignof(std::max_align_t)) override;
//     void do_deallocate(void *ptr, u64 bytes, u64 alignment = alignof(std::max_align_t)) override;
//     bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;
//     void reset();
//     void resize(u64 size, u64 alignment);

//   private:
//     void *m_ptr = nullptr;
//     u64 m_offset = 0;
// };

// //TEST_CASE("bench_allocator") {
// //    ant_disable_memory_tracking();
// //    const std::size_t A = static_cast<std::size_t>(1e9);
// //    const std::size_t B = static_cast<std::size_t>(1e8);
// //
// //    const std::vector<std::size_t> ALLOCATION_SIZES{32, 64, 256, 512, 1024, 2048, 4096};
// //    const std::vector<std::size_t> ALIGNMENTS{8, 8, 8, 8, 8, 8, 8};
// //
// //    allocator_base *cAllocator = new c_allocator();
// //    allocator_base *linearAllocator = new linear_allocator(A);
// //    allocator_base *stackAllocator = new stack_allocator(A);
// //    allocator_base *poolAllocator = new pool_allocator(16777216, 4096);
// //    //allocator_base *freeListAllocator = new FreeListAllocator(B, FreeListAllocator::PlacementPolicy::FIND_FIRST);
// //
// //    Benchmark benchmark(OPERATIONS);
// //
// //    std::cout << "C" << std::endl;
// //    benchmark.MultipleAllocation(cAllocator, ALLOCATION_SIZES, ALIGNMENTS);
// //    benchmark.MultipleFree(cAllocator, ALLOCATION_SIZES, ALIGNMENTS);
// //    benchmark.RandomAllocation(cAllocator, ALLOCATION_SIZES, ALIGNMENTS);
// //    benchmark.RandomFree(cAllocator, ALLOCATION_SIZES, ALIGNMENTS);
// //
// //    std::cout << "LINEAR" << std::endl;
// //    benchmark.MultipleAllocation(linearAllocator, ALLOCATION_SIZES, ALIGNMENTS);
// //    benchmark.RandomAllocation(linearAllocator, ALLOCATION_SIZES, ALIGNMENTS);
// //
// //    std::cout << "STACK" << std::endl;
// //    benchmark.MultipleAllocation(stackAllocator, ALLOCATION_SIZES, ALIGNMENTS);
// //    benchmark.MultipleFree(stackAllocator, ALLOCATION_SIZES, ALIGNMENTS);
// //    benchmark.RandomAllocation(stackAllocator, ALLOCATION_SIZES, ALIGNMENTS);
// //    benchmark.RandomFree(stackAllocator, ALLOCATION_SIZES, ALIGNMENTS);
// //
// //    std::cout << "POOL" << std::endl;
// //    benchmark.SingleAllocation(poolAllocator, 4096, 8);
// //    benchmark.SingleFree(poolAllocator, 4096, 8);
// //
// //    //std::cout << "FREE LIST" << std::endl;
// //    //benchmark.MultipleAllocation(freeListAllocator, ALLOCATION_SIZES, ALIGNMENTS);
// //    //benchmark.MultipleFree(freeListAllocator, ALLOCATION_SIZES, ALIGNMENTS);
// //    //benchmark.RandomAllocation(freeListAllocator, ALLOCATION_SIZES, ALIGNMENTS);
// //    //benchmark.RandomFree(freeListAllocator, ALLOCATION_SIZES, ALIGNMENTS);
// //
// //    delete cAllocator;
// //    delete linearAllocator;
// //    delete stackAllocator;
// //    delete poolAllocator;
// //    ;
// //}

// c_allocator::c_allocator() noexcept {}

// c_allocator::~c_allocator() noexcept {}

// void *c_allocator::do_allocate(u64 bytes, u64 alignment) { return aligned_alloc(alignment, bytes); }

// void c_allocator::do_deallocate(void *ptr, u64 bytes, u64 alignment) { free(ptr); }
// bool c_allocator::do_is_equal(const std::pmr::memory_resource &other) const noexcept { return this == &other; }
