# Configuration

- [Home](../README.md)

---
## Main
- Open the project configuration menu
```
$ cd ~/esp/esp32_http_probe
$ idf.py menuconfig
```
- Project configuration - [Kconfig.projbuild](../main/Kconfig.projbuild)

## Common

- Use UART0 for console output
```
(Top) -> Component config -> ESP System Settings
    Channel for console output (Default: UART0)  --->
    Channel for console secondary output (No secondary console)  --->
```
- Set LED control
```
(Top)-> Example Configuration -> LED setup
    Blink LED type (RMT - Addressable LED)  --->
(48) Blink GPIO number
```

- Save configuration and build the project
```
$ idf.py build
```
- Connect your device
- Flash onto the device
```
$ idf.py -p /dev/ttyUSB0 erase-flash
$ idf.py -p /dev/ttyUSB0 flash monitor
```
- To exit IDF monitor use the shortcut `Ctrl+]`.

