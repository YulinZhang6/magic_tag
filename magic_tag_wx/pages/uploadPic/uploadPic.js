// pages/uploadPic/uploadPic.js
var appdata = getApp().globalData

Page({

  /**
   * 页面的初始数据
   */
  data: {
    uploadText: "",
    piclist: [],
    dialogShow: false,
    dialogButtons: [{
      text: '映射'
    }, {
      text: '删除'
    }],
  },
  onUploadTextChange: function (e) {
    this.setData({
      uploadText: e.detail.value
    })
  },

  /**上传文本接口 */
  onUploadTextTap: function (e) {
    // console.log(this.data.uploadText)
    var that = this
    var iffail = true
    wx.showLoading({
      title: '上传中',
      mask: true
    })
    wx.request({
      url: appdata.baseUrl + 'uploadtext',
      data: {
        deviceid: appdata.current_device_id,
        text: that.data.uploadText
      },
      success(response) {
        console.log(response.data)
        iffail = false
        if (response.statusCode == 200) {
          that.onLoad()
          wx.hideLoading()
          wx.showToast({
            title: '成功',
            icon: 'success',
            duration: 1500
          })
        } else {
          wx.hideLoading()
          wx.showToast({
            title: '出错了',
            icon: 'error',
            duration: 1500
          })
        }
      }
    })
    setTimeout(function () {
      if (iffail) {
        wx.hideLoading()
        wx.showToast({
          title: '超时了',
          icon: 'error',
          duration: 1500
        })
      }
    }, 8000)
    this.setData({
      uploadText: ""
    })
  },

  onUploadPicTap: function (e) {
    console.log(e)
    this.chooseImage()
  },

  chooseImage: function (e) {
    var that = this;
    var iffail = true
    wx.chooseImage({
      count: 1,
      sizeType: ['original', 'compressed'], // 可以指定是原图还是压缩图，默认二者都有
      sourceType: ['album', 'camera'], // 可以指定来源是相册还是相机，默认二者都有
      success: function (res) {
        wx.showLoading({
          title: '上传中',
          mask: true
        })
        // 返回选定照片的本地文件路径列表，tempFilePath可以作为img标签的src属性显示图片
        wx.uploadFile({
          url: appdata.baseUrl + 'uploadimg',
          filePath: res.tempFilePaths[0],
          name: 'imgfile',
          formData: {
            'deviceid': appdata.current_device_id
          },
          success(res) {
            iffail = false
            if (res.statusCode == 200) {
              that.onLoad()
              wx.hideLoading()
              wx.showToast({
                title: '成功',
                icon: 'success',
                duration: 1500
              })
            } else {
              wx.hideLoading()
              wx.showToast({
                title: '出错了',
                icon: 'error',
                duration: 1500
              })
            }
          },
          fail(res) {
            console.log(data)
          }
        })
        setTimeout(function () {
          if (iffail) {
            wx.hideLoading()
            wx.showToast({
              title: '超时了',
              icon: 'error',
              duration: 1500
            })
          }
        }, 8000)
      }
    })
  },

  previewImage: function (e) {
    wx.previewImage({
      current: e.currentTarget.dataset.id, // 当前显示图片的http链接
      urls: this.data.piclist // 需要预览的图片http链接列表
    })
  },
  // previewImage:function(e){
  //   console.log(e)
  //   this.setData({
  //     galleryShow: true,
  //     galleryIndex: e.currentTarget.dataset.id
  //   })
  // },
  change: function (e) {
    // console.log(e)
    this.setData({
      galleryIndex: e.detail.current
    })
  },
  deleteImg: function (e) {
    console.log(e.currentTarget.dataset.id)
    this.setData({
      needDelete: e.currentTarget.dataset.id,
      dialogShow: true
    })

  },
  //剩下显示接口
  tapDialogButton: function (e) {
    // console.log(e)
    var index = e.detail.index
    var that = this
    /**
     * 下面这个是删除的接口
     */
    if (index == 1) {
      wx.request({
        url: appdata.baseUrl + 'deleteimg',
        data: {
          imgid: that.data.idlist[that.data.needDelete]
        },
        success(response) {
          console.log(response.data)
          that.onLoad()
        }
      })
    }
    /*显示此图片*/
    else {
      wx.request({
        url: appdata.baseUrl + 'displayimg',
        data: {
          imgid: that.data.idlist[that.data.needDelete]
        },
        success(response) {
          console.log(response.data)
          that.onLoad()
        }
      })
    }
    //窗口关闭，刷新
    this.setData({
      dialogShow: false,
    })
  },

  hide: function () {
    this.setData({
      galleryShow: false
    })
  },
  /**
   * 生命周期函数--监听页面加载，图片列表
   */
  onLoad: function (options) {
    var that = this
    this.setData({
      current_device_id:appdata.current_device_id
    })
    wx.request({
      url: appdata.baseUrl + 'getimglist',
      data: {
        deviceid: appdata.current_device_id,
      },
      success(res) {
        console.log(res.data)
        that.setData({
          piclist: res.data.piclist,
          idlist: res.data.idlist
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