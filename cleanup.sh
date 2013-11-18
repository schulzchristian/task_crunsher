#!/bin/bash


scons program=task_crunsher variant=optimized -j 4 -c 

rm -rf deploy
