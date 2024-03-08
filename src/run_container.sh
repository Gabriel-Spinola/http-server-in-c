#!/bin/bash

docker compose down
docker image rm http-server
docker build -t http-server .

if [ $? -eq 0 ]; then
    docker compose up
else
    echo "Build failed. Aborting Docker Compose execution."
fi