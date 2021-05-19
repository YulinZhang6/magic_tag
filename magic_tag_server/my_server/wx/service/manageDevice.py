import datetime
from my_server.utils.device_cmd import queryDeviceIdByImei, track_mode
from my_server.models import Deviceinfo, User

# 之后将调整逻辑，初版测试用



def bindDevice(userid, newdevice):
    response={}
    device = Deviceinfo.objects.filter(device_id=newdevice)
    if len(device) == 0:
        # user = User.objects.get(user_id=userid)
        res = queryDeviceIdByImei(newdevice)
        if res is False:
            response['ok'] = 3
            response['msg'] = '设备不存在'
        else:
            newDev = Deviceinfo(user_id=userid, device_id=newdevice, bind_time=datetime.datetime.now(),device_aep_id=res)
            newDev.save()
            response['ok'] = 1
            response['msg'] = '绑定成功'
            return response
    else:
        response['ok'] = 2
        response['msg'] = '设备已被绑定'
    return response

# 改变选中设备
def changeDevice(userid, deviceid):
    response = {}
    user = User.objects.get(user_id=userid)
    user.current_device_id = deviceid
    user.save()

    response['ok'] = 1
    response['msg'] = '切换成功'
    return response


def renameDevice(newname, deviceid):
    response = {}
    device = Deviceinfo.objects.get(device_id=deviceid)
    device.device_name = newname
    device.save()
    response['msg'] = '重命名成功'
    response['ok'] = 1
    return response

def deleteDevice(deviceid):
    response = {}

    # 检查是否是当前设备
    User.objects.filter(current_device_id=deviceid).update(current_device_id=None)

    # 删除
    device = Deviceinfo.objects.get(device_id=deviceid)
    device.delete()

    response['ok'] = 1
    response['msg'] = '删除成功'
    return response

# 追踪模式开关
def trackModeSwitch(deviceid, status):
    response = {}
    device = Deviceinfo.objects.get(device_id=deviceid)
    track_mode(device.device_aep_id, status)
    response['ok'] = 1
    return response