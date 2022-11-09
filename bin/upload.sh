#!/usr/bin/env bash

set -e -o pipefail

bindir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd "$bindir"/../
source .env

"$bindir"/rebuild.sh
source /work/iot/platformio/bin/activate
pio run -t upload
