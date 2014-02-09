#!/bin/bash
# script para deletar a pasta tmp com os arquivos temporários e excluir os Makefile e qrc_*

rm -r ./tmp;

find ./ -name "Makefile" -exec rm {} \;

find ./ -name "qrc_*" -exec rm {} \;

find ./ -name "*.pro.user" -exec rm {} \;


