FROM debian:jessie 

RUN apt-get update && apt-get -y install locales nasm llvm clang gcc qemu make ssh

WORKDIR /root
VOLUME ["/root/so-2018-1c/TP2"]
CMD ["bash"]