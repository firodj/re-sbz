# How to Create the Perprocessed Header

Using pcpp in Python.

Bash Script:

```
#!/bin/bash
#
#

if [[ -z "$1" ]]; then
	echo "please provide header file"
	exit 1
fi

BASENAME=$(basename "$1")

echo "Writing to $BASENAME.i ..."
/c/Python311/Scripts/pcpp.exe \
	-D_MSC_EXTENSIONS \
	-D_INTEGRAL_MAX_BITS=64 \
	-D_MSC_VER=1400 \
	-D_MSC_FULL_VER=140050727 \
	-D_WIN32 \
	-D_M_IX86=600 \
	-D_M_IX86_FP=0 \
	-D_MT \
	-D_UNICODE \
	-D__w64="" \
	-D_WCHAR_T_DEFINED \
	-I./VC/PlatformSDK/Include \
	-I./VC/include \
	"$1" > "$BASENAME.i"
```
