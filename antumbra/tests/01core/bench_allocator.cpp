
#include "framework/01core/memory/memory.h"
#include "framework/01core/utils/utils.h"
#include <catch2/catch_test_macros.hpp>

using namespace ant::memory;
using namespace ant;

const u64 A = static_cast<u64>(1e9);
const u64 B = static_cast<u64>(1e8);

const std::vector<u64> ALLOCATION_SIZES{32, 64, 256, 512, 1024, 2048, 4096};
const std::vector<u64> ALIGNMENTS{8, 8, 8, 8, 8, 8, 8};

TEST_CASE("single allocation") {}

//TEST_CASE("")

//void Benchmark::SingleAllocation(Allocator *allocator, const u64 size, const u64 alignment) {
//    std::cout << "BENCHMARK: ALLOCATION" << IO::endl;
//    std::cout << "\tSize:     \t" << size << IO::endl;
//    std::cout << "\tAlignment\t" << alignment << IO::endl;
//
//    StartRound();
//
//    allocator->Init();
//
//    auto operations = 0u;
//
//    while (operations < m_nOperations) {
//        allocator->Allocate(size, alignment);
//        ++operations;
//    }
//
//    FinishRound();
//
//    BenchmarkResults results = buildResults(m_nOperations, std::move(TimeElapsed), allocator->m_peak);
//
//    PrintResults(results);
//}
//
//void Benchmark::SingleFree(Allocator *allocator, const u64 size, const u64 alignment) {
//    std::cout << "BENCHMARK: ALLOCATION/FREE" << IO::endl;
//    std::cout << "\tSize:     \t" << size << IO::endl;
//    std::cout << "\tAlignment\t" << alignment << IO::endl;
//
//    // BUG: (https://github.com/mtrebi/memory-allocators/issues/6)
//    // void* addresses[m_nOperations];
//    void *addresses[OPERATIONS];
//
//    StartRound();
//
//    allocator->Init();
//
//    auto operations = 0u;
//
//    while (operations < m_nOperations) {
//        addresses[operations] = allocator->Allocate(size, alignment);
//        ++operations;
//    }
//
//    while (operations) {
//        allocator->Free(addresses[--operations]);
//    }
//
//    FinishRound();
//
//    BenchmarkResults results = buildResults(m_nOperations, std::move(TimeElapsed), allocator->m_peak);
//
//    PrintResults(results);
//}
//
//void Benchmark::MultipleAllocation(Allocator *allocator, const std::vector<u64> &allocationSizes,
//                                   const std::vector<u64> &alignments) {
//    assert(allocationSizes.size() == alignments.size() && "Allocation sizes and Alignments must have same length");
//
//    for (auto i = 0u; i < allocationSizes.size(); ++i) {
//        SingleAllocation(allocator, allocationSizes[i], alignments[i]);
//    }
//}
//
//void Benchmark::MultipleFree(Allocator *allocator, const std::vector<u64> &allocationSizes,
//                             const std::vector<u64> &alignments) {
//    assert(allocationSizes.size() == alignments.size() && "Allocation sizes and Alignments must have same length");
//
//    for (auto i = 0u; i < allocationSizes.size(); ++i) {
//        SingleFree(allocator, allocationSizes[i], alignments[i]);
//    }
//}
//
//void Benchmark::RandomAllocation(Allocator *allocator, const std::vector<u64> &allocationSizes,
//                                 const std::vector<u64> &alignments) {
//
//    // NOTE: Is this actually initializing the RNG? Jose Fernando Lopez Fernandez 11/07/2018 @ 12:54am (UTC)
//    srand(1);
//
//    std::cout << "\tBENCHMARK: ALLOCATION" << IO::endl;
//
//    StartRound();
//
//    u64 allocation_size;
//    u64 alignment;
//
//    allocator->Init();
//
//    auto operations = 0u;
//
//    while (operations < m_nOperations) {
//        this->RandomAllocationAttr(allocationSizes, alignments, allocation_size, alignment);
//        allocator->Allocate(allocation_size, alignment);
//        ++operations;
//    }
//
//    FinishRound();
//
//    BenchmarkResults results = buildResults(m_nOperations, std::move(TimeElapsed), allocator->m_peak);
//
//    PrintResults(results);
//}
//
//void Benchmark::RandomFree(Allocator *allocator, const std::vector<u64> &allocationSizes,
//                           const std::vector<u64> &alignments) {
//
//    // NOTE: Is this actually initializing the RNG? Jose Fernando Lopez Fernandez 11/07/2018 @ 1:51am (UTC)
//    srand(1);
//
//    std::cout << "\tBENCHMARK: ALLOCATION/FREE" << IO::endl;
//
//    StartRound();
//
//    void *addresses[OPERATIONS];
//
//    u64 allocation_size;
//    u64 alignment;
//
//    allocator->Init();
//
//    auto operations = 0u;
//
//    while (operations < m_nOperations) {
//        this->RandomAllocationAttr(allocationSizes, alignments, allocation_size, alignment);
//        addresses[operations] = allocator->Allocate(allocation_size, alignment);
//        ++operations;
//    }
//
//    while (operations) {
//        allocator->Free(addresses[--operations]);
//    }
//
//    FinishRound();
//
//    BenchmarkResults results = buildResults(m_nOperations, std::move(TimeElapsed), allocator->m_peak);
//
//    PrintResults(results);
//}
//
//void Benchmark::PrintResults(const BenchmarkResults &results) const {
//    std::cout << "\tRESULTS:" << IO::endl;
//    std::cout << "\t\tOperations:    \t" << results.Operations << IO::endl;
//    std::cout << "\t\tTime elapsed: \t" << results.Milliseconds.count() << " ms" << IO::endl;
//    std::cout << "\t\tOp per sec:    \t" << results.OperationsPerSec << " ops/ms" << IO::endl;
//    std::cout << "\t\tTimer per op:  \t" << results.TimePerOperation << " ms/ops" << IO::endl;
//    std::cout << "\t\tMemory peak:   \t" << results.MemoryPeak << " bytes" << IO::endl;
//
//    std::cout << IO::endl;
//}
//
//const BenchmarkResults Benchmark::buildResults(u64 nOperations, std::chrono::milliseconds &&elapsedTime,
//                                               const u64 memoryPeak) const {
//    BenchmarkResults results;
//
//    results.Operations = nOperations;
//    results.Milliseconds = std::move(elapsedTime);
//    results.OperationsPerSec = results.Operations / static_cast<double>(results.Milliseconds.count());
//    results.TimePerOperation =
//        static_cast<double>(results.Milliseconds.count()) / static_cast<double>(results.Operations);
//    results.MemoryPeak = memoryPeak;
//
//    return results;
//}
//
//void Benchmark::RandomAllocationAttr(const std::vector<u64> &allocationSizes,
//                                     const std::vector<u64> &alignments, u64 &size,
//                                     u64 &alignment) {
//    const int r = rand() % allocationSizes.size();
//    size = allocationSizes[r];
//    alignment = alignments[r];
//}