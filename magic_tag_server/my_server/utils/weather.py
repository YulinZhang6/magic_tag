import requests

# 获取当前天气，返回字符串 Sunny
def getWeather(lon, lat):
    if lon == 0:
        return 'NaN'
    loc = "%.3f,%.3f" % (lon, lat)
    param = {'key': 'secretkey',
             'lang': 'en',
             'location': loc}
    url = 'https://devapi.qweather.com/v7/weather/now'
    response = requests.get(url, param)
    res_json = response.json()
    print("[weather]", res_json['now']['text'])
    return res_json['now']['text']


if __name__ == '__main__':
    getWeather(118.825, 31.883)
