ARG OS=ubuntu
FROM $OS:latest

ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update
RUN apt-get dist-upgrade -y
RUN apt-get install -y joe lsb-release apt-transport-https ca-certificates
RUN apt-get install -y libqt5serialbus5-dev
RUN apt-get install -y git make g++ cppcheck libgtest-dev cccc fakeroot

RUN addgroup dev
RUN adduser --ingroup dev --home /home/dev --gecos "Development user" --disabled-password dev
USER dev
WORKDIR /home/dev
RUN git clone https://github.com/stmork/mrw-track-control-ng.git
