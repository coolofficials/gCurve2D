# Setup NVIDIA Container Toolkit (https://github.com/NVIDIA/nvidia-docker)
# docker build -t gcurve --network host --build-arg USER_NAME=$USER --build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g) .
# docker run --rm -it --gpus all -v $HOME:$HOME --name asdf gcurve bash

# NOTE:          vvvvvv this should match host's CUDA version (check `nvidia-smi`)
FROM nvidia/cuda:11.2.0-devel-ubuntu20.04
ARG DEBIAN_FRONTEND=noninteractive

# weird hack from https://vsupalov.com/docker-shared-permissions/
ARG USER_NAME
ARG USER_ID
ARG GROUP_ID
RUN addgroup --gid $GROUP_ID $USER_NAME
RUN adduser --disabled-password --gecos '' --uid $USER_ID --gid $GROUP_ID $USER_NAME

RUN apt-get update

# dev tools
RUN apt-get install -y software-properties-common && \
    add-apt-repository ppa:neovim-ppa/unstable && \
    apt-get update
RUN apt-get install -y build-essential make unzip autoconf automake libtool cmake g++ pkg-config
RUN apt-get install -y python3 python3-pip python3-dev
RUN apt-get install -y git wget curl gdb neovim htop

# tmux
RUN apt-get install -y libevent-dev ncurses-dev bison
RUN wget https://github.com/tmux/tmux/releases/download/3.2/tmux-3.2.tar.gz && \
    tar xzf tmux-3.2.tar.gz && \
    cd tmux-3.2 && \
    ./configure && make install -j4

# dependencies
RUN apt-get install -y libgl1-mesa-dev libglew-dev freeglut3-dev


USER $USER_NAME
WORKDIR /home/$USER_NAME