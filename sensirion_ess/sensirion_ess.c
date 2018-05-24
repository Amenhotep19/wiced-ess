/*
 *  Copyright (c) 2017-2018, Sensirion AG
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  * Neither the name of Sensirion AG nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "wiced.h"
#include "sensirion_ess.h"
#include "sensirion_common_wiced.h"

#include "sgp30.h"
#include "sht.h"

static s8 gLedGpiosConfigured     = 0;
static wiced_gpio_t gEssLedRed    = 0;
static wiced_gpio_t gEssLedYellow = 0;
static wiced_gpio_t gEssLedGreen  = 0;

wiced_result_t ess_init()
{
    /* default to WICED_I2C_1 */
    return ess_init_on_port(WICED_I2C_1);
}

wiced_result_t ess_init_on_port(wiced_i2c_t port)
{
    sensirion_wiced_set_i2c_port(port);

    if (sgp_probe() != STATUS_OK) {
        WPRINT_APP_INFO(("SGP sensor probing failed\n"));
        return WICED_ERROR;
    }
    if (sht_probe() != STATUS_OK) {
        WPRINT_APP_INFO(("SHT sensor probing failed\n"));
        return WICED_ERROR;
    }

    // TODO: check err
    //u16 err = sgp_iaq_init();
    sgp_iaq_init();

    return WICED_SUCCESS;
}

wiced_result_t ess_measure_iaq(u16* tvoc_ppb, u16* co2_eq_ppm)
{
    u16 err = sgp_measure_iaq_blocking_read(tvoc_ppb, co2_eq_ppm);
    if (err == STATUS_OK) {
        return WICED_SUCCESS;
    } else {
        return WICED_ERROR;
    }
}

wiced_result_t ess_measure_rht(s32* temperature, s32* humidity)
{
    s8 ret = sht_measure_blocking_read(temperature, humidity);
    if (ret == STATUS_OK) {
        return WICED_SUCCESS;
    } else {
        return WICED_ERROR;
    }
}

void ess_set_led(wiced_gpio_t pin, int state)
{
    if (state) {
        wiced_gpio_output_high(pin);
    } else {
        wiced_gpio_output_low(pin);
    }
}

void ess_init_gpios()
{
    if (gLedGpiosConfigured) {
        wiced_gpio_init(gEssLedRed, OUTPUT_PUSH_PULL);
        wiced_gpio_init(gEssLedYellow, OUTPUT_PUSH_PULL);
        wiced_gpio_init(gEssLedGreen, OUTPUT_PUSH_PULL);
    }
}


void ess_set_leds_ryg(int r, int y, int g)
{
    if (gLedGpiosConfigured) {
        ess_set_led(gEssLedRed, r);
        ess_set_led(gEssLedYellow, y);
        ess_set_led(gEssLedGreen, g);
    }
}

void ess_configure_leds(wiced_gpio_t pinRed, wiced_gpio_t pinYellow, wiced_gpio_t pinGreen)
{
    gEssLedRed = pinRed;
    gEssLedYellow = pinYellow;
    gEssLedGreen = pinGreen;
    gLedGpiosConfigured = 1;
    ess_init_gpios();
    ess_set_leds_ryg(0, 0, 0);
}
