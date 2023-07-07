#pragma once

#include <atomic>

namespace ant::gal {
    
extern std::atomic<bool> b_enable_validation_layer;
void enable_validation_layer() { ant::gal::b_enable_validation_layer = true; }
void disable_validation_layer() { ant::gal::b_enable_validation_layer = false; }

} // namespace ant::gal
