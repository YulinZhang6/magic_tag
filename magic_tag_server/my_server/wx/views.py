from django.views.decorators.http import require_http_methods
from django.http import JsonResponse
from django.http import HttpResponse
from .service.deleteOper import deletePoi
from .service.getdata import getCurrentPosData, getDeviceList, getTrackInfo, getHistoryData
from .service.login import Login
from .service.manageDevice import bindDevice, changeDevice, renameDevice, deleteDevice, trackModeSwitch
from .service.manageImg import uploadText, uploadImg, getImgList, deleteImg, displayImg
from .service.rename import renameStar
import json
import hashlib
# from .service import process

# Create your views here.



@require_http_methods(["GET", "POST"])
def wx_test(request):
    response = {}
    try:
        # json_result = json.loads(request.body)
        # print(request.body)
        # process.decode_json(json_result)
        response['ok'] = 1

    except Exception as e:
        response['msg'] = str(e)
        response['ok'] = 0
    return JsonResponse(response)

# 登录函数
@require_http_methods(["GET"])
def login(request):
    response = {}
    try:
        code = request.GET.get('code')
        # print(code)
        response = Login(code)
    except Exception as e:
        response['msg'] = str(e)
        response['ok'] = 0
    return JsonResponse(response)


# 请求当前的位置数据 包含S0S，收藏等
@require_http_methods(["GET"])
def getcurrentposdata(request):
    response = {}
    try:
        deviceid = request.GET.get('deviceid')
        response = getCurrentPosData(deviceid)
    except Exception as e:
        response['msg'] = str(e)
        response['ok'] = 0
    return JsonResponse(response)

# 重命名收藏点
@require_http_methods(["GET"])
def renamestar(request):
    response = {}
    try:
        starid = request.GET.get('starid')
        newname = request.GET.get('newname')
        response = renameStar(starid, newname)
    except Exception as e:
        response['msg'] = str(e)
        response['ok'] = 0
    return JsonResponse(response)

# 删除点位
@require_http_methods(["GET"])
def deletepoi(request):
    response = {}
    try:
        type = request.GET.get('type')
        id = request.GET.get('id')
        response = deletePoi(type, id)
    except Exception as e:
        response['msg'] = str(e)
        response['ok'] = 0
    return JsonResponse(response)


# 绑定设备
@require_http_methods(["GET"])
def binddevice(request):
    response = {}
    try:
        userid = request.GET.get('userid')
        newdevice = request.GET.get('newdevice')
        response = bindDevice(userid, newdevice)
    except Exception as e:
        response['msg'] = str(e)
        response['ok'] = 0
    return JsonResponse(response)


# 获取设备列表
@require_http_methods(["GET"])
def getdevicelist(request):
    response = {}
    try:
        userid = request.GET.get('userid')
        response = getDeviceList(userid)
    except Exception as e:
        response['msg'] = str(e)
        response['ok'] = 0
    return JsonResponse(response)

# 改变选中设备
@require_http_methods(["GET"])
def changedevice(request):
    response = {}
    try:
        userid = request.GET.get('userid')
        deviceid = request.GET.get('deviceid')
        response = changeDevice(userid, deviceid)
    except Exception as e:
        response['msg'] = str(e)
        response['ok'] = 0
    return JsonResponse(response)


# 设备重命名
@require_http_methods(["GET"])
def renamedevice(request):
    response = {}
    try:
        deviceid = request.GET.get('deviceid')
        newname = request.GET.get('newname')
        response = renameDevice(newname, deviceid)
    except Exception as e:
        response['msg'] = str(e)
        response['ok'] = 0
    return JsonResponse(response)

# 设备删除
@require_http_methods(["GET"])
def deletedevice(request):
    response = {}
    try:
        deviceid = request.GET.get('deviceid')
        response = deleteDevice(deviceid)
    except Exception as e:
        response['msg'] = str(e)
        response['ok'] = 0
    return JsonResponse(response)

# 上传文本
@require_http_methods(["GET"])
def uploadtext(request):
    response = {}
    try:
        deviceid = request.GET.get('deviceid')
        text = request.GET.get('text')
        response = uploadText(deviceid, text)
    except Exception as e:
        response['msg'] = str(e)
        response['ok'] = 0
    return JsonResponse(response)

# 上传图片
@require_http_methods(["POST"])
def uploadimg(request):
    response = {}
    try:
        deviceid = request.POST.get('deviceid')
        img = request.FILES['imgfile']
        print(deviceid, img, type(img))
        # deviceid = request.GET.get('deviceid')

        response = uploadImg(deviceid, img)
    except Exception as e:
        response['msg'] = str(e)
        response['ok'] = 0
    return JsonResponse(response)

# 获得图片列表
@require_http_methods(["GET"])
def getimglist(request):
    response = {}
    try:
        deviceid = request.GET.get('deviceid')
        response = getImgList(deviceid)
    except Exception as e:
        response['msg'] = str(e)
        response['ok'] = 0
    return JsonResponse(response)

# 图片删除
@require_http_methods(["GET"])
def deleteimg(request):
    response = {}
    try:
        imgid = request.GET.get('imgid')
        response = deleteImg(imgid)
    except Exception as e:
        response['msg'] = str(e)
        response['ok'] = 0
    return JsonResponse(response)

# 微信签名验证
def checkSignature(signature, timestamp, nonce):
    TOKEN = 'luzy666zylcaicaicai'
    if not signature or not timestamp or not nonce:
        return False
    tmp_str = "".join(sorted([TOKEN, timestamp, nonce]))
    tmp_str = hashlib.sha1(tmp_str.encode('UTF-8')).hexdigest()
    if tmp_str == signature:
        return True
    else:
        return False

# 微信校验
@require_http_methods(["GET"])
def wxsubscribe(request):
    response = ""
    try:
        signature = request.GET.get('signature')
        timestamp = request.GET.get('timestamp')
        nonce = request.GET.get('nonce')
        echostr = request.GET.get('echostr')
        if checkSignature(signature, timestamp, nonce):
            response = echostr
    except Exception as e:
        response = str(e)
    return HttpResponse(response)

# 显示图片
@require_http_methods(["GET"])
def displayimg(request):
    response = {}
    try:
        imgid = request.GET.get('imgid')
        response = displayImg(imgid)
    except Exception as e:
        response['msg'] = str(e)
        response['ok'] = 0
    return JsonResponse(response)

# 历史数据
@require_http_methods(["GET"])
def gethistorydata(request):
    response = {}
    try:
        deviceid = request.GET.get('deviceid')
        date = request.GET.get('date')
        response = getHistoryData(deviceid, date)
    except Exception as e:
        response['msg'] = str(e)
        response['ok'] = 0
    return JsonResponse(response)

# 获取追踪模式定位
@require_http_methods(["GET"])
def gettrackinfo(request):
    response = {}
    try:
        deviceid = request.GET.get('deviceid')
        response = getTrackInfo(deviceid)
    except Exception as e:
        response['msg'] = str(e)
        response['ok'] = 0
    return JsonResponse(response)

# 追踪模式开关
@require_http_methods(["GET"])
def trackmodeswitch(request):
    response = {}
    try:
        deviceid = request.GET.get('deviceid')
        status = request.GET.get('status')
        response = trackModeSwitch(deviceid, status)
    except Exception as e:
        response['msg'] = str(e)
        response['ok'] = 0
    return JsonResponse(response)