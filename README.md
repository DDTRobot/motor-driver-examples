# DDT motor driver examples

This is a collection of DDT motor driver's examples, including:

- [M0602C](./M0602C/)
- [M1502A](./M1502A/)

Read `README.md` under these subdirectories.

## Prerequisite

Make sure you have installed cross-complier for ARM Cortex M series MCU, such as `arm-none-eabi-gcc`. If your development environment platform is Ubuntu, just install it by using:

```
$ sudo apt install build-essential gcc-arm-none-eabi
```

## How to compile

To compile examples in this repository, from any folder in [M0602C](./M0602C/), [M1502A](./M1502A/), execute:

```
make
```

All product will be created in the `build` subdirectory in where you executed `make`.
