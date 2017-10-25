#!/bin/bash
#Corre 10 ejecuciones y calcula el promedio de los tiempos de comunicacion

echo "Ejecutando 10 veces";
echo "Los tiempos de comunicacion son los siguientes:";
sum="(0";
for i in `seq 1 10`; do
  val=$(java AskRemote localhost);
  if [ ${#val} -ne 0 ]; then
    echo "$val";
    sum="$sum+$val";
  fi
  val=0;
done;
prom="$sum)/10";
echo "";
echo "Formato de la cuenta por si no esta instalado python:";
echo $prom;
echo "";
echo "Promedio: $(python -c "print '%.6f' % ($prom)")";
