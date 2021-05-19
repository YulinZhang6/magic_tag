from my_server.models import Starinfo

# 重命名收藏点
def renameStar(starid, newName):
    response = {}
    star = Starinfo.objects.get(star_id=starid)
    star.star_name = newName
    star.save()

    response['ok'] = 1
    response['msg'] = '重命名收藏成功'
    return response