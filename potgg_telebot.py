import os,telebot,json,random,threading,requests
from bs4 import BeautifulSoup
import urllib.request

TeleAPI = os.environ['key_api']
bot = telebot.TeleBot(TeleAPI)

def get_tspeak_data():
  Tspeak_url = "https://api.thingspeak.com/channels/1560863/feeds.json"
  Tspeak_header ='?results=1'
  url = Tspeak_url +  Tspeak_header
  get_data=requests.get(url).json()
  print(get_data)

  channel_id=get_data['channel']['id']

  field_1=get_data['feeds']
  print(field_1)

  t=[]
  for x in field_1:
    t.append(x['field1'])
    t.append(x['field2'])
    t.append(x['field3'])
    t.append(x['field4'])
    t.append(x['field5'])
    t.append(x['created_at'])
  print(t)
  return t


@bot.message_handler(commands=['ping'])
def data_now(message):
  bot.send_message(message.chat.id,"pong!")

@bot.message_handler(commands=['d'])
def data_now(message):
  data = get_tspeak_data()
  str_data = "Tanggal " + str(data[5]) + "\nTemperatur : " + str(data[0]) + "\nKelembaban Udara : " + str(data[1]) + "\nKelembaban Tanah : " + str(data[2]) + "\npH Tanah : " + str(data[3]) + "\nTingkat Kecerahan (0-255) : " + str(data[4])
  bot.send_message(message.chat.id,str_data)

bot.polling();
