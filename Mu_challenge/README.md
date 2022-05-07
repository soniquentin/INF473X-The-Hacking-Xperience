# Mu CHALLENGE (INF473X The Hacking Xperience)


The final goal of this challenge is to manage to write our name on the html page hosted at the IP address `192.168.56.101`.

Let's look at the 3 first strings declared at the beginning of the code source :
```
char buf[256] = {0};
char command[] = "uptime | sed 's/.*up \[^,]*\, .*/\\1/'";
char greeting_text[128];
```
These strings are stored in the memory in the following order : `buf`, `greeting_text` and `command`. `buf` contains the data sent by a client and is then copied in `greeting_text`. However, as `greeting_text` has a smaller length in the memory than `buf`, copying `buf` may dangerously overwrite the next variable in the memory: `command`. Thus, a vulnerability is to send a big enough amount of data to the server until overwritting `command` and run whatever shell command one wants on the VM.

The file _hack_terminal.c_ is a terminal-like program of the VM. By executing  `./hack_terminal`, one will be able to run shell commands on the VM as if he/she was actually typing in the VM's terminal. To add a name to the page hosted at the IP address `192.168.56.101`, just type `cd ; for i in $(find . -name *.html); do echo '<p>YOUR NAME</p>' >>  $i; done` in the "hacked terminal"; this command finds all HTML files and append at the end `YOUR NAME`. 
