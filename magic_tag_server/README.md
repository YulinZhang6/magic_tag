# magic_tag_server

1. 安装依赖

   ```sh
   pip install -r requirements.txt
   ```
   
2. 启动服务

   ```sh
   python ./manage.py runserver 8001
   ```

3. 目录结构

   ```
   ├─magic_tag_server
   │  └─项目入口，settings.py
   └─my_server
       ├─aep 云平台后端接口
       │  ├─service
       │  │  └─ 业务代码
       │  │  urls.py	路由(aep/...)
       │  └  views.py	api入口
       │─wx 微信小程序后端
       │  ├─service
       │  │  └─ 业务代码
       │  │  urls.py	路由(api/...)
       │  └  views.py	api入口
       │ 
       ├─utils 工具
       │  ├─apis 云平台api
       │  └─指令下发、cv等接口
       └  models.py	公用model (from my_server.models import ...)
   ```


- 数据库迁移(仅第一次)

  ```python
  python manage.py makemigrations
  python manage.py migrate my_server
  ```
