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
    
## Adapting to a new Wiced Platform

By default, the code uses the first I2C bus ("port"), named ``WICED_I2C_1``. If your platform has multiple I2C ports, and you're running the ESS on another bus, simply use ``ess_init_on_port()`` to initialize the board, and pass the port name as an argument, for example ``ess_init_on_port(WICED_I2C_2)``.

