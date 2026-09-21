#pragma once

#include <Arduino.h>

namespace board {

// Inputs are active-low after the isolated/comparator line interface.
constexpr gpio_num_t kOffHookA = GPIO_NUM_1;
constexpr gpio_num_t kOffHookB = GPIO_NUM_2;

// Outputs drive low-voltage MOSFET/isolator inputs, never a phone line directly.
constexpr gpio_num_t kLineRelayA = GPIO_NUM_4;
constexpr gpio_num_t kLineRelayB = GPIO_NUM_5;
constexpr gpio_num_t kRingEnable = GPIO_NUM_6;
constexpr gpio_num_t kRingPolarity = GPIO_NUM_7;

// Reserved expansion bus.
constexpr gpio_num_t kI2cSda = GPIO_NUM_8;
constexpr gpio_num_t kI2cScl = GPIO_NUM_9;

}  // namespace board

