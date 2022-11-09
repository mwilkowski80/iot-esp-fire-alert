#!/usr/bin/env bash

set -e -o pipefail

bindir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd "$bindir"/../
source .env

source /work/iot/platformio/bin/activate
rm -rf ./../../iot-iotcommons/.pio/*
rm -rf .pio/*
pio init --ide clion --board nodemcuv2
pio run
