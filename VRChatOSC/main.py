from cgi import test
from pythonosc import dispatcher
from pythonosc import osc_server
import requests
test = 0
def messege_handler(unused_addr, *p):
    try:
        print(p)
        test.send_message("/message_echo", p)
    except ValueError: pass

dispatcher = dispatcher.Dispatcher()
dispatcher.map("/message", messege_handler)
server = osc_server.ThreadingOSCUDPServer(("127.0.0.1", 9001), dispatcher)
print("Serving on {}".format(server.server_address))

if 0 == 1:
    response = requests.get('http://mimihp.local/?LENGTH0=1000&POWER0=255&LENGTH1=1000&POWER1=255')
    print(response.status_code)    # HTTPのステータスコード取得
    print(response.text)    # レスポンスのHTMLを文字列で取得

server.serve_forever()