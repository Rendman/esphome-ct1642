#include "ct1642.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace ct1642 {

static const char *const TAG = "display.ct1642";
const uint8_t CT1642_UNKNOWN_CHAR = 0b11111111;

//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D   X
// ABCDEFGX
const uint8_t CT1642_ASCII_TO_RAW[] PROGMEM = {
  0b00000000,           // ' ', ord 0x20
  0b01100000,           // '!', ord 0x21
  CT1642_UNKNOWN_CHAR,  // '"', ord 0x22
  CT1642_UNKNOWN_CHAR,  // '#', ord 0x23
  CT1642_UNKNOWN_CHAR,  // '$', ord 0x24
  0b01001010,           // '%', ord 0x25 
  CT1642_UNKNOWN_CHAR,  // '&', ord 0x26
  0b01000000,           // ''', ord 0x27
  0b00010100,           // '(', ord 0x28
  0b01100000,           // ')', ord 0x29
  CT1642_UNKNOWN_CHAR,  // '*', ord 0x2A
  CT1642_UNKNOWN_CHAR,  // '+', ord 0x2B
  0b00010000,           // ',', ord 0x2C
  0b00000010,           // '-', ord 0x2D
  0b00000001,           // '.', ord 0x2E
  CT1642_UNKNOWN_CHAR,  // '/', ord 0x2F
  0b11111100,           // '0', ord 0x30
  0b01100000,           // '1', ord 0x31
  0b11011010,           // '2', ord 0x32
  0b11110010,           // '3', ord 0x33
  0b01100110,           // '4', ord 0x34
  0b10110110,           // '5', ord 0x35
  0b10111110,           // '6', ord 0x36
  0b11100000,           // '7', ord 0x37
  0b11111110,           // '8', ord 0x38
  0b11110110,           // '9', ord 0x39
  0b00000001,           // ':', ord 0x3A
  0b01011000,           // ';', ord 0x3B
  CT1642_UNKNOWN_CHAR,  // '<', ord 0x3C
  CT1642_UNKNOWN_CHAR,  // '=', ord 0x3D
  CT1642_UNKNOWN_CHAR,  // '>', ord 0x3E
  0b01100101,           // '?', ord 0x3F
  CT1642_UNKNOWN_CHAR,  // '@', ord 0x40
  0b11101110,           // 'A', ord 0x41
  CT1642_UNKNOWN_CHAR,  // 'B', ord 0x42
  0b10011100,           // 'C', ord 0x43
  CT1642_UNKNOWN_CHAR,  // 'D', ord 0x44
  0b10011110,           // 'E', ord 0x45
  0b10001110,           // 'F', ord 0x46
  CT1642_UNKNOWN_CHAR,  // 'G', ord 0x47
  0b01101110,           // 'H', ord 0x48
  0b00001100,           // 'I', ord 0x49
  0b01111000,           // 'J', ord 0x4A
  CT1642_UNKNOWN_CHAR,  // 'K', ord 0x4B
  0b00011100,           // 'L', ord 0x4C
  CT1642_UNKNOWN_CHAR,  // 'M', ord 0x4D
  CT1642_UNKNOWN_CHAR,  // 'N', ord 0x4E
  CT1642_UNKNOWN_CHAR,  // 'O', ord 0x4F
  0b11001110,           // 'P', ord 0x50
  CT1642_UNKNOWN_CHAR,  // 'Q', ord 0x51
  CT1642_UNKNOWN_CHAR,  // 'R', ord 0x52
  0b10110110,           // 'S', ord 0x53
  0b00011110,           // 'T', ord 0x54
  0b01111100,           // 'U', ord 0x55
  CT1642_UNKNOWN_CHAR,  // 'V', ord 0x56
  CT1642_UNKNOWN_CHAR,  // 'W', ord 0x57
  CT1642_UNKNOWN_CHAR,  // 'X', ord 0x58
  0b01110110,           // 'Y', ord 0x59
  CT1642_UNKNOWN_CHAR,  // 'Z', ord 0x5A
  CT1642_UNKNOWN_CHAR,  // '[', ord 0x5B
  CT1642_UNKNOWN_CHAR,  // '\', ord 0x5C
  CT1642_UNKNOWN_CHAR,  // ']', ord 0x5D
  CT1642_UNKNOWN_CHAR,  // '^', ord 0x5E
  0b00010000,           // '_', ord 0x5F
  CT1642_UNKNOWN_CHAR,  // '`', ord 0x60
  CT1642_UNKNOWN_CHAR,  // 'a', ord 0x61
  0b00111110,           // 'b', ord 0x62
  0b00011010,           // 'c', ord 0x63
  0b01111010,           // 'd', ord 0x64
  0b11011110,           // 'e', ord 0x65
  CT1642_UNKNOWN_CHAR,  // 'f', ord 0x66
  0b11110110,           // 'g', ord 0x67
  CT1642_UNKNOWN_CHAR,  // 'h', ord 0x68
  0b00001000,           // 'i', ord 0x69
  CT1642_UNKNOWN_CHAR,  // 'j', ord 0x6A
  CT1642_UNKNOWN_CHAR,  // 'k', ord 0x6B
  CT1642_UNKNOWN_CHAR,  // 'l', ord 0x6C
  CT1642_UNKNOWN_CHAR,  // 'm', ord 0x6D
  0b00101010,           // 'n', ord 0x6E
  0b00111010,           // 'o', ord 0x6F
  CT1642_UNKNOWN_CHAR,  // 'p', ord 0x70
  CT1642_UNKNOWN_CHAR,  // 'q', ord 0x71
  0b00001010,           // 'r', ord 0x72
  CT1642_UNKNOWN_CHAR,  // 's', ord 0x73
  0b00011110,           // 't', ord 0x74
  CT1642_UNKNOWN_CHAR,  // 'u', ord 0x75
  CT1642_UNKNOWN_CHAR,  // 'v', ord 0x76
  CT1642_UNKNOWN_CHAR,  // 'w', ord 0x77
  CT1642_UNKNOWN_CHAR,  // 'x', ord 0x78
  0b01110110,           // 'y', ord 0x79
  CT1642_UNKNOWN_CHAR,  // 'z', ord 0x7A
  CT1642_UNKNOWN_CHAR,  // '{', ord 0x7B
  CT1642_UNKNOWN_CHAR,  // '|', ord 0x7C
  CT1642_UNKNOWN_CHAR,  // '}', ord 0x7D
  CT1642_UNKNOWN_CHAR,  // '~', ord 0x7E
};

