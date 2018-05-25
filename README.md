# wiced-ess

This is a preview release WICED Wifi SDK support for the [Sensirion Environmental Sensor Shield (ESS)](https://developer.sensirion.com/ess/)

It has been tested on the [Future Nebula platform](http://www.futureelectronics.com/en/Technologies/Product.aspx?ProductID=NEB1DX01FCS1089735), and small changes might be necessary to run on other boards (see section "Adapting to a new Wiced Platform")

## Installation
1. download one of the release ZIPs from https://github.com/winkj/wiced-ess/releases (you can also clone the git repository, in which case the decompression step below isn't necessary).
2. decompress the ZIP file
3. rename the directory from ``wiced-ess`` to ``sensirion``
4. copy the ``sensirion`` directory to your WICED-SDK installation:
    - Open Wiced Studio
    - Open the project explorer, and navigate to ``43xxx_Wi-Fi > libraries > sensors``
    - Right click and select ``Properties``
    - Find the ``Location`` of the folder; this is something like ``C:\Users\YOURUSERNAME\Documents\WICED-Studio-6.0\43xxx_Wi-Fi\libraries\drivers\sensors``
5. open that directory in your file explorer (Windows) or Finder (MacOS)
6. copy the ``sensirion`` directory to this folder
7. switch back to Wiced Studio
8. Right click on ``43xxx_Wi-Fi > libraries > sensors`` and choose ``Refresh``

## Creating a demo app
1. Create a new directory ``ess_demo`` in ``43xxx_Wi-Fi > app > demo`` (or another location of your choosing
2. From ``43xxx_Wi-Fi > libraries > sensors > sensirion > sensirion_ess``, copy the ``ess_demo_c.txt`` and ``ess_demo_mk.txt`` files to the demo directory you created before
3. rename those to ``ess_demo.c`` and ``ess_demo.mk``
4. In the ``Make target`` window, create a new target called ``demo.ess_demo-YOURPLATFORM download run``. replace YOURPLATFORM with the platform you are using, for example NEB1DX_01 for the Future Nebula platform
5. double click the newly created make target

this should build the app, download it to the board and reset the board. Connect a serial terminal to the serial-usb port of your WICED board, and you should see printouts of the TVOC, CO2eq, humidity and temperature values.
    
## API definition
The sensirion_ess library has four functions:

```c
wiced_result_t ess_init(ess_device_config_t config);
```
``ess_init(ess_device_config_t config)`` initializes the ESS, probes the sensors, and makes sure the right sensor versions are detected. 
The ``config`` parameter configures platform specific settings. There are sample configurations in ``ess_device_configs.{h,c}``, including
a default config that is using ``WICED_I2C_1``, with LED support disabled. To run that, simply call ``ess_init(&ESS_DEVICE_CONFIG_DEFAULT)``.
Will return ``WICED_SUCCESS`` on success, ``WICED_ERROR`` otherwise.

```c
wiced_result_t ess_measure_iaq(u16* tvoc_ppb, u16* co2_eq_ppm);
```
Triggers an Indoor Air Quality (IAQ) measurement, and assigns result to ``tvoc_ppb`` and ``co2_eq_ppm`` respectively. Will return ``WICED_SUCCESS`` on success, ``WICED_ERROR`` otherwise.

```c
wiced_result_t ess_measure_rht(s32* temperature, s32* humidity);
```
Triggers a humidity and temperature measurement, and assigns result to ``temperature`` and ``humidity`` respectively. Will return ``WICED_SUCCESS`` on success, ``WICED_ERROR`` otherwise. Will return ``WICED_SUCCESS`` on success, ``WICED_ERROR`` otherwise.

```c
void ess_set_leds_ryg(int r, int y, int g);
```
Toggles LEDs on and off. Pass ``1`` to turn LED on, ``0`` to turn it off. ``r`` is for the red LED, ``y`` is the yellow one, ``g`` is for the green one.

## Adapting to a new Wiced Platform

To port to a new platform, create a ess_device_config_t to match your platform; there's a default config in ``ess_device_configs.c``, which looks like this:
```c
const ess_device_config_t ESS_DEVICE_CONFIG_DEFAULT = {
    .i2c_port              = WICED_I2C_1,
    .needs_init_workaround = 0,
    .leds_supported        = 0
};
```
The most common change would be to change the I2C port to something else, for example ``WICED_I2C_2``.

To add support for the LEDs on the ESS, set the ``.leds_supported`` field to ``1``, and set the GPIO numbers accordingly. Here an example for the [Arrow Quicksilver](https://www.arrow.com/quicksilver) platform:
```c
const ess_device_config_t ESS_DEVICE_CONFIG_QUICKSILVER = {
        .i2c_port              = WICED_I2C_2,
        .needs_init_workaround = 0,
        .leds_supported        = 1,
        .pin_red               = WICED_GPIO_36,
        .pin_yellow            = WICED_GPIO_22,
        .pin_green             = WICED_GPIO_21
};
```