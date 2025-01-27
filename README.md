# esp32_http_probe
HTTP client for testing only

## Requirements
- Ubuntu 22.04 LTS
- [ESP-IDF v5.4](https://docs.espressif.com/projects/esp-idf/en/v5.4/esp32s3/index.html)
- [ESP32-S3-DevKitC-1](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/hw-reference/esp32s3/user-guide-devkitc-1.html)

## Links
- [HTTP REST Client](https://components.espressif.com/components/parabuzzle/http_rest_client)
- [LED Strip Driver](https://components.espressif.com/components/espressif/led_strip/versions/3.0.0)

## Building
- Get ESP-IDF
```
$ mkdir -p ~/esp
$ cd ~/esp
$ git clone -b v5.4 --recursive https://github.com/espressif/esp-idf.git esp-idf-v5.4
$ cd esp-idf-v5.4
$ export IDF_TOOLS_PATH="$HOME/idf-tools-v5.4"
$ ./install.sh
```
- Set up the environment variables
```
$ . $HOME/esp/esp-idf-v5.4/export.sh
```
- Clone and setup [project](https://github.com/akhud78/esp32_http_probe)
```
$ cd ~/esp
$ git clone https://github.com/akhud78/esp32_http_probe
$ cd ~/esp/esp32_http_probe
$ git submodule update --init --recursive
```
- Set the chip target to build
```
$ idf.py set-target esp32s3
```
- Change build [Configuration](docs/config.md)

## Run
- Connect development board to your computer and run
```
$ cd ~/esp/esp32_http_probe
$ idf.py -p /dev/ttyUSB0 flash monitor
...
...
Type 'help' to get the list of commands.
Use UP/DOWN arrows to navigate through command history.
Press TAB when typing command name to auto-complete.
esp32>
```
- Set Wi-Fi AP `ssid` and `password` (only the first time)
```
  esp32> nvs_set ssid str -v Hotspot-5413
  esp32> nvs_set pass str -v fHMUja9a
  esp32> nvs_list nvs
  ...
  namespace 'storage', key 'ssid', type 'str' 
  namespace 'storage', key 'pass', type 'str'
```
- Join Wi-Fi AP as a station
```
esp32> join
I (57065) connect: Connecting to 'Hotspot-5413'
...
I (59975) esp_netif_handlers: sta ip: 192.168.1.129, mask: 255.255.255.0, gw: 192.168.1.1
I (59975) connect: Connected
```
- Send ICMP ECHO_REQUEST to network hosts
```
esp32> ping example.com
64 bytes from 93.184.216.34 icmp_seq=1 ttl=51 time=236 ms
64 bytes from 93.184.216.34 icmp_seq=2 ttl=51 time=275 ms
64 bytes from 93.184.216.34 icmp_seq=3 ttl=51 time=184 ms
64 bytes from 93.184.216.34 icmp_seq=4 ttl=51 time=208 ms
64 bytes from 93.184.216.34 icmp_seq=5 ttl=51 time=234 ms

--- 93.184.216.34 ping statistics ---
5 packets transmitted, 5 received, 0% packet loss, time 1137ms
```