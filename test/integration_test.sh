#!/usr/bin/env bash

set -e

echo "Starting TCP Server..."
./build/bin/server &
SERVER_PID=$!

trap 'kill $SERVER_PID' EXIT

sleep 2

echo "Running TCP Client with Expect (TTY simulation)..."

expect <<'EOF'
spawn ./build/bin/client

expect "Enter the Message for the Server"
send "Hello Server\r"

expect "Enter the Message for the Server"
send "quit\r"

expect eof
EOF

echo "Integration test PASSED"