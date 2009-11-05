
ARM_PLATFORM_PATH = /Developer/Platforms/iPhoneOS.platform/Developer
ARM_ISYSROOT_PATH = $(ARM_PLATFORM_PATH)/SDKs/iPhoneOS3.1.2.sdk/
ARM_CC = $(ARM_PLATFORM_PATH)/usr/bin/g++ -isysroot $(ARM_ISYSROOT_PATH)  -mno-thumb -arch armv7
# -mfloat-abi=softfp -mfpu=neon  

CLANG_CC=clang

CXXFLAGS += -Iinclude -Wall -Wextra -pedantic -Wno-unused -O3 -fstrict-aliasing -ffast-math
CC=g++

SPEC_SRC = $(wildcard spec/*.cpp)
SPEC_OBJ = $(SPEC_SRC:.cpp=.o)

SUFFIX=

DEFAULT_CC=1

ifeq ($(FORCE_SCALAR),1)
	CXXFLAGS+= -DVECTORIAL_FORCED -DVECTORIAL_SCALAR 
	SUFFIX=-scalar
endif

ifeq ($(FORCE_SSE),1)
	CXXFLAGS+= -DVECTORIAL_FORCED -DVECTORIAL_SSE 
	SUFFIX=-sse
endif

ifeq ($(FORCE_GNU),1)
	CXXFLAGS+= -DVECTORIAL_FORCED -DVECTORIAL_GNU 
	SUFFIX=-gnu
endif

ifeq ($(FORCE_NEON),1)
	CXXFLAGS+= -DVECTORIAL_FORCED -DVECTORIAL_NEON
	SUFFIX=-neon
	ARM=1
endif


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





all: specsuite$(SUFFIX)
	./specsuite$(SUFFIX)


full:
	FORCE_SCALAR=1 $(MAKE) clean specsuite-scalar
	FORCE_GNU=1 $(MAKE) clean specsuite-gnu
	FORCE_SSE=1 $(MAKE) clean specsuite-sse
	FORCE_NEON=1 $(MAKE) clean specsuite-neon
	./specsuite-scalar
	./specsuite-sse
	./specsuite-gnu

specsuite$(SUFFIX): $(SPEC_OBJ)
	$(CXX) $(SPEC_OBJ) -o $@

depend:
	@echo DEP
	@makedepend -Y -- $(CXXFLAGS) -- $(SPEC_SRC)  > /dev/null 2>&1 
	@$(RM) Makefile.bak

clean:
	rm -f $(SPEC_OBJ)

realclean: clean
	rm -f specsuite*


# DO NOT DELETE

spec/spec.o: spec/spec.h
spec/spec_main.o: spec/spec.h
spec/spec_vec4f.o: spec/spec_helper.h spec/spec.h include/vectorial/vec4f.h
spec/spec_vec4f.o: include/vectorial/simd.h include/vectorial/config.h
