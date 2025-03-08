import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import display
from esphome.const import (
    CONF_DATA_PIN,
    CONF_CLK_PIN,
    CONF_ID,
    CONF_LAMBDA,
    CONF_LENGTH,
)

ct1642_ns = cg.esphome_ns.namespace("ct1642")
CT1642Display = ct1642_ns.class_("CT1642Display", cg.PollingComponent)
CT1642DisplayRef = CT1642Display.operator("ref")

CONFIG_SCHEMA = display.BASIC_DISPLAY_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(CT1642Display),
        cv.Required(CONF_CLK_PIN): pins.gpio_output_pin_schema,
        cv.Required(CONF_DATA_PIN): pins.gpio_output_pin_schema,
        cv.Optional(CONF_LENGTH, default=4): cv.All(cv.uint8_t, cv.Range(min=1, max=4)),
    }
).extend(cv.polling_component_schema("20ms"))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await display.register_display(var, config)

    cs = await cg.gpio_pin_expression(config[CONF_CLK_PIN])
    cg.add(var.set_clk_pin(cs))
    data = await cg.gpio_pin_expression(config[CONF_DATA_PIN])
    cg.add(var.set_data_pin(data))

    cg.add(var.set_length(config[CONF_LENGTH]))

    if CONF_LAMBDA in config:
        lambda_ = await cg.process_lambda(
            config[CONF_LAMBDA], [(CT1642DisplayRef, "it")], return_type=cg.void
        )
        cg.add(var.set_writer(lambda_))
