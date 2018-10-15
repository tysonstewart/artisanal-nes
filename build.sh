#!/usr/bin/env sh

BUILD_DIR=build
SOURCE_DIR=src
LIB_DIR=lib
RES_DIR=res
CONFIG_DIR=config

mkdir -p ${BUILD_DIR}
rm ${BUILD_DIR}/Hudl.nes

cc65 -o ${BUILD_DIR}/main.s -Oi ${SOURCE_DIR}/main.c --add-source
ca65 -o ${BUILD_DIR}/crt0.o ${LIB_DIR}/NESLibrary/crt0.s
ca65 -o ${BUILD_DIR}/main.o ${BUILD_DIR}/main.s
ld65 -v -o ${BUILD_DIR}/Hudl.nes -C ${CONFIG_DIR}/nrom_128_horz.cfg ${BUILD_DIR}/crt0.o ${BUILD_DIR}/main.o ${LIB_DIR}/NESLibrary/nes.lib

rm ${BUILD_DIR}/*.o
rm ${BUILD_DIR}/main.s
