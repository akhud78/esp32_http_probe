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
$ idf.py -p /dev/ttyACM0 erase-flash
$ idf.py -p /dev/ttyACM0 flash monitor
```
- To exit IDF monitor use the shortcut `Ctrl+]`.

