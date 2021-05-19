import requests
import json
from my_server.models import User

# 登录，保存openid 和 更新session_key 返回用户的openid和当前绑定设备
def Login(code):
    result = {}
    AppID = 'appid'
    AppSecret = 'appsecrest'
    # url = 'https://api.weixin.qq.com/sns/jscode2session?appid={}&secret={}&js_code={}&grant_type=authorization_code'
    # response = request.get(url.format(AppID,AppSecret,code))
    baseUrl = 'https://api.weixin.qq.com/sns/jscode2session'
    data = {
        'appid': AppID,
        'secret': AppSecret,
        'js_code': code,
        'grant_type': 'authorization_code'
    }
    response = requests.get(url=baseUrl, params=data)
    response = json.loads(response.text)
    # print(response)
    session_key = response['session_key']
    openid = response['openid']

    user = User.objects.filter(user_id=openid)
    if len(user) == 0:
        new_user = User(user_id=openid, user_session=session_key)
        new_user.save()
        result['current_device_id'] = -1
        result['ok'] = 1
        result['openid'] = openid
        result['msg'] = '登录成功'
        print('[新用户注册]')
        return result
    else:
        if user[0].user_session!=session_key:
            print('[更新session_key]')
            user[0].user_session=session_key
            user[0].save()

    if user[0].current_device_id is None:
        result['current_device_id'] = -1
    else:
        result['current_device_id'] = user[0].current_device_id
    result['ok'] = 1
    result['openid'] = openid
    result['msg'] = '登录成功'
    return result