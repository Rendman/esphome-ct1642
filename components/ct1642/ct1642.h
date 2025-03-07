#pragma once

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace ct1642 {

#define DIGIT_1   0x7f
#define DIGIT_2   0xbf
#define DIGIT_3   0xdf
#define DIGIT_4   0xef
#define DIGIT_OFF 0xff 
  
class CT1642Display;

using ct1642_writer_t = std::function<void(CT1642Display &)>;

class CT1642Display : public PollingComponent {
  public:
    void set_writer(ct1642_writer_t &&writer) { this->writer_ = writer; }


    void setup() override;
    void dump_config() override;
    float get_setup_priority() const;
    void update();

    void set_length(uint8_t length) {length_ = length;}
    void set_clk_pin(GPIOPin *pin) {clk_pin_ = pin;}
    void set_data_pin(GPIOPin *pin) {data_pin_ = pin;}

    void display();

    uint8_t print(uint8_t pos, const char *str);

  protected:

    void send_byte_to_address(uint8_t byte, uint8_t address);

    GPIOPin *clk_pin_;
    GPIOPin *data_pin_;

    uint8_t length_;
    uint8_t buffer_[5] = {0};
    optional<ct1642_writer_t> writer_{};
}; 

}
}