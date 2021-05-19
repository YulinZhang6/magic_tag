// pages/firstpage.js
// var QQMapWX = require('../../utils/qqmap-wx-jssdk.js')
// var qqmapsdk = new QQMapWX({
//   key: '***' // 必填
// });

var appdata = getApp().globalData


Page({
  /**
   * 页面的初始数据
   */
  data: {
    /**导航栏按钮设置 */
    list: [{
        "text": "地图",
        "iconPath": "../../../images/map_icon_default.png",
        "selectedIconPath": "../../../images/map_icon_active.png",
        // dot: false
      },
      {
        "text": "设置",
        "iconPath": "../../../images/tabbar_icon_setting_default.png",
        "selectedIconPath": "../../../images/tabbar_icon_setting_active.png",
        // badge: 'New'
      }
    ],
    /****************************第一页面数据****************************/
    /**第一页面收藏位置的左滑按钮设置 */
    slideButtons: [{
      text: '重命名',
    }, {
      type: 'warn',
      text: '删除',
    }],
    /**第一页面SOS位置的左滑按钮设置 */
    sos_slideButtons: [{
      type: 'warn',
      text: '删除',
    }],
    /*第一页面收藏位置重命名弹窗按钮设置 */
    dialogButtons: [{
      text: '取消'
    }, {
      text: '确定'
    }],
    /** 
     * index 导航栏页面 0 为地图页，1 为设置页
     * my_latitude，my_longitude为 人的当前维度 经度
     * dialogShow 控制第一页面重命名弹窗的弹出
     * renameText 是重命名的文本框内容
     * device 当前设备
     */
    index: 0,
    my_latitude: 0,
    my_longitude: 0,
    dialogShow: false,
    renameText: "",
    device: 1234567876543210,
    /****************************第二页面数据****************************/
    headImg: '../../images/Megumi1.jpg',
    operList: [{
      text: '设备管理',
      iconPath: '../../images/device_manage.png'
    }, {
      text: '绑定设备',
      iconPath: '../../images/device_bind.png'
    }, {
      text: '上传图片',
      iconPath: '../../images/picture_upload.png'
    }, {
      text: '追踪模式',
      iconPath: '../../images/track_mode.png'
    }, {
      text: '历史数据',
      iconPath: '../../images/history_data.png'
    }, {
      text: '消息授权',
      iconPath: '../../images/user_allowence.png'
    }, {
      text: '关于我们',
      iconPath: '../../images/about_us.png'
    }],
    bindDeviceDialogShow: false,
    bindAboutDialogShow: false,
  },
  /****************************第一页面相应方法****************************/
  /**
   * 导航栏按钮点击的响应，相同按钮刷新，不同按钮切换页面。
   * 此处记得修改miniprogram_npm中的tabber组件js程序 
   * */
  tabChange(e) {
    // console.log('tab change', e)
    if (this.data.index == e.detail.index) {
      this.onRefresh()
    }
    this.setData({
      index: e.detail.index
    })
  },

  /**
   * 此处根据点击的第一页的按钮 将地图中心重置到点坐标
   */
  onPosCellTab: function (e) {
    console.log(e)
    var id = parseInt(e.currentTarget.dataset.id)
    var type = parseInt(e.currentTarget.dataset.type)
    var new_long = 0
    var new_lati = 0
    if (type == 0) {
      new_long = this.data.pos_json[id].longitude
      new_lati = this.data.pos_json[id].latitude
    } else if (type == 1) {
      new_long = this.data.star_json[id].longitude
      new_lati = this.data.star_json[id].latitude
    } else if (type == 2) {
      new_long = this.data.sos_json.longitude
      new_lati = this.data.sos_json.latitude
    } else if (type == 3) {
      new_long = this.data.current_json.longitude
      new_lati = this.data.current_json.latitude
    }
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
  /**
   * sos按钮左滑删除
   * 这个函数有点效率低下，但是删除还是要重新刷新一下的…… 
   * */
  sosslideButtonTap: function (e) {
    var sos_json = {} //此处为接口，将修改为发送sos解除给服务器
    var sosid = this.data.sos_json.record_id
    wx.request({
      url: appdata.baseUrl + 'deletepoi',
      data: {
        type: 0,
        id: sosid
      },
      success(response) {
        // console.log(response.data)
      }
    })

    this.setData({
      sos_json: sos_json
    })
    this.changeMarkers()
    // this.onLoad()
  },
  /**收藏的按钮左滑删除、重命名响应 */
  slideButtonTap: function (e) {
    // console.log(e)
    var that = this
    var operation = e.detail.index
    var id = e.currentTarget.dataset.id

    if (operation == 0) {
      this.setData({
        needRename: id,
        dialogShow: true
      })
    } else { //删除接口——————

      var temp = new Array()
      var star_id = that.data.star_json[id].star_id
      temp = that.data.star_json
      // console.log(temp,that)
      temp.splice(id, 1)

      wx.request({
        url: appdata.baseUrl + 'deletepoi',
        data: {
          type: 1,
          id: star_id
        },
        success(response) {
          // console.log(response.data)
        }
      })

      this.setData({
        star_json: temp
      })
      this.changeMarkers()

      // this.onLoad()
    }
  },
  /**
   * 根据重命名窗口的文本内容同步修改renameText
   */
  formInputChange: function (e) {
    this.setData({
      renameText: e.detail.value
    })
  },
  /**点击重命名窗口按钮的响应事件 */
  tapDialogButton: function (e) {
    // console.log(e)
    var index = e.detail.index
    var that = this
    var name = this.data.renameText
    /**
     * 下面这个是重命名的接口
     */
    if (index == 1) {
      //若输入文本为空, 无响应
      if (this.data.renameText.length == 0) {
        return;
      }

      wx.request({
        url: appdata.baseUrl + 'renamestar',
        data: {
          starid: that.data.star_json[that.data.needRename].star_id,
          newname: name
        },
        success(response) {
          // console.log(response.data)
        }
      })
      var temp = that.data.star_json
      temp[this.data.needRename].address = this.data.renameText
      this.setData({
        star_json: temp
      })
    }
    //窗口关闭，重命名文本清零，刷新
    this.setData({
      dialogShow: false,
      renameText: ""
    })
    // console.log(this.data)
    // this.onLoad()
  },
  /****************************第二页面相应方法****************************/
  /**点击操作列表的响应 */
  onOperCellTab: function (e) {
    // console.log(e)
    var id = e.currentTarget.dataset.id
    switch (id) {
      case 0:
        wx.navigateTo({
          url: '../manageDevice/manageDevice',
        })
        break;
      case 1:
        this.setData({
          bindDeviceDialogShow: true
        })
        break;
      case 2:
        wx.navigateTo({
          url: '../uploadPic/uploadPic',
        })
        break;
      case 3:
        wx.navigateTo({
          url: '../trackingMode/trackingMode',
        })
        break;
      case 4:
        wx.navigateTo({
          url: '../historyPage/historyPage',
        })
        break;
      case 5:
        wx.requestSubscribeMessage({
          tmplIds: ['hYHy1-QPDTkXmH0W9iu3B-5f7mbQbvIpT_3RLeroAjk'],
          success(res) {
            console.log(res)
          },
          fail(res) {
            console.log(res)
          }
        })
        break;
      case 6:
        this.setData({
          bindAboutDialogShow: true
        })
        break;
      default:
        ;
    }
  },
  /* 绑定设备弹窗的响应*/
  tapBindDeviceDialogButton: function (e) {
    // console.log(e)
    var index = e.detail.index
    var that = this
    /**
     * 下面这个是绑定新设备的接口
     */
    if (index == 1) {
      //若输入文本为空, 无响应,可改进为不是16位数提示
      if (this.data.renameText.length == 0) {
        return;
      }

      wx.request({
        url: appdata.baseUrl + 'binddevice',
        data: {
          userid: appdata.userid,
          newdevice: that.data.renameText
        },
        success(response) {
          console.log(response.data)
          if (response.data.ok == 2) {
            wx.showToast({
              title: response.data.msg,
              icon: 'error',
              duration: 2000
            })
          } else if (response.data.ok == 1) {
            wx.showToast({
              title: response.data.msg,
              icon: 'success',
              duration: 2000
            })
          } else if (response.data.ok == 3) {
            wx.showToast({
              title: response.data.msg,
              icon: 'error',
              duration: 2000
            })
          }
        }
      })

      // console.log(this.data.renameText)
    }
    //窗口关闭，重命名文本清零，不用刷新
    this.setData({
      bindDeviceDialogShow: false,
      renameText: ""
    })
    // this.onLoad()
  },
  tapBindAboutDialogButton: function (e) {
    //窗口关闭
    this.setData({
      bindAboutDialogShow: false,
    })
  },

  // 将添加 请求当前设备，根据设备请求地理数据
  init: function () {
    console.log(appdata)
    //points是根据历史路径划线得到，markers是根据所有点标记得到
    var points = new Array();
    var pos_json = new Array();
    var star_json = new Array();
    var sos_json = new Object();
    var current_json = new Object();
    var that = this
    if (appdata.current_device_id == -1) {
      console.log("无设备")
    } else {
      wx.request({
        url: appdata.baseUrl + 'getcurrentposdata',
        data: {
          deviceid: appdata.current_device_id
        },
        success(response) {
          console.log(response.data)
          points = response.data.points
          star_json = response.data.star_json
          pos_json = response.data.pos_json
          sos_json = response.data.sos_json
          current_json = response.data.current_json

          that.setData({
            pos_json: pos_json,
            star_json: star_json,
            sos_json: sos_json,
            current_json: current_json,
            poi: [{
              points: points,
              width: 10,
              color: "#047CFF",
              arrowLine: true
            }],
            userid: appdata.userid,
            current_device_id: appdata.current_device_id
          })
          //判断是否有紧急情况和收藏的位置

          that.changeMarkers()

        }
      })
    }

    this.setData({
      pos_json: pos_json,
      star_json: star_json,
      sos_json: sos_json,
      current_json: current_json,
      poi: [{
        points: points,
        width: 10,
        color: "#FA6400",
      }],
      userid: appdata.userid,
      current_device_id: appdata.current_device_id
    })

    // this.changeMarkers() //报错但不影响

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
      },
      complete: function (res) {
        wx.hideNavigationBarLoading();
      }
    })
  },

  changeMarkers: function () {
    var markers = new Array();
    var have_sos = false
    var have_star = false
    if (Object.keys(this.data.sos_json).length > 0) {
      markers[0] = this.data.sos_json
      if(Object.keys(this.data.current_json).length > 0){
        markers[1] = this.data.current_json
      }
      have_sos = true
    } else if (Object.keys(this.data.current_json).length > 0) {
      markers[0] = this.data.current_json
    }

    if (this.data.star_json.length > 0) {
      have_star = true
    }
    markers = markers.concat(this.data.pos_json).concat(this.data.star_json)
    this.setData({
      have_star: have_star,
      have_sos: have_sos,
      markers: markers
    })

  },
  /**
   * 生命周期函数--监听页面加载，登录
   */
  onLoad: function (options) {
    var that = this
    wx.login({
      success: res => {
        // console.log(res)
        if (res.code) {
          // 发起网络请求
          wx.request({
            url: appdata.baseUrl + 'login',
            data: {
              code: res.code
            },
            success(response) {
              console.log(response.data)
              appdata.userid = response.data.openid
              appdata.current_device_id = response.data.current_device_id
              that.init()
            }
          })
        } else {
          console.log('登录失败！' + res.errMsg)
        }
      }
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
    this.onRefresh()
  },

  /**刷新响应 */
  onRefresh: function () {
    //在当前页面显示导航条加载动画
    wx.showNavigationBarLoading();
    //显示 loading 提示框。需主动调用 wx.hideLoading 才能关闭提示框
    // wx.showLoading({
    //   title: '刷新中...',
    // })
    this.onLoad()
    this.mapCtx.moveToLocation({
      longitude: this.data.my_longitude,
      latitude: this.data.my_latitude,
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
    // console.log(this.data)
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