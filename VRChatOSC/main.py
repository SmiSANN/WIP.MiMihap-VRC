from cgi import test
from pythonosc import dispatcher
from pythonosc import osc_server
import requests

server = osc_server.ThreadingOSCUDPServer(("127.0.0.1", 9001), dispatcher)
print("Serving on {}".format(server.server_address))
url = 'http://mimihp.local/?LENGTH=' + length0 + '&POWER=' + power0 + '&LENGTH1' + length1 + '&POWER1' + power1
print (url)
response = requests.get(url)
print(response.status_code)    # HTTPのステータスコード取得
print(response.text)    # レスポンスのHTMLを文字列で取得

server.serve_forever()