void CT1642Display::setup() {
    ESP_LOGCONFIG(TAG, "Setting up CT1642...");
  
    this->data_pin_->setup();               // OUTPUT
    this->data_pin_->digital_write(false);  // LOW
    this->clk_pin_->setup();                // OUTPUT
    this->clk_pin_->digital_write(false);   // LOW
  
    for (int i=0; i<4; i++)
    {
      this->buffer_[i] = 0b11111110;
    }

    this->display();
  }

  void CT1642Display::dump_config() {
    ESP_LOGCONFIG(TAG, "CT1642:");
    ESP_LOGCONFIG(TAG, "  Length: %d", this->length_);
    LOG_PIN("  CLK Pin: ", this->clk_pin_);
    LOG_PIN("  DATA Pin: ", this->data_pin_);
    LOG_UPDATE_INTERVAL(this);
  }

  void CT1642Display::update() {
    //for (uint8_t &i : this->buffer_)
    //  i = 0b11111110;
    if (this->writer_.has_value())
      (*this->writer_)(*this);
    this->display();
  }

  float CT1642Display::get_setup_priority() const { return setup_priority::PROCESSOR; }

  void CT1642Display::display() {
    ESP_LOGVV(TAG, "CT1642 Buffer %02X%02X%02X%02X", buffer_[0], buffer_[1], buffer_[2], buffer_[3]);

    this->send_byte_to_address(buffer_[0], 0);
    this->send_byte_to_address(buffer_[1], 1);
    this->send_byte_to_address(buffer_[2], 2);
    this->send_byte_to_address(buffer_[3], 3);
  }

  uint8_t CT1642Display::print(uint8_t pos, const char *str)
  {
    uint8_t data = CT1642_UNKNOWN_CHAR;

    if (*str >= ' ' && *str <= '~') {
      data = progmem_read_byte(&CT1642_ASCII_TO_RAW[*str - 32]);  // subract 32 to account for ASCII offset
      //ESP_LOGW(TAG, "got raw value %d", data);
    } else if (data == CT1642_UNKNOWN_CHAR) {
      ESP_LOGW(TAG, "Encountered character '%c' with no TM1638 representation while translating string!", *str);
    }

    this->buffer_[0] = data;
    return 0;
  }

  void CT1642Display::send_byte_to_address(uint8_t byte, uint8_t address)
  {
    uint8_t digit = 0;
    switch (address)
    {
      case 0: {
        digit = 0x7f;
        break;
      }
      case 1: {
        digit = 0xbf;
        break;
      }
      case 2: {
        digit = 0xdf;
        break;
      }
      case 3: {
        digit = 0xef;
        break;
      }
      default: {
        digit = 0xff;
      }
    }
        
    // Start by sending the four address bits
    for (int i=0; i<4; i++)
    {
      this->clk_pin_->digital_write(false);
      if ((digit << i) & 0x80)
      {
        this->data_pin_->digital_write(true);
      } else {
        this->data_pin_->digital_write(false);
      }
      this->clk_pin_->digital_write(true);
    }

    // Empty bits next
    for (int i=0; i<6; i++)
    {
      this->data_pin_->digital_write(true);
      this->clk_pin_->digital_write(false);
      this->clk_pin_->digital_write(true);
    }

    // Data bits last
    for (int i=0; i<8; i++)
    {
      this->clk_pin_->digital_write(false);
      if ((byte << i) & 0x80)
      {
        this->data_pin_->digital_write(true);
      } else {
        this->data_pin_->digital_write(false);
      }
      this->clk_pin_->digital_write(true);
    }

    // Then latch the data to the output pins
    this->data_pin_->digital_write(false);
    this->data_pin_->digital_write(true);
    this->clk_pin_->digital_write(false);
    this->data_pin_->digital_write(false);
    this->data_pin_->digital_write(true);

    //delay(100);
  }
}
}
