FROM ubuntu:19.10

RUN apt-get update
RUN apt-get install -y software-properties-common
RUN add-apt-repository ppa:kip/pistache-unstable
RUN apt-get update
RUN apt-get install -y gcc
RUN apt-get install -y g++
RUN apt install -y libpistache-dev
RUN apt-get install -y rapidjson-dev
RUN apt-get install -y libpqxx-dev
WORKDIR /
ADD * /

RUN g++ *.cpp -o main.o -lpthread -lpqxx -lpq -lpistache
EXPOSE 8080
CMD ./main.o