#!/bin/bash

set -e

echo "Starting TCP Server..."

./build/bin/server &
SERVER_PID=$!

# Give server time to start
sleep 2

echo "Running TCP Client (sending message to server)..."

# Send message to client via stdin
OUTPUT=$(echo "Hello Server" | ./build/bin/client)

echo "Client Output:"
echo "$OUTPUT"

echo "Stopping server..."
kill $SERVER_PID

echo "Integration test PASSED"