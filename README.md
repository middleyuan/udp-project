# udp-project

This project is a simple UDP client-server application that demonstrates the use of the User Datagram Protocol (UDP) for communication between a client and a server. The server listens for incoming messages from the client and responds with an acknowledgment message. The client sends a message to the server and waits for an acknowledgment message.

# Instructions to run the code

Clone the udp-project repository and navigate to the project directory.

```
git clone https://github.com/middleyuan/udp-project.git
cd udp-project
```

## Native

Ensure the build tools and CMake are installed.

```
sudo apt update
sudo apt install -y build-essential cmake
```

Create a build directory and build the project.

```
mkdir build
cd build
cmake ..
make
```
After building, you'll have two executables in the build directory: `server` and `client`, which will be in the `build/server` and `build/client` directories respectively.

To run the server, execute the following command:

```
./server/server
```

Open a new terminal and run the client:

```
./client/client
```

## Docker

Ensure Docker and Docker Compose are installed.

Run the following command to build the Docker image and start the containers:

```
docker-compose up --build
```

Stop and remove the containers with the following command:

```
docker-compose down
```


