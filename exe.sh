#!/bin/sh
./jq-linux64 -c {id},{text} /arquivo.json >> /filtrado.txt
echo 'filtrado.txt gerado!'

javac Filtrador.java
java Filtrador filtrado.txt
echo 'filtrado2.txt gerado!'
