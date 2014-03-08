TMPDIR ?= /tmp
SDK_HOME ?= /opt/vertica/sdk
SHELL = /bin/bash
CC=g++
TGT_DIR ?= UDF/ISNUMERIC/build
VSQL=/opt/vertica/bin/vsql


build: build/isnumeric.so
## See targets below for actual build logic

clean:
        rm -rf UDF

install: build

uninstall:
        $(VSQL) -f UDF/ISNUMERIC/ddl/uninstall.sql

.PHONY: build clean install uninstall test

## Actual build target
build/isnumeric.so: isnumeric.cpp $(SDK_HOME)/include/Vertica.cpp $(SDK_HOME)/in
clude/BuildInfo.h
        mkdir -p UDF/ISNUMERIC/build
        mkdir -p UDF/ISNUMERIC/ddl
        mv install.sql UDF/ISNUMERIC/ddl
        mv uninstall.sql UDF/ISNUMERIC/ddl
        # Note that we link boost statically
        $(CC) -D HAVE_LONG_INT_64 -I $(SDK_HOME)/include -Wall -shared -Wno-unus
ed-value -fPIC -o $(TGT_DIR)/isnumeric.so isnumeric.cpp $(SDK_HOME)/include/Vert
ica.cpp
        $(VSQL) -f UDF/ISNUMERIC/ddl/install.sql
        #Function Deployed into the database successfully
