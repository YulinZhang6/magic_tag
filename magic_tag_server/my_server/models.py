import time

from django.db import models


class User(models.Model):
    user_id = models.CharField(max_length=30, primary_key=True)
    user_name = models.CharField(max_length=255, blank=True, null=True)
    user_session = models.CharField(max_length=255, blank=True, null=True)
    current_device_id = models.CharField(max_length=255,null=True, blank=True)
    class Meta:
        db_table = 'user'


class Deviceinfo(models.Model):
    user = models.ForeignKey('User', models.CASCADE, blank=True, null=True)
    device_id = models.CharField(primary_key=True, max_length=50)
    device_name = models.CharField(max_length=255, blank=True, null=True)
    bind_time = models.DateTimeField(blank=True, null=True)
    device_aep_id = models.CharField(max_length=50)
    class Meta:
        db_table = 'deviceinfo'


class Devicerecord(models.Model):
    device = models.ForeignKey(Deviceinfo, models.SET_NULL, blank=True, null=True)
    longitude = models.FloatField(blank=True, null=True)
    latitude = models.FloatField(blank=True, null=True)
    address = models.CharField(max_length=255, blank=True, null=True)
    humidity = models.IntegerField(blank=True, null=True)
    temperature = models.FloatField(blank=True, null=True)
    report_time = models.DateTimeField(blank=True, null=True)
    report_type = models.IntegerField(blank=True, null=True)
    message_type = models.IntegerField(blank=True, null=True)   # serviceId
    record_id = models.AutoField(primary_key=True)

    class Meta:
        db_table = 'devicerecord'

# 上传的原始图像
def origin_image(instance, filename):
    ext = filename.split('.')[-1]
    seconds = round(time.time())
    return 'origin/{0}_{1}.{2}'.format(instance.device.device_id, seconds, ext)

class Deviceimg(models.Model):
    img_id = models.AutoField(primary_key=True)
    user = models.ForeignKey('User', models.CASCADE, blank=True, null=True)
    device = models.ForeignKey(
        'Deviceinfo', models.SET_NULL, blank=True, null=True)
    add_time = models.DateTimeField(blank=True, null=True)
    image = models.ImageField(
        upload_to=origin_image, default="origin/default.jpg")

    class Meta:
        db_table = 'deviceimg'


class Starinfo(models.Model):
    star_id = models.AutoField(primary_key=True)
    device = models.ForeignKey(
        Deviceinfo, models.SET_NULL, blank=True, null=True)
    latitude = models.FloatField(blank=True, null=True)
    longitude = models.FloatField(blank=True, null=True)
    star_name = models.CharField(max_length=255, blank=True, null=True)
    add_time = models.DateTimeField(blank=True, null=True, auto_now_add=True)
    address = models.CharField(max_length=255, blank=True, null=True)
    class Meta:
        db_table = 'starinfo'
