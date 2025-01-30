# Python SimpleHTTPServer

- [Tests](tests.md)

---

## Setup
- [Python SimpleHTTPServer](https://www.digitalocean.com/community/tutorials/python-simplehttpserver-http-server) - use as local file server
- Download test files and run server
```
$ hostname
spb
$ mkdir /tmp/files
$ cd /tmp/files
$ curl https://raw.githubusercontent.com/akhud78/media/master/image.jpeg -o image.jpeg
$ python3 -m http.server 9000
Serving HTTP on 0.0.0.0 port 9000 (http://0.0.0.0:9000/) ...
```
- Check [spb.local:9000](http://spb.local:9000/) - `Directory listing for`

## Clients
### Raspberry Pi
- Login
```
avk@spb:~$ ssh ubuntu@ubuntu.local
```
- [time - run programs and summarize system resource usage](https://manpages.ubuntu.com/manpages/jammy/man1/time.1.html)
```
$ /bin/time --help
$ /bin/time -f %e sleep 1
1.01
```
- Time command execution (download image)
```
$ /bin/time -f %e curl http://spb.local:9000/image.jpeg -o /tmp/image.jpeg
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100 35588  100 35588    0     0   398k      0 --:--:-- --:--:-- --:--:--  404k
0.13
```
- Run a command multiple times
```
$ cmd="/bin/time -f %e curl -s http://spb.local:9000/image.jpeg -o /tmp/image.jpeg"
$ for i in {1..10}; do $cmd; done
0.48 <-- seconds max
0.09
0.20
0.09
0.07 <-- seconds min
0.07
0.07
0.07
0.07
0.07
```

### esp32s3
- HTTP REST Client
```
esp32> join
esp32> rest http://spb.local:9000/image.jpeg -l100
I (1762474) cmd_rest: status_code=200 bytes=35588
I (1762874) cmd_rest: status_code=200 bytes=35588
I (1762974) cmd_rest: status_code=200 bytes=35588
I (1763084) cmd_rest: status_code=200 bytes=35588
...
I (1773044) cmd_rest: status_code=200 bytes=35588
I (1773144) cmd_rest: status_code=200 bytes=35588
I (1773254) cmd_rest: status_code=200 bytes=35588
I (1773354) cmd_rest: status_code=200 bytes=35588
I (1773364) cmd_rest: laps=100 run_time_ms=13505 average_time_ms=135 average_bps=2108137
```
- ESP HTTP Client
```
esp32> client http://spb.local:9000/image.jpeg -l100
I (1857234) cmd_client: status_code=200 bytes=35588
I (1857344) cmd_client: status_code=200 bytes=35588
I (1857444) cmd_client: status_code=200 bytes=35588
I (1857554) cmd_client: status_code=200 bytes=35588
...
I (1867394) cmd_client: status_code=200 bytes=35588
I (1867504) cmd_client: status_code=200 bytes=35588
I (1867604) cmd_client: status_code=200 bytes=35588
I (1867714) cmd_client: status_code=200 bytes=35588
I (1867824) cmd_client: status_code=200 bytes=35588
I (1867834) cmd_client: laps=100 run_time_ms=11213 average_time_ms=112 average_bps=2539053
```
### esp32c3
- HTTP REST Client
```
esp32> join
esp32> rest http://spb.local:9000/image.jpeg -l100
I (21974) cmd_rest: status_code=200 bytes=35588
I (22454) cmd_rest: status_code=200 bytes=35588
I (23164) cmd_rest: status_code=200 bytes=35588
I (23334) cmd_rest: status_code=200 bytes=35588
...
I (33994) cmd_rest: status_code=200 bytes=35588
I (34104) cmd_rest: status_code=200 bytes=35588
I (34224) cmd_rest: status_code=200 bytes=35588
I (34334) cmd_rest: status_code=200 bytes=35588
I (34344) cmd_rest: laps=100 run_time_ms=13053 average_time_ms=130 average_bps=2181138
```
- ESP HTTP Client
```
esp32> client http://spb.local:9000/image.jpeg -l100
I (91814) cmd_client: status_code=200 bytes=35588
I (92504) cmd_client: status_code=200 bytes=35588
I (92764) cmd_client: status_code=200 bytes=35588
I (92904) cmd_client: status_code=200 bytes=35588
I (93114) cmd_client: status_code=200 bytes=35588
...
I (103704) cmd_client: status_code=200 bytes=35588
I (103824) cmd_client: status_code=200 bytes=35588
I (103924) cmd_client: status_code=200 bytes=35588
I (104034) cmd_client: status_code=200 bytes=35588
I (104034) cmd_client: laps=100 run_time_ms=12698 average_time_ms=126 average_bps=2242117
```