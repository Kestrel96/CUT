#!/bin/bash
pid="$(ps aux | grep cut | grep -vw "grep" | awk '{print $2}')"

if [ "$pid" == "" ]
then
	echo "Did not find CUT running..."
	exit 0
else

	echo "Killing CUT, PID: $pid"
	kill $pid
fi
