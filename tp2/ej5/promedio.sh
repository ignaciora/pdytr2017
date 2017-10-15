#!/bin/bash
#corre 10 ejecuciones y calcula el promedio de los tiempos de comunicacion

sum=0;
for i in `seq 1 2`; do
  val=$(./fs_client localhost -w ./files/img $i | grep EJ5-A | cut -d '@' -f 2);
  if [ ${#val} -ne 0 ]; then
    echo $val;
  fi
  #sum=$(($sum + $val));
  val=0;
done;
#echo "sumatoria: $sum";
#echo "promedio: $(($sum / 2))";
