
ARM_PLATFORM_PATH = /Developer/Platforms/iPhoneOS.platform/Developer
ARM_ISYSROOT_PATH = $(ARM_PLATFORM_PATH)/SDKs/iPhoneOS3.1.2.sdk/
ARM_CC = $(ARM_PLATFORM_PATH)/usr/bin/g++ -isysroot $(ARM_ISYSROOT_PATH)  -mno-thumb -arch armv7
# -mfloat-abi=softfp -mfpu=neon  

CLANG_CC=clang

CXXFLAGS += -Iinclude -Wall -Wextra -pedantic -Wno-unused -O3 -fstrict-aliasing -ffast-math
CC=g++

SPEC_SRC = $(wildcard spec/*.cpp)
SPEC_OBJ = $(SPEC_SRC:.cpp=.o)

DEFAULT_CC=1



ifeq ($(ARM),1)
	CC=$(ARM_CC)
	CXX=$(ARM_CC)
	DEFAULT_CC=0
endif

ifeq ($(CLANG),1)
	CC=$(CLANG_CC)
	CXX=$(CLANG_CC)
	DEFAULT_CC=0
endif

ifeq ($(DEFAULT_CC),1)
	CXXFLAGS += -msse -msse2 -mfpmath=sse
endif

ifeq ($(ASM),1)
	CC+= -S
	CXX+= -S
endif





all: specsuite
	./specsuite


specsuite: $(SPEC_OBJ)
	$(CXX) $(SPEC_OBJ) -o $@

depend:
	@echo DEP
	@makedepend -Y -- $(CXXFLAGS) -- $(SRCS) $(SPEC_SRC)  > /dev/null 2>&1 
	@$(RM) Makefile.bak

clean:
	rm -f specsuite $(SPEC_OBJ)

# DO NOT DELETE

spec/spec.o: spec/spec.h
spec/spec_main.o: spec/spec.h
spec/spec_vec4f.o: spec/spec_helper.h spec/spec.h include/vectorial/vec4f.h
spec/spec_vec4f.o: include/vectorial/simd.h include/vectorial/config.h
