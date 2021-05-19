from my_server.models import Devicerecord, Starinfo


def deletePoi(postype, id):
    postype = int(postype)
    response = {}
    # sos
    if postype == 0:
        sos = Devicerecord.objects.get(record_id=id)
        all_sos = Devicerecord.objects.filter(device_id=sos.device_id, report_type=1)
        all_sos.delete()
    elif postype == 1:
        star = Starinfo.objects.get(star_id=id)
        star.delete()

    response['ok'] = 1
    response['msg'] = '删除成功'
    return response