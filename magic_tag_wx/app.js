// app.js
App({
  globalData: {
    userid:"",
    current_device_id:-1,
    baseUrl:"https://your.web.site/"
  },
  onLaunch() {
    var that = this
    // 展示本地存储能力
    const logs = wx.getStorageSync('logs') || []
    logs.unshift(Date.now())
    wx.setStorageSync('logs', logs)
    
    
  }
})