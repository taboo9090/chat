# Chat Application Makefile README

This Makefile is designed to simplify the building and running process of the Chat Application, a C++ program utilizing TCP sockets for communication. Please note that the application is currently compatible only with Unix-based operating systems, such as Linux or macOS.

## Usage

### 1. Building the Executables

To build the executables for both the server and client, use the following command:

```bash
make build
```

This command compiles the necessary source files and generates the executable files for the server (`server_executable`) and client (`client_executable`).

### 2. Cleaning Up

To delete the generated executables, use the following command:

```bash
make clean
```

This command removes the `server_executable` and `client_executable` files, providing a clean slate for rebuilding.

### 3. Running the Server

To start the server, execute the following command:

```bash
make run-server
```

This command initiates the server executable, allowing it to listen for incoming connections and manage chat communication.

### 4. Running the Client

To initialize the client, use the following command:

```bash
make run-client
```

This command launches the client executable, connecting it to the server and providing a user interface for interaction.

## Important Note

- The Chat Application is currently compatible only with Unix-based operating systems, such as Linux or macOS. Ensure that you are using a supported environment before attempting to build or run the application.

## Contributing

Feel free to contribute to the improvement of Chat Application. If you encounter any issues or have suggestions, please open an issue or submit a pull request.

Happy chatting!
