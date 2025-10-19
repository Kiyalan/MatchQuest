#!/bin/bash

# 这个文件由 Deekseek 生成，用于快速检测 CMakeLists.txt 的报错 Cannot find source file

# 假设 file_list 变量中包含了由空格和换行分隔的文件列表
file_list=" include/base/userinterface.h source/base/userinterface.cpp
        include/base/level.h source/base/level.cpp
        include/base/playercontroller.h source/base/playercontroller.cpp
        include/base/character.h source/base/character.cpp
        include/base/configuration.h source/base/configuration.cpp
        include/base/emptygraphicsitem.h source/base/emptygraphicsitem.cpp
        include/base/particle.h source/base/particle.cpp
        include/base/actor.h source/base/actor.cpp
        include/world/tilelevel.h source/world/tilelevel.cpp
        include/world/tile.h source/world/tile.cpp
        include/world/prop.h source/world/prop.cpp
        include/world/startmenulevel.h source/world/startmenulevel.cpp
        include/world/singleplayerlevel.h source/world/singleplayerlevel.cpp
        include/world/dualplayerlevel.h source/world/dualplayerlevel.cpp
        include/world/propgenerator.h source/world/propgenerator.cpp
        include/world/hinter.h source/world/hinter.cpp
        include/ui/startmenuui.h source/ui/startmenuui.cpp
        include/ui/singleplayeringameui.h source/ui/singleplayeringameui.cpp
        include/ui/dualplayeringameui.h source/ui/dualplayeringameui.cpp
        include/ui/singleplayergameendui.h source/ui/singleplayergameendui.cpp
        include/ui/singleplayerpausegameui.h source/ui/singleplayerpausegameui.cpp
        include/particle/tileElimination.h source/particle/tileElimination.cpp
        include/player/matcher.h source/player/matcher.cpp
        include/player/matchercontroller.h source/player/matchercontroller.cpp
        include/player/singlematchercontroller.h source/player/singlematchercontroller.cpp
        include/player/dualmatchercontroller.h source/player/dualmatchercontroller.cpp
        include/player/startmenucontroller.h source/player/startmenucontroller.cpp
        resource/resourcestree.qrc
        resource/ui/singleplayeringameui.ui
        resource/ui/singleplayergameendui.ui
        resource/ui/singleplayerpausegameui.ui
        resource/ui/startmenuui.ui"

# 设置IFS，将空格、换行、制表符视为分隔符
IFS=$' \n\t'

# 将文件列表转换为数组
files=($file_list)

# 遍历数组，对每个文件执行file命令
for file in "${files[@]}"; do
    # 如果文件存在，则执行file命令
    if [ -e "$file" ]; then
        file "$file"
    else
        echo "文件不存在: $file"
    fi
done