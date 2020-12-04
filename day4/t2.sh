#! /usr/bin/env bash

sed 's/ *$//;s/ \+/ /g;s/ /\n/g' | ./t2.awk
