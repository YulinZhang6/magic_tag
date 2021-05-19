import datetime
import json
import requests

from my_server.models import Deviceinfo

# 传入设备id IMEI 、 SOS地址
# 暂时无法测试
def subSoSMsg(deviceid, address):
    # 拿到小程序的 ACESS TOKEN
    AppID = 'appid'
    AppSecret = 'appsecrest'
    getTokenUrl = 'https://api.weixin.qq.com/cgi-bin/token'
    form1 = {
        'grant_type': 'client_credential',
        'appid': AppID,
        'secret': AppSecret
    }
    response = requests.get(url=getTokenUrl, params=form1)
    access_token = json.loads(response.text)['access_token']
    print(access_token)
    baseUrl = 'https://api.weixin.qq.com/cgi-bin/message/subscribe/send'

    device = Deviceinfo.objects.get(device_id=deviceid)
    name = ''
    if device.device_name is None:
        name = '暂无设备名'
    else:
        name = device.device_name


    data = {
        'thing2':{'value': name},
        'character_string1': {'value': device.device_id},
        'time3': {'value': datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')},
        'thing5': {'value': address}
    }

    form2 = {
        'touser': device.user_id,
        'template_id': 'templateid',
        'page': 'pages/firstpage/firstpage',
        'miniprogram_state': 'developer',
        'data': data
    }

    res = requests.post(url=baseUrl, params={'access_token': access_token},
                        data=json.dumps(form2, ensure_ascii=False).encode('utf-8'))
    print(json.loads(res.text))
