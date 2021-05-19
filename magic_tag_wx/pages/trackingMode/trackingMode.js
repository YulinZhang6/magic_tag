// pages/trackingMode/trackingMode.js
var QQMapWX = require('../../utils/qqmap-wx-jssdk.js')
var qqmapsdk = new QQMapWX({
  key: '***' // 必填
});
var appdata = getApp().globalData
var degree = 0
var motionTimeoutID=0
Page({

  /**
   * 页面的初始数据
   */
  data: {
    switchStatus: false,
    rotateDeg: 0,
    my_latitude: 0,
    my_longitude: 0,
  },
  writeMotionDegree:function(){
    motionTimeoutID=setTimeout(this.writeMotionDegree, 1000);
    this.setData({
      rotateDeg: degree
    })
  }
  ,
  switchChange: function (e) {
    console.log(e.detail)
    var that = this
    var num = 0
    /*开启 关闭追踪模式 */
    if (e.detail.value) {
      /**通知开启追踪 */
      wx.request({
        url: appdata.baseUrl + 'trackmodeswitch',
        data: {
          deviceid: appdata.current_device_id,
          status: 1
        },
        success(response) {}
      })
      this.setData({
        switchStatus: true
      })
      wx.startLocationUpdate({
        success: function (res) {
          wx.onLocationChange(that.LocationChange)
          that.traceOn()
        }
      })
      wx.startDeviceMotionListening({
        success: function () {
            wx.onDeviceMotionChange((result) => {
              degree = 360 - result.alpha
            })
            that.writeMotionDegree()
        }
      })
    } else {
      /**通知关闭追踪*/
      wx.offLocationChange()
      wx.stopLocationUpdate()
      wx.offDeviceMotionChange()
      wx.stopDeviceMotionListening()
      wx.request({
        url: appdata.baseUrl + 'trackmodeswitch',
        data: {
          deviceid: appdata.current_device_id,
          status: 0
        },
        success(response) {
          // console.log(response.data)
        }
      })
      this.setData({
        switchStatus: false,
        markers: [],
        rotateDeg: 0
      })
      clearTimeout(this.data.timeOutID)
      clearTimeout(motionTimeoutID)
    }
  },

  /*获取数据&&数据赋值&&计算直线距离*/
  traceOn: function () {
    var that = this
    var timeOutID = setTimeout(this.traceOn, 5000)
    var targetLoc = new Object()
    wx.request({
      url: appdata.baseUrl + 'gettrackinfo',
      data: {
        deviceid: appdata.current_device_id
      },
      success(response) {
        targetLoc = response.data.data
        if (Object.keys(targetLoc).length > 0) {
          //计算距离
          qqmapsdk.calculateDistance({
            mode: 'straight',
            from: {
              latitude: that.data.my_latitude,
              longitude: that.data.my_longitude
            },
            to: [{
              latitude: targetLoc.latitude,
              longitude: targetLoc.longitude
            }],
            sig: 'wmQGXiKZFNJcVUP9dCUXRIkxA5JzbsRn',
            success: function (res) { //成功后的回调
              // console.log(res);
              var res = res.result;
              var distance = res.elements[0].distance
              var distance_str = ''
              if (distance < 1000) {
                distance_str = distance + '米'
              } else {
                distance_str = (distance / 1000).toFixed(1) + '公里'
              }
              that.setData({ //设置并更新distance数据
                distance: distance_str,
                markers: [targetLoc],
              });
            }
          });
        }
      }
    })



    this.setData({
      timeOutID: timeOutID,
    })
    // console.log(this.data.my_latitude, this.data.my_longitude)
  },
  /**将位置变更数据更新 */
  LocationChange: function (res) {
    // console.log('location change', res)
    this.setData({
      my_latitude: res.latitude,
      my_longitude: res.longitude,
    })
  },
  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    wx.getLocation({
      type: 'gcj02',
      success: (res) => {
        this.setData({
          my_latitude: res.latitude,
          my_longitude: res.longitude,
        })
      },
      fail: function (error) {
        console.error(error);
      }
    })


    this.setData({
      userid: appdata.userid,
      current_device_id: appdata.current_device_id,
      // current_device_id: 1,
    })
  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {

  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {

  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {

  },

  /**
   * 生命周期函数--监听页面卸载 发送关闭信息
   */
  onUnload: function () {
    wx.offLocationChange()
    wx.stopLocationUpdate()
    wx.offDeviceMotionChange()
    wx.stopDeviceMotionListening()
    wx.request({
      url: appdata.baseUrl + 'trackmodeswitch',
      data: {
        deviceid: appdata.current_device_id,
        status: 0
      },
      success(response) {}
    })
    clearTimeout(this.data.timeOutID)
  },

})