// pages/historyPage/historyPage.js
var appdata = getApp().globalData
import test_json from '../../test_data/static_pos'
var pos_json = test_json.pos_json
Page({

  /**
   * 页面的初始数据
   */
  data: {
    date: '无',
    userid: ""
  },
  onPosCellTab: function (e) {
    console.log(e)
    var id = parseInt(e.currentTarget.dataset.id)
    var new_long = this.data.pos_json[id].longitude
    var new_lati = this.data.pos_json[id].latitude
    
    /**移动到中心，并且回顶部 */
    this.mapCtx.moveToLocation({
      longitude: new_long,
      latitude: new_lati,
      success: (res) => {
        wx.pageScrollTo({
          scrollTop: 0,
          duration: 300
        })
      },
      fail: function (error) {
        console.error(error);
      },
      complete: function (res) {
        // console.log(res);
      }
    })
  },
  bindDateChange: function (e) {
    // console.log('Change:' + e.detail.value)
    var hist_date = e.detail.value
    var that = this
    this.setData({
      date: hist_date
    })
    //接口请求数据，成功后调用setdata
    wx.request({
      url: appdata.baseUrl + 'gethistorydata',
      data: {
        deviceid: appdata.current_device_id,
        date: hist_date
      },
      success(response) {
        // console.log(response.data)
        that.setData({
          pos_json: response.data.pos_json, //移动到选择日期之后
          markers: response.data.pos_json,
          poi: [{
            points: response.data.poi,
            width: 10,
            color: "#047CFF",
            arrowLine: true
          }]
        })
      }
    })    
    

  },

  /**今日日期字符串 */
  getToday: function () {
    var date = new Date();
    var month = date.getMonth() + 1;
    var strDate = date.getDate();
    if (month >= 1 && month <= 9) {
      month = "0" + month;
    }
    if (strDate >= 0 && strDate <= 9) {
      strDate = "0" + strDate;
    }
    var currentdate = date.getFullYear() + '-' + month + '-' + strDate;
    // console.log(currentdate)
    return currentdate
  },
  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    console.log(appdata.userid)
    wx.getLocation({
      type: 'gcj02',
      isHighAccuracy: true,
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
      // current_device_id: appdata.current_device_id
      current_device_id: 1,
      end_day: this.getToday()
    })
  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {
    this.mapCtx = wx.createMapContext('tx_map')
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
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  }
})