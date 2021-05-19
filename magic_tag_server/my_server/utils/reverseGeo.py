import hashlib

import requests
import json

def reverseAnalyzeGeo(lat,lng):
    baseUrl = 'https://apis.map.qq.com/ws/geocoder/v1'
    data = {}
    key = '地图key'
    sign_key = "应用生成的签名"

    baseSign = "/ws/geocoder/v1?key={}&location={},{}{}".format(key,lat,lng,sign_key)
    sign = hashlib.md5(bytes(baseSign, encoding="utf8")).hexdigest()
    # print(sign)
    data['key'] = key
    data['location'] = '{},{}'.format(lat, lng)
    data['sig'] = sign
    response = requests.get(url= baseUrl, params=data)
    # print(json.loads(response.text))
    return json.loads(response.text)['result']['address']

if __name__=='__main__':
    print(reverseAnalyzeGeo(39.913950, 116.395683))
