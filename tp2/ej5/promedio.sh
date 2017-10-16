#!/bin/bash
#Corre 10 ejecuciones y calcula el promedio de los tiempos de comunicacion

echo "Ejecutando 10 veces: ./fs_client localhost -w ./files/test (dest_file=1..10)...";
echo "Los tiempos de comunicacion son los siguientes:";
sum="(0";
for i in `seq 1 10`; do
  val=$(./fs_client localhost -w ./files/test $i | grep EJ5-A | cut -d '@' -f 2);
  if [ ${#val} -ne 0 ]; then
    echo "$i - $val";
    sum="$sum+$val";
  fi
  val=0;
done;
prom="$sum)/10";
echo "";
echo "Promedio: $(python -c "print '%.6f' % ($prom)")";
