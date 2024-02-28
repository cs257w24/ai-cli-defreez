FROM ubuntu:latest

# Avoid warnings by switching to noninteractive
ENV DEBIAN_FRONTEND=noninteractive

# Install C++ development and libcurl tooling
RUN apt-get update && apt-get install -y \
    build-essential \
    gdb \
    git \
    curl \
    libcurl4-openssl-dev \
    nlohmann-json3-dev \
    jq \
    && apt-get clean -y && rm -rf /var/lib/apt/lists/*

# Install Python3 and PIP
RUN apt-get update && apt-get install -y python3 python3-pip && \
    pip3 install --upgrade pip 

# Install wget and ca-certificates, required for downloading and verifying Miniconda
RUN apt-get update && apt-get install -y wget ca-certificates && \
    apt-get clean && rm -rf /var/lib/apt/lists/*


# Install wget and ca-certificates, required for downloading and verifying Miniconda
RUN apt-get update && apt-get install -y zsh sudo && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

# Create a new user 'cs257', set zsh as default shell
RUN useradd -m -s /usr/bin/zsh cs257 && \
    echo "cs257:cs257" | chpasswd && \
    usermod -aG sudo cs257

# 

# Download and install Miniconda
# Run . ~/.conda.sh to start conda
RUN wget --quiet https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh -O ~/miniconda.sh && \
    /bin/bash ~/miniconda.sh -b -p /opt/conda && \
    rm ~/miniconda.sh && \
    ln -s /opt/conda/etc/profile.d/conda.sh /home/cs257/conda.sh

# Switch back to dialog for any ad-hoc use of apt-get
ENV DEBIAN_FRONTEND=dialog

USER cs257

RUN sh -c "$(wget -O- https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" "" --unattended
