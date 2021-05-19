import os
import sys
DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.append(DIR)
from apis import aep_device_command, aep_device_management
import json
import cv2
from cv import dithering, img_to_hex
import _thread
import time
from store import pic_cache

# ---------------- configs ---------------------
app_key = 'app_key'
app_secret = 'app_secret'
master_key = 'master_key'
product_id = 'product_id'

command = {
    "content": {},
    "deviceId": '',
    "operator": "user",
    "productId": product_id,
    "ttl": 7200,
    "deviceGroupId": 100,
    "level": 1  # 1或2为设备级别,3为设备组级别
}
# ----------------------------------------------
# 发送指令 传入指令dict对象，返回 dict
def send_cmd(command):
    res = aep_device_command.CreateCommand(
        app_key, app_secret, master_key, json.dumps(command))
    return json.loads(res)

def send_pic_cmd(device_aep_id, pic, pic_idx):
    command['deviceId'] = device_aep_id
    command['content'] = {
        "params":
        {
            "pic1": pic,
            "pic_idx": pic_idx
        },
        "serviceIdentifier": "send_pic"   # 服务定义时的服务标识
    }
    res = send_cmd(command)
    # print(res)
    return res

# 发送图片，直接调用这个
def send_pic(device_aep_id, pic_path):
    print(device_aep_id, pic_path)
    if pic_path[0] == '/':
        pic_path = pic_path[1:]
    img = cv2.imread(pic_path, cv2.IMREAD_GRAYSCALE)
    if device_aep_id in pic_cache.keys():
        return False
    pic_cache[device_aep_id] = img_to_hex(img)
    # cv2.imshow('img', img)
    for i in range(49):
        _thread.start_new_thread(send_pic_cmd, (device_aep_id, pic_cache[device_aep_id][i], i))
        time.sleep(0.5)
    time.sleep(1)
    send_pic_cmd(device_aep_id, pic_cache[device_aep_id][49], 49)

# 异步发送图片
def send_pic_asy(device_aep_id, pic_path):
    print('send_pic_asy')
    _thread.start_new_thread(send_pic, (device_aep_id, pic_path))
    return

# 重发图片
def resend_pic(device_aep_id, lost_list):
    if device_aep_id not in pic_cache.keys():
        return False
    # 为空表示完成，不重发
    print(lost_list)
    if len(lost_list) == 0:
        pic_cache.pop(device_aep_id)
        return True

    if lost_list[-1] == 49:
        lost_list.pop()
    for i in lost_list:
        _thread.start_new_thread(send_pic_cmd, (device_aep_id, pic_cache[device_aep_id][i], i))
        time.sleep(0.5)
    time.sleep(1)
    send_pic_cmd(device_aep_id, pic_cache[device_aep_id][49], 49)


# 根据imei查询deviceId
def queryDeviceIdByImei(imei):
    if len(imei) != 15:
        return False
    res = aep_device_management.QueryDeviceList(app_key, app_secret, master_key, product_id, imei, 0, 1)
    json_obj = json.loads(res)
    lst = json_obj['result']['list']
    if len(lst) == 0:
        return False
    return lst[0]['deviceId']

def send_test_cmd(device_aep_id):
    command['deviceId'] = device_aep_id
    command['content'] = {
        "params":
        {
            "test_int": 2,
            "test_bin": "12345678"
        },
        "serviceIdentifier": "test_cmd"   # 服务定义时的服务标识
    }
    res = send_cmd(command)
    print(res)

# 推送天气
def push_weather(device_aep_id, wstr):
    command['deviceId'] = device_aep_id
    command['content'] = {
        "params":
        {
            "weather": wstr,
        },
        "serviceIdentifier": "push_weather"   # 服务定义时的服务标识
    }
    res = send_cmd(command)

# 开启1、关闭0 追踪模式
def track_mode(device_aep_id, status):
    command['deviceId'] = device_aep_id
    command['content'] = {
        "params":
        {
            "act_result": status,
        },
        "serviceIdentifier": "track_mode"   # 服务定义时的服务标识
    }
    res = send_cmd(command)

if __name__ == '__main__':
    device_aep_id = ''
    # img = dithering('3.jpg')
    # pic_list = img_to_hex(img)
    # # cv2.imshow('img', img)
    # for i in range(49):
    #     _thread.start_new_thread(send_pic_cmd, (device_aep_id, pic_list[i], i))
    #     time.sleep(0.5)

    # send_pic_cmd(device_aep_id, pic_list[49], 49)
    # id=queryDeviceIdByImei('')
    # print(id)
    send_pic(device_aep_id, '3.jpg')
    # cv2.waitKey(0)
    # print('')
    # send_test_cmd(device_aep_id)

