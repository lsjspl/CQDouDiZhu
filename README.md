简介
----
使用 Visual C++ 编写酷Q应用。

文件说明
--------
简单得说明下,vs2017编译的项目只能win10使用，想要低版本的系统使用插件请使用vs2010进行编译
项目核心结构(省略头文件)
  CQProjects.sln
      --com.master5.doudizhu
        --cqp.h                               酷q官方SDK 
        --appmain.h                           酷q官方SDK
        --appmain.cpp                         酷q官方SDK
        --game.h                              游戏
        --game.cpp                            游戏
        --com.master5.doudizhu.json           酷q官方配置
        --CQP.lib                             酷q官方SDK
      --TestDouDiZhu
        --TestDouDiZhu.cpp                    测试
解决方案下包括两个项目：
com.master5.doudizhu 和 TestDouDiZhu
com.master5.doudizhu 为插件项目 请使用release编译
TestDouDiZhu 为测试项目 请使用debug编译



官方网站
--------
主站：https://cqp.cc

文库：https://d.cqp.me
