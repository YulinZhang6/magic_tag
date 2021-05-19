## 事件上报

| **参数名**   | **说明**                       | **类型** |
| :----------- | :----------------------------- | :------- |
| tenantId     | 租户ID                         | string   |
| productId    | 产品ID                         | string   |
| deviceId     | 设备ID                         | string   |
| messageType  | 消息类型=eventReport           | string   |
| imei         | 终端设备识别号                 | string   |
| imsi         | 终端sim卡标识                  | string   |
| deviceSn     | 设备编号                       | string   |
| timestamp    | 时间戳                         | long     |
| eventType    | 事件类型，信息:1,警告:2,故障:3 | int      |
| eventContent | 事件上报数据                   | json     |
| serviceId    | 服务ID                         | int      |
| protocol     | 协议类型                       | string   |

```json
// 上线 
{
    "timestamp":1619523036482,
    "tenantId":"2000033827",
    "protocol":"lwm2m",
    "productId":"15045240",
    "messageType":"deviceOnlineOfflineReport",
    "iccid":"undefined",
    "eventType":1,	// 上线：1，下线：0
    "deviceId":"d01308047f864601a821e02dc328eab4"
}

// 低电量
{
    "timestamp":1619523767263,
    "tenantId":"2000033827",
    "serviceId":1002,
    "protocol":"lwm2m",
    "productId":"15045240",
    "messageType":"eventReport",
    "eventType":2,
    "eventContent":{
        "battery_voltage":1,
        "battery_state":1
    },
    "deviceSn":"",
    "deviceId":"d01308047f864601a821e02dc328eab4",
    "IMSI":"undefined",
    "IMEI":"161941770261953"
}
```



## 数据上报

| **参数名**   | **说明**                                                     | **类型** | **必填** |
| :----------- | :----------------------------------------------------------- | :------- | :------- |
| tenantId     | 租户ID                                                       | string   | Y        |
| productId    | 产品ID                                                       | string   | Y        |
| deviceId     | 设备ID                                                       | string   | Y        |
| messageType  | 消息类型=dataReport                                          | string   | Y        |
| IMEI         | NB终端设备识别号                                             | string   | N        |
| IMSI         | NB终端sim卡标识                                              | string   | N        |
| deviceType   | 设备标识                                                     | string   | N        |
| topic        | 数据上报主题                                                 | string   | Y        |
| assocAssetId | 合作伙伴ID                                                   | string   | N        |
| timestamp    | 时间戳                                                       | long     | Y        |
| upPacketSN   | 上行报文序号                                                 | int      | N        |
| upDataSN     | 数据上报报文序号                                             | int      | N        |
| serviceId    | 服务ID                                                       | string   | N        |
| protocol     | 协议类型                                                     | string   | Y        |
| payload      | 消息负载，非透传消息格式为payload:消息内容JSON；透传消息格式为payload:{"APPdata":"消息内容BASE64编码"} | json     | Y        |

```json
{
    "upPacketSN":-1,
    "upDataSN":-1,
    "topic":"v1/up/ad19",	// 主题
    "timestamp":1619524024049,
    "tenantId":"2000033827",
    "serviceId":10,			// 服务ID
    "protocol":"lwm2m",
    "productId":"15045240",
    "payload":{
        "time":1619524019000,
        "longitude":-174,
        "latitude":-86
    },
    "messageType":"dataReport",
    "deviceType":"",
    "deviceId":"d01308047f864601a821e02dc328eab4",
    "assocAssetId":"",
    "IMSI":"undefined",
    "IMEI":"161941770261953"
}
```

