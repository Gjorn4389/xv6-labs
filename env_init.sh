#!/bin/bash

# 检查是否已经初始化
if [ -f "~/.xv6_initialized" ]; then
    echo "xv6 env has been initialized, just exec bash"
    exec /bin/bash
fi

# 配置阿里云镜像源
sed -i "s@http://.*archive.ubuntu.com@http://mirrors.aliyun.com@g" /etc/apt/sources.list.d/ubuntu.sources
sed -i "s@http://.*security.ubuntu.com@http://mirrors.aliyun.com@g" /etc/apt/sources.list.d/ubuntu.sources

# 更新和安装软件包
apt update
apt install -y ca-certificates

# 配置为https
sed -i "s@http://mirrors.aliyun.com@https://mirrors.aliyun.com@g" /etc/apt/sources.list.d/ubuntu.sources
apt update

apt install -y \
    git \
    build-essential \
    gdb-multiarch \
    qemu-system-misc \
    gcc-riscv64-linux-gnu \
    binutils-riscv64-linux-gnu \
    curl \
    wget \
    vim

# 配置Git安全目录
git config --global --add safe.directory /xv6-labs-2024

# 配置全局ignore
GLOBAL_GITIGNORE="$HOME/.gitignore_global"

cat > "$GLOBAL_GITIGNORE" << 'EOF'
.vscode/
.idea/
EOF

git config --global core.excludesfile $GLOBAL_GITIGNORE

# 标记初始化完成
touch ~/.xv6_initialized

echo "xv6 env has been initialized!"
exec /bin/bash
