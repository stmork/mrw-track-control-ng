ARG OS=ubuntu
FROM $OS:latest

ENV DEBIAN_FRONTEND noninteractive
RUN apt update
RUN apt dist-upgrade -y
RUN apt install -y apt-utils apt-transport-https
RUN apt install -y joe lsb-release ca-certificates
RUN apt install -y libqt5serialbus5-dev
RUN apt install -y git make g++ cppcheck libgtest-dev cccc fakeroot

RUN addgroup dev
RUN adduser --ingroup dev --home /home/dev --gecos "Development user" --disabled-password dev
USER dev
WORKDIR /home/dev
RUN git clone https://github.com/stmork/mrw-track-control-ng.git
