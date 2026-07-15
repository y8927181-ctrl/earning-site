FROM gcc:latest
COPY . /usr/src/myapp
WORKDIR /usr/src/myapp
RUN g++ -O3 website.cpp -o server
CMD ["./server"]
