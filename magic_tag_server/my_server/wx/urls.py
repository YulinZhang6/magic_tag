from django.conf.urls import url
from . import views


urlpatterns = [
    url(r'wx_test$', views.wx_test, ),
    url(r'login$', views.login, ),
    url(r'getcurrentposdata$', views.getcurrentposdata, ),
    url(r'renamestar$', views.renamestar, ),
    url(r'deletepoi$', views.deletepoi, ),
    url(r'binddevice$', views.binddevice, ),
    url(r'getdevicelist$', views.getdevicelist, ),
    url(r'changedevice$', views.changedevice, ),
    url(r'renamedevice$', views.renamedevice, ),
    url(r'deletedevice$', views.deletedevice, ),
    url(r'uploadtext$', views.uploadtext, ),
    url(r'uploadimg$', views.uploadimg, ),
    url(r'getimglist$', views.getimglist, ),
    url(r'deleteimg$', views.deleteimg, ),
    url(r'wxsubscribe$', views.wxsubscribe, ),
    url(r'displayimg$', views.displayimg, ),
    url(r'gettrackinfo$', views.gettrackinfo, ),
    url(r'trackmodeswitch$', views.trackmodeswitch, ),
    url(r'gethistorydata$', views.gethistorydata, ),
]
