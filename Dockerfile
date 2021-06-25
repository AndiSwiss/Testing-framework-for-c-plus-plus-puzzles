FROM ubuntu:latest

RUN apt-get update && apt-get install --yes gcc g++ make libcap-dev libacl1-dev libncurses5-dev gdb

CMD ["bash"]