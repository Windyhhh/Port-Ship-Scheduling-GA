#!/bin/bash

# CPLEX安装脚本 for WSL
echo "开始安装CPLEX Optimization Studio 22.1.0..."
echo ""

# 设置安装目录
INSTALL_DIR="$HOME/cplex"
echo "安装目录: $INSTALL_DIR"
echo ""

# 创建安装目录
mkdir -p "$INSTALL_DIR"
echo "已创建安装目录"
echo ""

# 复制安装文件
INSTALLER="$HOME/cplex_install/cplex_studio2210.linux_x86_64.bin"
if [ -f "$INSTALLER" ]; then
    echo "找到安装文件: $INSTALLER"
    echo ""
    
    # 执行安装
    echo "正在执行安装..."
    echo "请按照提示操作，当要求输入安装路径时，输入: $INSTALL_DIR"
    echo ""
    
    "$INSTALLER"
else
    echo "错误: 找不到安装文件 $INSTALLER"
    echo "请确保安装文件已复制到正确位置"
    exit 1
fi

echo ""
echo "安装完成!"
echo "请检查 $INSTALL_DIR 目录是否有CPLEX文件"
echo ""
