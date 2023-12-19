# Simple make file for a C++ project

# Build the application
build:
	@echo "Building..."
	@g++ -o ./bin/server ./src/server/main.cpp ./src/server/server.cpp -pthread
	@g++ -o ./bin/client ./src/client/main.cpp ./src/client/client.cpp -pthread

# Run the application
run-server:
	@echo "Running..."
	@./bin/server

run-client:
	@echo "Running..."
	@./bin/client

# Clear the binary
clean:
	@echo "Cleaning..."
	@rm -f ./bin/server
	@rm -f ./bin/client