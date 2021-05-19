# -*- coding: utf-8 -*-
from math import sin, cos, sqrt, fabs, atan2
from math import pi as PI

# define ellipsoid
a = 6378245.0
f = 1 / 298.3
b = a * (1 - f)
ee = 1 - (b * b) / (a * a)

# 返回列表信息 [lng,lat](经度，纬度)例如：[118.82612975898704, 31.882982622947665]
def reviseGps(origin_lng, origin_lat):
    # 第一次修正
    first_revision_lat = origin_lat//100 + (origin_lat % 100)/60
    first_revision_lng = origin_lng//100 + (origin_lng % 100)/60
    # print('第一次修正:', (first_revision_lng,first_revision_lat))
    # print('第二次修正:', wgs2gcj(first_revision_lng, first_revision_lat))
    # 第二次修正
    return wgs2gcj(first_revision_lng, first_revision_lat)


def outOfChina(lng, lat):
    """check weather lng and lat out of china

    Arguments:
        lng {float} -- longitude
        lat {float} -- latitude

    Returns:
        Bollen -- True or False
    """
    return not (72.004 <= lng <= 137.8347 and 0.8293 <= lat <= 55.8271)


def transformLat(x, y):
    ret = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y + 0.1 * x * y + 0.2 * sqrt(fabs(x))
    ret = ret + (20.0 * sin(6.0 * x * PI) + 20.0 * sin(2.0 * x * PI)) * 2.0 / 3.0
    ret = ret + (20.0 * sin(y * PI) + 40.0 * sin(y / 3.0 * PI)) * 2.0 / 3.0
    ret = ret + (160.0 * sin(y / 12.0 * PI) + 320.0 * sin(y * PI / 30.0)) * 2.0 / 3.0
    return ret


def transformLon(x, y):
    ret = 300.0 + x + 2.0 * y + 0.1 * x * x +  0.1 * x * y + 0.1 * sqrt(fabs(x))
    ret = ret + (20.0 * sin(6.0 * x * PI) + 20.0 * sin(2.0 * x * PI)) * 2.0 / 3.0
    ret = ret + (20.0 * sin(x * PI) + 40.0 * sin(x / 3.0 * PI)) * 2.0 / 3.0
    ret = ret + (150.0 * sin(x / 12.0 * PI) + 300.0 * sin(x * PI / 30.0)) * 2.0 / 3.0
    return ret


def wgs2gcj(wgsLon, wgsLat):
    """wgs coord to gcj

    Arguments:
        wgsLon {float} -- lon
        wgsLat {float} -- lat

    Returns:
        tuple -- gcj coords
    """

    if outOfChina(wgsLon, wgsLat):
        return wgsLon, wgsLat
    dLat = transformLat(wgsLon - 105.0, wgsLat - 35.0)
    dLon = transformLon(wgsLon - 105.0, wgsLat - 35.0)
    radLat = wgsLat / 180.0 * PI
    magic = sin(radLat)
    magic = 1 - ee * magic * magic
    sqrtMagic = sqrt(magic)
    dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * PI)
    dLon = (dLon * 180.0) / (a / sqrtMagic * cos(radLat) * PI)
    gcjLat = wgsLat + dLat
    gcjLon = wgsLon + dLon
    return [gcjLon, gcjLat]


if __name__ == '__main__':
    reviseGps(3153.03249, 11849.27325)
    reviseGps(3153.10362, 11849.26056)