#!/bin/bash

#java -cp lib/jade.jar:classes jade.Boot -gui -agents thefirst:FirstAgent
java -cp lib/jade.jar:classes jade.Boot -container -container-name Container-1 &
java -cp lib/jade.jar:classes jade.Boot -container -container-name Container-2 &