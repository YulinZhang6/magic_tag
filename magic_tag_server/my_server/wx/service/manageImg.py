import datetime
from django.conf import settings
from my_server.models import Deviceimg, Deviceinfo
from my_server.utils.cv import dithering, drawText
from my_server.utils.device_cmd import send_pic_asy

def uploadText(deviceid, text):
    response = {}
    device = Deviceinfo.objects.get(device_id=deviceid)
    imgrecord = Deviceimg(device=device,user_id=device.user_id,
                          add_time=datetime.datetime.now(), image=drawText(text=text))
    imgrecord.save()
    response['ok'] = 1
    response['msg'] = '上传文本成功：'+text
    return response

# 需要加图像处理
def uploadImg(deviceid, img):
    response = {}
    device = Deviceinfo.objects.get(device_id=deviceid)
    imgrecord = Deviceimg(device=device,user_id=device.user_id,
                          add_time=datetime.datetime.now(), image=dithering(img))

    imgrecord.save()
    imgPath = settings.ROOT_URL + imgrecord.image.url
    response['ok'] = 1
    response['path'] = imgPath
    response['msg'] = '上传图片成功：'
    return response


def getImgList(deviceid):
    response = {}
    piclist = []
    idlist = []
    imgrec = Deviceimg.objects.order_by('add_time').filter(device_id=deviceid)
    for record in imgrec:
        idlist.append(record.img_id)
        piclist.append(settings.ROOT_URL + record.image.url)
    response['piclist'] = piclist
    response['idlist'] = idlist
    response['ok'] = 1
    return response

def deleteImg(imgid):
    response = {}
    img = Deviceimg.objects.get(img_id=imgid)
    img.delete()
    response['msg'] = '删除成功'
    response['ok'] = 1
    return response

def displayImg(imgId):
    response = {}
    img = Deviceimg.objects.get(img_id=imgId)

    if img is None or img.image.url is None:
        response['ok'] = 0
        response['msg'] = '图片不存在'
        return response
    device = Deviceinfo.objects.get(device_id=img.device_id)
    send_pic_asy(device.device_aep_id, img.image.url)
    response['ok'] = 1
    response['msg'] = '发送成功'
    return response