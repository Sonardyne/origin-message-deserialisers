#!/bin/bash
#
# Script to build all example applications
#

for i in *Demonstrator*; do echo $i; cd $i; cmake -B build; cmake --build build; cd -; done
