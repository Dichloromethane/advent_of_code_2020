#! /usr/bin/env bash

sed 's/ *$//;s/ /\n/g' | ./t1.awk
