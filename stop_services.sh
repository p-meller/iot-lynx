#!/bin/sh
cd ./Docker/dev/
sudo docker-compose --env-file ./Docker/dev/.env -p lynx down
cd ../../