FROM ubuntu:24.04

RUN apt-get update && \
    apt-get install -y g++ nlohmann-json3-dev && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY backend ./backend
COPY frontend ./frontend

RUN g++ -std=c++20 backend/src/*.cpp \
    -Ibackend/include \
    -Ibackend \
    -o TransitNova \
    -pthread

EXPOSE 8080

CMD ["./TransitNova"]