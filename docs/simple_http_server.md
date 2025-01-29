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
esp32> rest http://spb.local:9000/image.jpeg -l10
I (71123) cmd_rest: status=200 bytes=35588 run_time_ms=2558 bps=111299 <-- max
I (71223) cmd_rest: status=200 bytes=35588 run_time_ms=93 bps=3061333
I (71313) cmd_rest: status=200 bytes=35588 run_time_ms=93 bps=3061333
I (71413) cmd_rest: status=200 bytes=35588 run_time_ms=96 bps=2965666
I (71503) cmd_rest: status=200 bytes=35588 run_time_ms=93 bps=3061333
I (71603) cmd_rest: status=200 bytes=35588 run_time_ms=92 bps=3094608  <-- min
I (71703) cmd_rest: status=200 bytes=35588 run_time_ms=99 bps=2875797
I (71893) cmd_rest: status=200 bytes=35588 run_time_ms=185 bps=1538940
I (72063) cmd_rest: status=200 bytes=35588 run_time_ms=170 bps=1674729
I (72273) cmd_rest: status=200 bytes=35588 run_time_ms=206 bps=1382058
```
- ESP HTTP Client
```
esp32> client http://spb.local:9000/image.jpeg -l10
I (389728) cmd_client: status=200 bytes=35588 run_time_ms=143 bps=1990937
I (390928) cmd_client: status=200 bytes=35588 run_time_ms=1194 bps=238445 <-- max
I (391028) cmd_client: status=200 bytes=35588 run_time_ms=95 bps=2996884
I (391118) cmd_client: status=200 bytes=35588 run_time_ms=89 bps=3198921 <-- min
I (391218) cmd_client: status=200 bytes=35588 run_time_ms=92 bps=3094608
I (391308) cmd_client: status=200 bytes=35588 run_time_ms=92 bps=3094608
I (391398) cmd_client: status=200 bytes=35588 run_time_ms=90 bps=3163377
I (391498) cmd_client: status=200 bytes=35588 run_time_ms=95 bps=2996884
I (391588) cmd_client: status=200 bytes=35588 run_time_ms=89 bps=3198921
I (391698) cmd_client: status=200 bytes=35588 run_time_ms=104 bps=2737538
```

