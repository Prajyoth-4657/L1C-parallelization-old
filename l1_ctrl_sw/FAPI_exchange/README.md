# FAPI_exchange


## src 

### Makefile to compile l1_dl_tti_request.c

This makefile is used to link and compile the files necessary to run l1_dl_tti_request.c alone in order to populate and use the DL_ttiRequest structure.

#### Usage

The Makefile is present in ```l1_ctrl_sw/FAPI_exchange/src/``` and it is required that the user navigate there first.


Create a directory called "obj/" before running make. This can be done as follows

```
mkdir obj
```

An error is thrown if the file already exists. The makefile can then be used as 

```
make -f Makefile1.txt
```

The executable can then be run as 

```
./testmake
```
