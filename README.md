简介
----
使用 Visual C++ 编写酷Q应用。

文件说明
--------
简单得说明下,vs2017编译的项目只能win10使用，想要低版本的系统使用插件请使用vs2010进行编译<br>

项目主要结构<br>
  CQProjects.sln<br>
      --com.master5.doudizhu<br>
        --cqp.h                               酷q官方SDK <br>
        --appmain.h                           酷q官方SDK<br>
        --appmain.cpp                         酷q官方SDK<br>
        --game.h                              游戏<br>
        --game.cpp                            游戏<br>
        --com.master5.doudizhu.json           酷q官方配置<br>
        --CQP.lib                             酷q官方SDK<br>
      --com.master5.test<br>
        --TestDouDiZhu.cpp                   测试<br>
解决方案下包括两个项目：<br>
com.master5.doudizhu 和 TestDouDiZhu<br>
com.master5.doudizhu 为插件项目 请使用release编译<br>
com.master5.test 为测试项目 请使用debug编译<br>



官方网站
--------
主站：https://cqp.cc

文库：https://d.cqp.me

插件地址：https://cqp.cc/t/32764
