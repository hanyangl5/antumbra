//#include "freelist_allocator.h"
//
//#include "framework/01core/logging/log.h"
//
//namespace ante::memory {
//
//freelist_allocator::freelist_allocator(freelist_allocator_mode mode, u64 pool_size,
//                                       u64 alignment) noexcept{
//
//    u64 size = align_up(pool_size, alignment);
//    m_size = size;
//    m_ptr = mi_aligned_alloc(alignment, size);
//
//    m_head = nullptr;
//
//    if (b_enable_memory_tracking) {
//        LOG_DEBUG("[memory]: freelist allocator initilized at {}, size {}", m_ptr, m_size);
//    }
//};
//
//freelist_allocator::~freelist_allocator() noexcept {
//    free(m_ptr);
//    if (b_enable_memory_tracking) {
//        LOG_DEBUG("[memory]: freelist allocator destroyed at {}, size {}", m_ptr, m_size);
//    }
//    m_ptr = nullptr;
//};
//
//void freelist_allocator::free_all() {
//
//}
//
//void freelist_allocator::resize(u64 size, u64 alignment) {
//
//}
//
//void *freelist_allocator::do_allocate([[maybe_unused]] u64 bytes, [[maybe_unused]] u64 alignment) {
//
//}
//void freelist_allocator::do_deallocate(void *ptr, [[maybe_unused]] u64 bytes, [[maybe_unused]] u64 alignment) {
//
//
//
//}
//bool freelist_allocator::do_is_equal(const ante_pmr::memory_resource &other) const noexcept { return this == &other; }
//
//} // namespace ante::memory