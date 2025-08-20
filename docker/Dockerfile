ARG OS=ubuntu
FROM $OS:latest

ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && apt dist-upgrade -y && apt install -y \
   apt-utils apt-transport-https joe lsb-release ca-certificates cmake \
   qmake6 libqt6serialbus6-bin qt6-serialbus-dev qt6-base-private-dev \
   libgtest-dev libx11-dev libxext-dev libdrm-dev libsystemd-dev \
   graphviz pkg-config git make g++ cppcheck cccc fakeroot doxygen valgrind

RUN addgroup dev
RUN adduser --ingroup dev --home /home/dev --gecos "Development user" --disabled-password dev
USER dev
WORKDIR /home/dev
RUN git clone https://github.com/stmork/mrw-track-control-ng.git
