# MLX90640 library for Pico/RP2040

This is an implementation of the [Melexis MLX90640 library](https://github.com/melexis/mlx90640-library) using the Pico C++ SDK

I2C communication currently uses the default configuration (on Pico that's the first I2C device on GPIO pins 4 & 5).

The [Pimoroni library](https://github.com/pimoroni/mlx90640-library) for this device gives some example usage, and I've grabbed a couple of functions implemented there which are missing from the Melexis library.
