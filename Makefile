.SECONDARY:
.DEFAULT_GOAL		:= all
TARGET			= Review4.exe
Version			= MakeDebug
COMPILER		:= C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\bin\cl.exe
LINKER			:= C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\bin\link.exe
OPTIONS			+= /O2 /Ob2 /Oi /Ot /Oy /GT /GL  /GF /Gm /EHsc /MD /Gy /W3 /nologo /c /Zi /TP /fp:fast /errorReport:prompt -Zm1024 
PREPROCESSOR_		+= "WIN32" "NDEBUG" "_CONSOLE" "_CRT_SECURE_NO_WARNINGS" "_SCL_SECURE_NO_WARNINGS" "_UNICODE" "UNICODE"
PREPROCESSOR		= $(foreach i, $(PREPROCESSOR_), /D \"$(i)\")
OUTPUT_DIR		= $(Version)
SRC_FILE		= $(wildcard *.cpp)
OBJECTS			= $(SRC_FILE:%.cpp=$(Version)/%.o)
PCH_H			= stdafx.h
PCH			= $(OUTPUT_DIR)/stdafx.h.gch
MICROSOFT_SDK_INC_DIR	= "C:\\Program Files\\Microsoft SDKs\\Windows\\v6.0A\\Include"
BOOST_INC_DIR		= D:\\MyCots\\boost\\boost_1_61_0
BOOST_LIB_DIR		= D:\\MyCots\\boost\\boost_1_61_0\\stage\lib
INC_DIS			= $(foreach i, $(BOOST_INC_DIR) $(MICROSOFT_SDK_INC_DIR), -I \"$i\")
CXXFLAGS		+= $(INC_DIS) -DWIN32 -DNDEBUG
LDFLAGS			+= -L $(BOOST_LIB_DIR)


 #/I "D:\MyCots\boost\boost_1_61_0" 

 #/Yc"stdafx.h" /Fp"x64\Release\Review4.pch" 
 #/Fo"x64\Release\\" /Fd"x64\Release\vc90.pdb"
 


.PHONY: all clean test

all: clean $(TARGET)

$(TARGET) : $(OBJECTS)
	@echo "==> LINKING..."
	$(LINK.cc) -o $@ $^
	@echo "	DONE"

$(OUTPUT_DIR)/%.o : %.cpp
	@echo "==> COMPILING $<"
	$(COMPILE.cc) -o $@ $<
	@echo "	DONE"

$(OBJECTS): $(PCH)

$(PCH): $(PCH_H)
	@echo "==> Generating PCH $@"
	$(COMPILE.cc) $@ $^

clean:
	@$(RM) $(TARGET) $(OBJECTS) $(PCH)

x = "a" "b" "c"
y = $(foreach i, $(x), /D \"$i\")

test:
	@echo $(INC_DIS)


