#!/bin/bash
pid="$(ps aux | grep cut | grep -vw "grep" | awk '{print $2}')"

echo "Killing CUT, PID: $pid"
kill $pid

