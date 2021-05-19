// pages/manageDevice.js
import device_json from '../../test_data/static_device'

var appdata = getApp().globalData

Page({

  /**
   * 页面的初始数据
   */
  data: {
    slideButtons: [{
      text: '重命名',
    }, {
      type: 'warn',
      text: '删除',
    }],
    selectIconPath: '../../images/select.png',
    dialogShow: false,
    dialogButtons: [{
      text: '取消'
    }, {
      text: '确定'
    }],
    renameText: "",
  },
  /**
   * 左滑按钮响应事件 删除接口
   */
  slideButtonTap: function (e) {
    console.log(e)
    var operation = e.detail.index
    var id = e.currentTarget.dataset.id
    var that = this
    if (operation == 0) {
      this.setData({
        needRename: id,
        dialogShow: true
      })
    } else { //删除接口
      wx.request({
        url: appdata.baseUrl + 'deletedevice',
        data: {
          deviceid: that.data.devicesList[id].IMEIid,
        },
        success(response) {
          // console.log(response.data)
          // this.onLoad()
        }
      })
      var temp = this.data.devicesList
      temp.splice(id,1)
      var tempindex = this.data.currentIndex
      if(tempindex == id){
        tempindex = -1
      } else if(tempindex > id){
        tempindex -= 1
      }
      this.setData({
        devicesList:temp,
        currentIndex:tempindex
      })
    }
  },
  /**项目点击按钮响应事件 选中设备接口*/
  onDevCellTab: function (e) {
    var id = e.currentTarget.dataset.id
    var that = this
    if (id != this.data.currentIndex) {
      wx.request({
        url: appdata.baseUrl + 'changedevice',
        data: {
          userid: appdata.userid,
          deviceid: that.data.devicesList[id].IMEIid
        },
        success(response) {
          // console.log(response.data)
        }
      })

      this.setData({
        currentIndex: id
      })
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

  tapDialogButton: function (e) {
    // console.log(e)
    var index = e.detail.index
    var that = this
    /**
     * 下面这个是重命名的接口
     */
    if (index == 1) {
      //若输入文本为空, 无响应
      if (this.data.renameText.length == 0) {
        return;
      }
      wx.request({
        url: appdata.baseUrl + 'renamedevice',
        data: {
          deviceid: that.data.devicesList[that.data.needRename].IMEIid,
          newname: that.data.renameText
        },
        success(response) {
          // console.log(response.data)
        }
      })
      
      var temp = this.data.devicesList
      temp[this.data.needRename].name = this.data.renameText
      this.setData({
        devicesList: temp
      })
      
    }
    //窗口关闭，重命名文本清零，刷新
    this.setData({
      dialogShow: false,
      renameText: ""
    })
    // this.onLoad()
  },
  /**
   * 生命周期函数--监听页面加载 获取设备数据
   */
  onLoad: function (options) {
    var device_json = new Object()
    var that = this
    wx.request({
      url: appdata.baseUrl + 'getdevicelist',
      data: {
        userid: appdata.userid
      },
      success(response) {
        // console.log(response.data)
        device_json = response.data.devicelist
        that.setData({
          currentIndex: device_json.currentIndex,
          devicesList: device_json.device_json
        })
      }
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