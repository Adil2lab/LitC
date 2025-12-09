#pragma once

/**
 * Controller Protocol Definition
 * Defines the packet structure and button mappings for controller communication
 */

#include <cstddef>
#include <cstdint>

namespace controller {

#pragma pack(push, 1)
/**
 * Packed controller data packet
 * Total size: 18 bytes (no padding due to pack(1))
 */
struct Packet {
  uint32_t packetId;    // Unique packet identifier
  uint16_t buttons;     // Button state bitfield
  int16_t leftX;        // Left stick X axis (-32768 to 32767)
  int16_t leftY;        // Left stick Y axis (-32768 to 32767)
  int16_t rightX;       // Right stick X axis (-32768 to 32767)
  int16_t rightY;       // Right stick Y axis (-32768 to 32767)
  uint8_t leftTrigger;  // Left trigger (0-255)
  uint8_t rightTrigger; // Right trigger (0-255)
  uint16_t checksum;    // Packet integrity checksum
};
#pragma pack(pop)

// Verify expected packed size
static_assert(sizeof(Packet) == 18,
              "Packet must be exactly 18 bytes");

/**
 * Simple checksum calculation
 * Iterates through all bytes except the checksum field
 * @param packet Pointer to the packet to calculate checksum for
 * @return Computed checksum value
 */
inline uint16_t calculateChecksum(const Packet *packet) {
  uint16_t sum = 0;
  const uint8_t *data = reinterpret_cast<const uint8_t *>(packet);
  // Sum all bytes except the checksum
  for (size_t i = 0; i < sizeof(Packet) - sizeof(uint16_t); i++) {
    sum += data[i];
  }
  return sum;
}

/**
 * Controller button definitions
 * Use bitwise operations to check/set button states
 */
enum class Button : uint16_t {
  A = 1 << 0,
  B = 1 << 1,
  X = 1 << 2,
  Y = 1 << 3,
  L1 = 1 << 4,
  R1 = 1 << 5,
  L2 = 1 << 6,
  R2 = 1 << 7,
  SELECT = 1 << 8,
  START = 1 << 9,
  LEFT = 1 << 10,
  RIGHT = 1 << 11,
  UP = 1 << 12,
  DOWN = 1 << 13,
  LEFT_STICK_DOWN = 1 << 14,
  RIGHT_STICK_DOWN = 1 << 15,
};

// Bitwise operators for Button enum class
[[nodiscard]] constexpr uint16_t operator|(Button lhs,
                                           Button rhs) noexcept {
  return static_cast<uint16_t>(lhs) | static_cast<uint16_t>(rhs);
}

[[nodiscard]] constexpr uint16_t operator&(uint16_t lhs,
                                           Button rhs) noexcept {
  return lhs & static_cast<uint16_t>(rhs);
}

/**
 * Check if a specific button is pressed
 * @param buttons Button state bitfield
 * @param button Button to check
 * @return true if button is pressed
 */
[[nodiscard]] constexpr bool isButtonPressed(uint16_t buttons,
																						 Button button) noexcept {
  return (buttons & button) != 0;
}

/**
 * Set a button state
 * @param buttons Button state bitfield (modified in place)
 * @param button Button to set
 * @param pressed true to press, false to release
 */
constexpr void setButton(uint16_t &buttons, Button button,
                         bool pressed) noexcept {
  if (pressed) {
    buttons |= static_cast<uint16_t>(button);
  } else {
    buttons &= ~static_cast<uint16_t>(button);
  }
}

} // namespace controller