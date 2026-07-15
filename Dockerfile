FROM gcc:latest
COPY . /usr/src/myapp
WORKDIR /usr/src/myapp
RUN g++ -O3 server.cpp -o server
CMD ["./server"]
