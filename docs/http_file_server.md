# ESP HTTP File Server

- [Tests](tests.md)

---

## Setup
- [esp32_http_file_server](https://github.com/akhud78/esp32_http_file_server)
- Upload [image.jpeg](https://raw.githubusercontent.com/akhud78/media/master/image.jpeg) to ESP32 File Server
- Open [esp32-fs.local](http://esp32-fs.local/) and check.

## Clients
### Raspberry Pi
- Login
```
avk@spb:~$ ssh ubuntu@ubuntu.local
```
- Time command execution (download image)
```
$ /bin/time -f %e curl http://esp32-fs.local/image.jpeg -o /tmp/image.jpeg
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100 35588    0 35588    0     0   195k      0 --:--:-- --:--:-- --:--:--  196k
0.22
```
- Run a command multiple times
```
$ cmd="/bin/time -f %e curl -s http://esp32-fs.local/image.jpeg -o /tmp/image.jpeg"
$ for i in {1..10}; do $cmd; done
0.83 <-- max
0.27
0.29
0.57
0.39
0.24
0.29
0.35
0.23 <-- min
0.30
```
### esp32s3
- HTTP REST Client
```
esp32> join
esp32> rest http://esp32-fs.local/image.jpeg -l10
I (51533) cmd_rest: status=200 bytes=35588 run_time_ms=2260 bps=125975
I (53903) cmd_rest: status=200 bytes=35588 run_time_ms=2361 bps=120586 <-- max
I (54113) cmd_rest: status=200 bytes=35588 run_time_ms=212 bps=1342943
I (54233) cmd_rest: status=200 bytes=35588 run_time_ms=122 bps=2333639
I (54363) cmd_rest: status=200 bytes=35588 run_time_ms=126 bps=2259555
I (54513) cmd_rest: status=200 bytes=35588 run_time_ms=152 bps=1873052
I (54633) cmd_rest: status=200 bytes=35588 run_time_ms=113 bps=2519504
I (54763) cmd_rest: status=200 bytes=35588 run_time_ms=134 bps=2124656
I (54913) cmd_rest: status=200 bytes=35588 run_time_ms=146 bps=1950027
I (55043) cmd_rest: status=200 bytes=35588 run_time_ms=124 bps=2296000 <-- min
```
- ESP HTTP Client
```
esp32> client http://esp32-fs.local/image.jpeg -l10
I (77043) cmd_client: status=200 bytes=35588 run_time_ms=1748 bps=162874 chunked
I (77193) cmd_client: status=200 bytes=35588 run_time_ms=146 bps=1950027 chunked
I (77343) cmd_client: status=200 bytes=35588 run_time_ms=140 bps=2033600 chunked
I (77503) cmd_client: status=200 bytes=35588 run_time_ms=159 bps=1790591 chunked
I (77653) cmd_client: status=200 bytes=35588 run_time_ms=145 bps=1963475 chunked
I (77773) cmd_client: status=200 bytes=35588 run_time_ms=120 bps=2372533 chunked
I (77923) cmd_client: status=200 bytes=35588 run_time_ms=141 bps=2019177 chunked
I (78073) cmd_client: status=200 bytes=35588 run_time_ms=151 bps=1885456 chunked
I (78223) cmd_client: status=200 bytes=35588 run_time_ms=149 bps=1910765 chunked
I (78373) cmd_client: status=200 bytes=35588 run_time_ms=148 bps=1923675 chunked
```
### esp32c3
- HTTP REST Client
```
esp32> join
esp32> rest http://esp32-fs.local/image.jpeg -l10
I (110383) cmd_rest: status=200 bytes=35588 run_time_ms=3391 bps=83958 <-- max
I (112653) cmd_rest: status=200 bytes=35588 run_time_ms=2267 bps=125586
I (114923) cmd_rest: status=200 bytes=35588 run_time_ms=2268 bps=125530
I (117173) cmd_rest: status=200 bytes=35588 run_time_ms=2251 bps=126478
I (118223) cmd_rest: status=200 bytes=35588 run_time_ms=1048 bps=271664
I (118433) cmd_rest: status=200 bytes=35588 run_time_ms=210 bps=1355733
I (118603) cmd_rest: status=200 bytes=35588 run_time_ms=166 bps=1715084
I (118753) cmd_rest: status=200 bytes=35588 run_time_ms=151 bps=1885456
I (118903) cmd_rest: status=200 bytes=35588 run_time_ms=148 bps=1923675
I (119043) cmd_rest: status=200 bytes=35588 run_time_ms=139 bps=2048230 <-- min
```
- ESP HTTP Client
```
esp32> client http://esp32-fs.local/image.jpeg -l10
I (228053) cmd_client: status=200 bytes=35588 run_time_ms=2282 bps=124760 chunked
I (230413) cmd_client: status=200 bytes=35588 run_time_ms=2352 bps=121047 chunked <-- max
I (232673) cmd_client: status=200 bytes=35588 run_time_ms=2261 bps=125919 chunked
I (234933) cmd_client: status=200 bytes=35588 run_time_ms=2256 bps=126198 chunked
I (236153) cmd_client: status=200 bytes=35588 run_time_ms=1223 bps=232791 chunked
I (236363) cmd_client: status=200 bytes=35588 run_time_ms=206 bps=1382058 chunked
I (236523) cmd_client: status=200 bytes=35588 run_time_ms=150 bps=1898026 chunked
I (236673) cmd_client: status=200 bytes=35588 run_time_ms=145 bps=1963475 chunked
I (236813) cmd_client: status=200 bytes=35588 run_time_ms=140 bps=2033600 chunked
I (236953) cmd_client: status=200 bytes=35588 run_time_ms=138 bps=2063072 chunked <-- min
```