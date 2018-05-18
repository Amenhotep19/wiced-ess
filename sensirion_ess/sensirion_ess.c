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

#include "sgp30.h"
#include "sht.h"

void setLed(wiced_gpio_t pin, int state)
{
    if (state) {
        wiced_gpio_output_high(pin);
    } else {
        wiced_gpio_output_low(pin);
    }
}

wiced_result_t ess_init()
{
    wiced_gpio_init(ESS_LED_RED, OUTPUT_PUSH_PULL);
    wiced_gpio_init(ESS_LED_YEL, OUTPUT_PUSH_PULL);
    wiced_gpio_init(ESS_LED_GRN, OUTPUT_PUSH_PULL);

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

void ess_set_leds_ryg(int r, int y, int g)
{
    setLed(ESS_LED_RED, r);
    setLed(ESS_LED_YEL, y);
    setLed(ESS_LED_GRN, g);
}
