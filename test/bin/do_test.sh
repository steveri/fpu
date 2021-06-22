#!/bin/bash

if [ "$1" == "--help" ]; then
    echo "Examples:"
    echo "  $0 ADD |& tee ADD.log ; # Results to ADD.results"
    echo "  $0 SUB |& tee SUB.log ; # Results to SUB.results"
    echo "  $0 MUL |& tee MUL.log ; # Results to MUL.results"
    exit
fi
make clean
make build TEST=$1
make run TEST=$1 | tee $1.results
