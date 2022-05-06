#!/bin/bash

NUM_PACKET_SEND=0
SERVER_PORT=5046
SERVER_ADDR=localhost

while [ $NUM_PACKET_SEND -le 100 ]
do
    echo -n "Test 1" |netcat -u $SERVER_ADDR $SERVER_PORT -c
    NUM_PACKET_SEND=$(( $NUM_PACKET_SEND + 1 ))
    echo "num packet sent: $NUM_PACKET_SEND"

    echo -n "Test 2" |netcat -u $SERVER_ADDR $SERVER_PORT -c
    NUM_PACKET_SEND=$(( $NUM_PACKET_SEND + 1 ))
    echo "num packet sent: $NUM_PACKET_SEND"

    echo -n "Test 3" |netcat -u $SERVER_ADDR $SERVER_PORT -c
    NUM_PACKET_SEND=$(( $NUM_PACKET_SEND + 1 ))
    echo "num packet sent: $NUM_PACKET_SEND"
done
