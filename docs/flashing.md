# Flashing the prebuild binaries

- [Home](../README.md)

---
- Go to `esp32_http_probe` project directory and flash target.

```bash
esptool.py --port /dev/ttyACM0 erase_flash
esptool.py --chip esp32s3 --port /dev/ttyACM0 \
--before=default_reset --after=hard_reset write_flash \
--flash_mode dio --flash_freq 80m --flash_size detect \
0x0 build/esp32s3/bootloader.bin \
0x8000 build/esp32s3/partition-table.bin \
0x10000 build/esp32s3/esp32_http_probe.bin
```
- You can use [PuTTY](https://www.putty.org/) for Windows or [GTKTerm](https://manpages.ubuntu.com/manpages/jammy/man1/gtkterm.1.html) for Linux as serial port terminal.

