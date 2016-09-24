#!/bin/env bash

DIRECTORY=$(pwd)

. /etc/profile

export VS_CONFIGURATION="$1"
shift
export VS_PLATFORM="#1"
shift

cd "$DIRECTORY"

make "$@"