#!/usr/bin/env bash

set -e

echo "Starting TCP Server..."
./build/bin/server &
SERVER_PID=$!

trap 'kill $SERVER_PID' EXIT

sleep 2

NUM_CLIENTS=5

echo "Starting $NUM_CLIENTS clients..."

for i in $(seq 1 $NUM_CLIENTS)
do
(
expect <<EOF
spawn ./build/bin/client

expect "Enter the Message for the Server"
send "Hello From Client $i\r"

expect "Enter the Message for the Server"
send "quit\r"

expect eof
EOF
) &
done

wait

echo "All clients completed successfully"