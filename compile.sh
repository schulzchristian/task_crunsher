#!/bin/bash

rm -rf deploy

scons program=task_crunsher variant=optimized -j 4 

mkdir deploy
cp ./optimized/task_crunsher deploy/

rm -rf ./optimized
