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
esp32> rest http://esp32-fs.local/image.jpeg -l100
I (480844) cmd_rest: status_code=200 bytes=35588
I (483404) cmd_rest: status_code=200 bytes=35588
I (485864) cmd_rest: status_code=200 bytes=35588
I (488424) cmd_rest: status_code=200 bytes=35588
I (491694) cmd_rest: status_code=200 bytes=35588
...
I (515574) cmd_rest: status_code=200 bytes=35588
I (515714) cmd_rest: status_code=200 bytes=35588
I (515844) cmd_rest: status_code=200 bytes=35588
I (515974) cmd_rest: status_code=200 bytes=35588
I (515974) cmd_rest: laps=100 run_time_ms=37388 average_time_ms=373 average_bps=761485
```
- ESP HTTP Client
```
esp32> client http://esp32-fs.local/image.jpeg -l100
I (71084) cmd_client: status_code=200 bytes=35588 chunked
I (75334) cmd_client: status_code=200 bytes=35588 chunked
I (78824) cmd_client: status_code=200 bytes=35588 chunked
I (81694) cmd_client: status_code=200 bytes=35588 chunked
...
I (97204) cmd_client: status_code=200 bytes=35588 chunked
I (97414) cmd_client: status_code=200 bytes=35588 chunked
I (97594) cmd_client: status_code=200 bytes=35588 chunked
I (97764) cmd_client: status_code=200 bytes=35588 chunked
I (97954) cmd_client: status_code=200 bytes=35588 chunked
I (97954) cmd_client: laps=100 run_time_ms=27069 average_time_ms=270 average_bps=1051771
```
### esp32c3
- HTTP REST Client
```
esp32> join
esp32> rest http://esp32-fs.local/image.jpeg -l100
I (181434) cmd_rest: status_code=200 bytes=35588
I (184704) cmd_rest: status_code=200 bytes=35588
I (187674) cmd_rest: status_code=200 bytes=35588
I (192184) cmd_rest: status_code=200 bytes=35588
I (194444) cmd_rest: status_code=200 bytes=35588
I (196704) cmd_rest: status_code=200 bytes=35588

...
I (215384) cmd_rest: status_code=200 bytes=35588
I (215524) cmd_rest: status_code=200 bytes=35588
I (215694) cmd_rest: status_code=200 bytes=35588
I (215834) cmd_rest: status_code=200 bytes=35588
I (215974) cmd_rest: status_code=200 bytes=35588
I (215974) cmd_rest: laps=100 run_time_ms=38328 average_time_ms=383 average_bps=742809
```
- ESP HTTP Client
```
esp32> client http://esp32-fs.local/image.jpeg -l100
I (266764) cmd_client: status_code=200 bytes=35588 chunked
I (266994) cmd_client: status_code=200 bytes=35588 chunked
I (267224) cmd_client: status_code=200 bytes=35588 chunked
I (267474) cmd_client: status_code=200 bytes=35588 chunked
I (267724) cmd_client: status_code=200 bytes=35588 chunked
I (267954) cmd_client: status_code=200 bytes=35588 chunked
...
I (281834) cmd_client: status_code=200 bytes=35588 chunked
I (281984) cmd_client: status_code=200 bytes=35588 chunked
I (282134) cmd_client: status_code=200 bytes=35588 chunked
I (282274) cmd_client: status_code=200 bytes=35588 chunked
I (282434) cmd_client: status_code=200 bytes=35588 chunked
I (282434) cmd_client: laps=100 run_time_ms=15959 average_time_ms=159 average_bps=1783971

```