import datetime
import time

from my_server.models import Deviceinfo, Devicerecord, Starinfo, User
from my_server.utils.reverseGeo import reverseAnalyzeGeo

# 请求位置数据
def getCurrentPosData(deviceid):
    response = {}
    pos_json = []
    star_json = []
    sos_json = {}
    current_json = {}
    points = []

    # user = User.objects.get(user_id=openid)
    # 前端保证有选中设备
    # if user.current_device_id is None:
    #     response['ok'] = 1
    #     response['pos_json'] = pos_json
    #     response['star_json'] = star_json
    #     response['sos_json'] = sos_json
    #     response['markers'] = markers
    #     return response
    # else:

    today_date = datetime.datetime.now().replace(hour=0, minute=0, second=0, microsecond=0)
    device = Deviceinfo.objects.get(device_id=deviceid)
    # 返回SOS、历史数据

    deviceRecord = Devicerecord.objects.filter(device_id=device, report_time__gte=today_date).filter(
        report_time__gte=device.bind_time).exclude(longitude=0)
    sosRecord = Devicerecord.objects.filter(report_time__gte=device.bind_time, device_id=device,report_type=1).order_by('-report_time')
    if len(sosRecord) != 0:
        sos_json['latitude'] = sosRecord[0].latitude
        sos_json['longitude'] = sosRecord[0].longitude
        sos_json['record_id'] = sosRecord[0].record_id
        if sosRecord[0].address is None:
            sosRecord[0].address = reverseAnalyzeGeo(lat=sosRecord[0].latitude, lng=sosRecord[0].longitude)
            sosRecord[0].save()
        sos_json['address'] = sosRecord[0].address
        if sosRecord[0].latitude == 0:
            sos_json['address'] = '地点未知'
        time.sleep(0.2)
        sos_json['width'] = 30
        sos_json['height'] = 30
        sos_json['iconPath'] = "../../images/location_sos.png"
        sos_json['record_time'] = sosRecord[0].report_time.strftime("%m-%d %H:%M")

    history = deviceRecord.filter(report_type=0).order_by('report_time')
    tempTime = 0
    for r in history:
        stamp = int(time.mktime(r.report_time.timetuple()))
        # 30分钟取一次
        if stamp-tempTime > 1800:
            pos={}
            pos['latitude'] = r.latitude
            pos['longitude'] = r.longitude
            pos['record_id'] = r.record_id
            if r.address is None:
                r.address = reverseAnalyzeGeo(lat=r.latitude, lng=r.longitude)
                time.sleep(0.2)
                r.save()
            pos['address'] = r.address
            pos['record_time'] = r.report_time.strftime("%H:%M")
            pos['iconPath'] = "../../images/location_history.png"
            pos['width'] = 30
            pos['height'] = 30
            tempTime = stamp
            pos_json.append(pos)
    pos_json.reverse()


    # 返回收藏数据
    starRecord = Starinfo.objects.filter(device_id=device, add_time__gte=device.bind_time)
    for r in starRecord:
        pos={}
        pos['latitude'] = r.latitude
        pos['longitude'] = r.longitude
        pos['star_id'] = r.star_id
        if r.address is None:
            r.address = reverseAnalyzeGeo(lat=r.latitude, lng=r.longitude)
            time.sleep(0.2)
            r.save()
        pos['callout'] = {
            "content": r.address,
            "display": 'BYCLICK'
        }
        time.sleep(0.2)
        if r.star_name is None:
            pos['address'] = r.address
        else:
            pos['address'] = r.star_name

        pos['iconPath'] = "../../images/location_star.png"
        pos['width'] = 30
        pos['height'] = 30
        star_json.append(pos)

    # 当前位置
    if len(history) >0:
        currentPos = list(history)[-1]
        current_json['latitude'] = currentPos.latitude
        current_json['longitude'] = currentPos.longitude
        current_json['record_id'] = currentPos.record_id
        current_json['address'] = reverseAnalyzeGeo(lat=currentPos.latitude, lng=currentPos.longitude)
        current_json['record_time'] = currentPos.report_time.strftime("%H:%M")
        current_json['iconPath'] = "../../images/location_current.png"
        current_json['width'] = 30
        current_json['height'] = 30

    for r in history:
        pos = {}
        pos['latitude'] = r.latitude
        pos['longitude'] = r.longitude
        points.append(pos)

    response['ok'] = 1
    response['pos_json'] = pos_json
    response['star_json'] = star_json
    response['sos_json'] = sos_json
    response['current_json'] = current_json
    response['points'] = points
    return response

# 设备列表数据
def getDeviceList(userid):
    response = {}
    device_json = []
    currentIndex = -1
    user = User.objects.get(user_id=userid)
    devicelist = Deviceinfo.objects.filter(user_id=user).order_by('bind_time')
    if len(devicelist) == 0:
        response['ok'] = 1
        response['devicelist'] = {'device_json':device_json,'currentIndex':currentIndex}
        return response


    index = 0
    for device in devicelist:
        info = {'IMEIid':device.device_id}
        if device.device_name is None:
            info['name'] = info['IMEIid']
        else:
            info['name'] = device.device_name
        if device.device_id == user.current_device_id:
            currentIndex = index
        device_json.append(info)
        index += 1

    response['ok'] = 1
    response['devicelist'] = {'device_json':device_json,'currentIndex':currentIndex}
    return response

# 获取历史数据
def getHistoryData(deviceid, date):
    response = {}
    datelist = date.split('-')
    print(datelist,date)
    device = Deviceinfo.objects.get(device_id=deviceid)
    history = Devicerecord.objects.filter(
        report_time__year=datelist[0],report_time__month=datelist[1],
        report_time__day=datelist[2],report_type=0,
        report_time__gte=device.bind_time).order_by('report_time').exclude(longitude=0)

    tempTime = 0
    points=[]
    pos_json = []
    for r in history:
        stamp = int(time.mktime(r.report_time.timetuple()))
        point = {}
        point['latitude'] = r.latitude
        point['longitude'] = r.longitude
        points.append(point)
        # 30分钟取一次
        if stamp-tempTime > 1800:
            pos={}
            pos['latitude'] = r.latitude
            pos['longitude'] = r.longitude
            if r.address is None:
                r.address = reverseAnalyzeGeo(lat=r.latitude, lng=r.longitude)
                time.sleep(0.2)
                r.save()
            pos['address'] = r.address
            pos['record_time'] = r.report_time.strftime("%H:%M")
            pos['iconPath'] = "../../images/location_history.png"
            pos['width'] = 30
            pos['height'] = 30
            tempTime = stamp
            pos_json.append(pos)
    pos_json.reverse()

    response['ok'] = 1
    response['poi'] = points
    response['pos_json'] = pos_json
    return response

# 请求位置数据
def getTrackInfo(deviceid):
    response = {}
    data = {
        "latitude": 31.86,
        "longitude": 118.821,
        "iconPath": "../../images/location_sos.png",
        "width": 30,
        "height": 30
        }
    device = Deviceinfo.objects.get(device_id=deviceid)
    last_minute = datetime.datetime.now() - datetime.timedelta(minutes=10)
    device_records = Devicerecord.objects.filter(device_id=device, report_type=0, report_time__gte=last_minute).order_by('-report_time').exclude(longitude=0)
    if len(device_records) == 0:
        return response
    data['latitude'] = device_records[0].latitude
    data['longitude'] = device_records[0].longitude
    response['data'] = data
    return response