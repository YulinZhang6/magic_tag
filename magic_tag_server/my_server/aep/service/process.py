import json
from my_server.utils import cv
from my_server.models import Devicerecord, Deviceinfo, Starinfo, User
from my_server.utils.store import pic_cache
from my_server.utils.device_cmd import resend_pic, push_weather
from datetime import datetime
from my_server.utils.reverseGeo import reverseAnalyzeGeo
from my_server.utils.gps_revise import reviseGps
from my_server.utils.weather import getWeather
from my_server.utils.subscribeSOSMsg import subSoSMsg

# 解析 json对象，并进行相应处理
def decode_json(json_obj):
    device_aep_id = json_obj['deviceId']
    # 事件上报
    if json_obj['messageType'] == 'eventReport':
        
        if json_obj['serviceId'] == 12:
            pass

    # 上线：1，下线：0
    elif json_obj['messageType'] == 'deviceOnlineOfflineReport':
        pass

    # 数据上传
    elif json_obj['messageType'] == 'dataReport':
        # 发送图片反馈
        if json_obj['serviceId'] == 50:
            lost_idxs = json_obj['payload']['lost_idxs']
            print(lost_idxs)
            lost_list = checkPicLost(lost_idxs)
            # 重发图片
            resend_pic(device_aep_id, lost_list)
            pass
        # gps上报
        elif json_obj['serviceId'] == 101:
            report_time = json_obj['timestamp']
            payload = json_obj['payload']
            longitude = payload['longitude']
            latitude = payload['latitude']
            report_type = payload['report_type']

            print('[gps上报] %d' % payload['report_type'])

            # 修正gps数据
            [longitude, latitude] = reviseGps(longitude, latitude)

            # 保存数据
            device = Deviceinfo.objects.get(device_aep_id=device_aep_id)
            record = Devicerecord(device=device, longitude=longitude, latitude=latitude,
                                  report_time=datetime.fromtimestamp(report_time / 1000),
                                  message_type=json_obj['serviceId'], report_type=report_type)
            record.save()
            # 定时上报 0           # 获取天气 3
            if report_type == 0 or report_type == 3:
                # 顺带查个天气
                wstr = getWeather(longitude, latitude)
                if wstr != "NaN":   # 发送天气指令
                    push_weather(device_aep_id, wstr)
                pass
            # SOS 1
            elif report_type == 1:
                addr = reverseAnalyzeGeo(latitude, longitude)
                if addr == '':
                    addr = '地点未知'
                record.address = addr
                record.save()

                # 消息推送
                subSoSMsg(record.device_id, addr)
                pass
            # 收藏 2
            elif report_type == 2 and record.latitude != 0:
                addr = reverseAnalyzeGeo(latitude, longitude)
                record.address = addr
                record.save()

                star = Starinfo(device=device, address=addr, latitude=latitude, longitude=longitude)
                star.save()
                pass
            # 追踪模式 4
            elif report_type == 4:
                record.report_type = 0
                record.save()
                pass
        elif json_obj['serviceId'] == 12:
            pass
        pass
    pass

# 检查图片丢包


def checkPicLost(data):
    lost_list = []
    for i in range(50):
        if data[i] == '\0':
            lost_list.append(i)
    return lost_list
