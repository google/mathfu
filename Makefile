
CXX?=g++
CLANG_CC=clang
CLANG_CXX=clang++

IPHONE_PLATFORM_PATH = /Developer/Platforms/iPhoneOS.platform/Developer
IPHONE_ISYSROOT_PATH = $(IPHONE_PLATFORM_PATH)/SDKs/iPhoneOS4.2.sdk/
IPHONE_CC = $(IPHONE_PLATFORM_PATH)/usr/bin/g++ -isysroot $(IPHONE_ISYSROOT_PATH)   -arch armv7
# -mfloat-abi=softfp -mfpu=neon  

#CXXFLAGS += -Iinclude -O0
#CXXFLAGS += -g -Iinclude -Wall -Wextra -pedantic -Wno-unused -O3 -fstrict-aliasing -Wstrict-aliasing=2 -ffast-math 
CXXFLAGS += -Iinclude -Wall -Wextra -pedantic -Wno-unused -O3 -fstrict-aliasing -Wstrict-aliasing=2 -ffast-math  -D__extern_always_inline=inline

SPEC_SRC = $(wildcard spec/*.cpp)
SPEC_OBJ = $(SPEC_SRC:.cpp=.o)

BENCH_SRC = $(wildcard bench/*.cpp)
BENCH_OBJ = $(BENCH_SRC:.cpp=.o)
BENCH_ASM = $(patsubst %.cpp,asm$(SUFFIX)/%.S,$(BENCH_SRC))

SUFFIX=

DEFAULT_CC=1

ifeq ($(FORCE_SCALAR),1)
	CXXFLAGS+= -DVECTORIAL_FORCED -DVECTORIAL_SCALAR
	SUFFIX=-scalar
endif

ifeq ($(FORCE_SSE),1)
	CXXFLAGS+= -DVECTORIAL_FORCED -DVECTORIAL_SSE -msse -msse2 -mfpmath=sse
	SUFFIX=-sse
endif

ifeq ($(FORCE_GNU),1)
	CXXFLAGS+= -DVECTORIAL_FORCED -DVECTORIAL_GNU 
	#-msse -msse2 -mfpmath=sse
	SUFFIX=-gnu
endif

ifeq ($(FORCE_NEON),1)
	CXXFLAGS+= -DVECTORIAL_FORCED -DVECTORIAL_NEON
	SUFFIX=-neon
	ARM=1
endif


ifeq ($(ARM),1)
ifeq ($(shell uname -s),Darwin)
	CC=$(IPHONE_CC)
	CXX=$(IPHONE_CC)
endif
#	CXXFLAGS+= -mcpu=cortex-a8 
	CXXFLAGS+= -mno-thumb -mfloat-abi=softfp -mfpu=neon
	DEFAULT_CC=0
endif

ifeq ($(CLANG),1)
	CC=$(CLANG_CC)
	CXX=$(CLANG_CXX)
	DEFAULT_CC=0
endif

ifeq ($(DEFAULT_CC),1)
#	CXXFLAGS += -msse -msse2 -mfpmath=sse
endif

ifeq ($(ASM),1)
	CC+= -S
	CXX+= -S
endif

BUILDDIR=build$(SUFFIX)
SPEC_OBJ := $(addprefix $(BUILDDIR)/,$(SPEC_OBJ))
BENCH_OBJ := $(addprefix $(BUILDDIR)/,$(BENCH_OBJ))
SILENT=@
MKDIR=mkdir -p
PATH_SEPARATOR=/

$(BUILDDIR)/%.o: %.cpp
	@echo CXX $<
	$(SILENT) $(MKDIR) $(subst /,$(PATH_SEPARATOR),$(dir $@))
	$(SILENT) $(COMPILE.cc) -o $@ $<



.PHONY: all
all: specsuite$(SUFFIX)
	./specsuite$(SUFFIX)


.PHONY: full
full:
	@clear
	@echo FULL COMPILE at `date +%H:%M:%S`
#	FORCE_SCALAR=1 $(MAKE) clean 
	@FORCE_SCALAR=1 $(MAKE)  specsuite-scalar
#	FORCE_GNU=1 $(MAKE) clean 
	@FORCE_GNU=1 $(MAKE)  specsuite-gnu
#	FORCE_SSE=1 $(MAKE) clean 
	@FORCE_SSE=1 $(MAKE)  specsuite-sse
#	FORCE_NEON=1 $(MAKE) clean 
#	FORCE_NEON=1 $(MAKE) specsuite-neon
	@./specsuite-scalar
	@./specsuite-sse
	@./specsuite-gnu

specsuite$(SUFFIX): $(SPEC_OBJ)
	@echo LINK $@
	@$(CXX) $(LDFLAGS) $^ -o $@

.PHONY: depend
depend:
	@echo DEP
	@makedepend -Y -- $(CXXFLAGS) -- $(SPEC_SRC) $(BENCH_SRC) -p$(BUILDDIR)/ > /dev/null 2>&1 
	@$(RM) Makefile.bak

define asm-command
@mkdir -p $(dir asm$(SUFFIX)/$(1))
$(CXX) $(CXXFLAGS) -S $(1) -o asm$(SUFFIX)/$(1).S

endef

bench-asm: $(BENCH_SRC)
	$(foreach p,$(BENCH_SRC),$(call asm-command,$(p)))

benchmark$(SUFFIX): $(BENCH_OBJ) bench-asm
	$(CXX) $(BENCH_OBJ) -o $@

.PHONY: bench-full
bench-full:
	FORCE_SCALAR=1 $(MAKE) benchmark-scalar
	FORCE_GNU=1 $(MAKE) benchmark-gnu
	FORCE_SSE=1 $(MAKE) benchmark-sse
#	FORCE_NEON=1 $(MAKE) clean 
#	FORCE_NEON=1 $(MAKE) benchmark-neon
	./benchmark-scalar
	./benchmark-sse
	./benchmark-gnu

.PHONY: clean
clean:
	rm -f $(SPEC_OBJ) $(BENCH_OBJ) benchmark$(SUFFIX) specsuite$(SUFFIX) 
	rm -rf asm$(SUFFIX)

.PHONY: realclean
realclean: clean
	rm -f specsuite*
	rm -rf build*


.PHONY: update_spec
update_spec:
	./tools/update_spec.rb spec/spec_*.cpp

ifeq ($(MAKECMDGOALS),export)
ifeq ($(origin to),undefined)
$(error to not set, like  make export to=/foo/bar)
endif
endif

.PHONY: export
export:
	$(SILENT) git archive --format tar master | tar x -C $(to)


include/vectorial/vec2f.h include/vectorial/vec3f.h include/vectorial/vec4f.h: include/vectorial/simd4f.h
include/vectorial/simd4f.h: include/vectorial/simd4f_scalar.h
include/vectorial/simd4f.h: include/vectorial/simd4f_neon.h
include/vectorial/simd4f.h: include/vectorial/simd4f_gnu.h
include/vectorial/simd4f.h: include/vectorial/simd4f_sse.h
include/vectorial/simd4f.h: include/vectorial/simd4f_scalar.h
include/vectorial/simd4f.h: include/vectorial/config.h
include/vectorial/simd4x4f.h: include/vectorial/simd4f.h
include/vectorial/simd4x4f.h: include/vectorial/simd4x4f_scalar.h
include/vectorial/simd4x4f.h: include/vectorial/simd4x4f_neon.h
include/vectorial/simd4x4f.h: include/vectorial/simd4x4f_gnu.h
include/vectorial/simd4x4f.h: include/vectorial/simd4x4f_sse.h
include/vectorial/simd4x4f.h: include/vectorial/config.h
spec/spec_helper.h: include/vectorial/simd4x4f.h include/vectorial/simd4f.h include/vectorial/vec4f.h include/vectorial/vec3f.h include/vectorial/vec2f.h
spec/spec.cpp: spec/spec.h
spec/spec_main.cpp: spec/spec.h
spec/spec_simd4f.cpp: spec/spec_helper.h
spec/spec_simd4x4f.cpp: spec/spec_helper.h
spec/spec_vec2f.cpp: spec/spec_helper.h
spec/spec_vec3f.cpp: spec/spec_helper.h
spec/spec_vec4f.cpp: spec/spec_helper.h

$(BUILDDIR)/spec/spec_simd4f.o: \
  include/vectorial/simd4x4f.h include/vectorial/simd4f.h \
  include/vectorial/simd4f_scalar.h include/vectorial/simd4f_neon.h \
  include/vectorial/simd4f_gnu.h include/vectorial/simd4f_sse.h \
  include/vectorial/config.h

$(BUILDDIR)/spec/spec_simd4x4f.o: \
  include/vectorial/simd4x4f.h include/vectorial/simd4f.h \
  include/vectorial/simd4f_scalar.h include/vectorial/simd4f_neon.h \
  include/vectorial/simd4f_gnu.h include/vectorial/simd4f_sse.h \
  include/vectorial/simd4x4f_scalar.h include/vectorial/simd4x4f_neon.h \
  include/vectorial/simd4x4f_gnu.h include/vectorial/simd4x4f_sse.h include/vectorial/config.h
  
$(BUILDDIR)/spec/spec_vec2f.o $(BUILDDIR)/spec/spec_vec3f.o $(BUILDDIR)/spec/spec_vec4f.o: \
  include/vectorial/simd4x4f.h include/vectorial/simd4f.h \
  include/vectorial/vec4f.h include/vectorial/vec3f.h include/vectorial/vec2f.h \
  include/vectorial/simd4f_scalar.h include/vectorial/simd4f_neon.h \
  include/vectorial/simd4f_gnu.h include/vectorial/simd4f_sse.h \
  include/vectorial/simd4x4f_scalar.h include/vectorial/simd4x4f_neon.h \
  include/vectorial/simd4x4f_gnu.h include/vectorial/simd4x4f_sse.h include/vectorial/config.h





# DO NOT DELETE

$(BUILDDIR)/spec/spec.o: spec/spec.h
$(BUILDDIR)/spec/spec_main.o: spec/spec.h
$(BUILDDIR)/spec/spec_mat4f.o: spec/spec_helper.h spec/spec.h
$(BUILDDIR)/spec/spec_mat4f.o: include/vectorial/simd4f.h
$(BUILDDIR)/spec/spec_mat4f.o: include/vectorial/config.h
$(BUILDDIR)/spec/spec_mat4f.o: include/vectorial/simd4f_gnu.h
$(BUILDDIR)/spec/spec_mat4f.o: include/vectorial/simd4f_common.h
$(BUILDDIR)/spec/spec_mat4f.o: include/vectorial/vec4f.h include/vectorial/vec3f.h
$(BUILDDIR)/spec/spec_mat4f.o: include/vectorial/vec2f.h
$(BUILDDIR)/spec/spec_mat4f.o: include/vectorial/simd4x4f.h
$(BUILDDIR)/spec/spec_mat4f.o: include/vectorial/simd4f.h
$(BUILDDIR)/spec/spec_mat4f.o: include/vectorial/simd4x4f_gnu.h
$(BUILDDIR)/spec/spec_mat4f.o: include/vectorial/mat4f.h
$(BUILDDIR)/spec/spec_simd4f.o: spec/spec_helper.h spec/spec.h
$(BUILDDIR)/spec/spec_simd4f.o: include/vectorial/simd4f.h
$(BUILDDIR)/spec/spec_simd4f.o: include/vectorial/config.h
$(BUILDDIR)/spec/spec_simd4f.o: include/vectorial/simd4f_gnu.h
$(BUILDDIR)/spec/spec_simd4f.o: include/vectorial/simd4f_common.h
$(BUILDDIR)/spec/spec_simd4f.o: include/vectorial/vec4f.h include/vectorial/vec3f.h
$(BUILDDIR)/spec/spec_simd4f.o: include/vectorial/vec2f.h
$(BUILDDIR)/spec/spec_simd4f.o: include/vectorial/simd4x4f.h
$(BUILDDIR)/spec/spec_simd4f.o: include/vectorial/simd4f.h
$(BUILDDIR)/spec/spec_simd4f.o: include/vectorial/simd4x4f_gnu.h
$(BUILDDIR)/spec/spec_simd4f.o: include/vectorial/mat4f.h
$(BUILDDIR)/spec/spec_simd4x4f.o: spec/spec_helper.h spec/spec.h
$(BUILDDIR)/spec/spec_simd4x4f.o: include/vectorial/simd4f.h
$(BUILDDIR)/spec/spec_simd4x4f.o: include/vectorial/config.h
$(BUILDDIR)/spec/spec_simd4x4f.o: include/vectorial/simd4f_gnu.h
$(BUILDDIR)/spec/spec_simd4x4f.o: include/vectorial/simd4f_common.h
$(BUILDDIR)/spec/spec_simd4x4f.o: include/vectorial/vec4f.h
$(BUILDDIR)/spec/spec_simd4x4f.o: include/vectorial/vec3f.h
$(BUILDDIR)/spec/spec_simd4x4f.o: include/vectorial/vec2f.h
$(BUILDDIR)/spec/spec_simd4x4f.o: include/vectorial/simd4x4f.h
$(BUILDDIR)/spec/spec_simd4x4f.o: include/vectorial/simd4f.h
$(BUILDDIR)/spec/spec_simd4x4f.o: include/vectorial/simd4x4f_gnu.h
$(BUILDDIR)/spec/spec_simd4x4f.o: include/vectorial/mat4f.h
$(BUILDDIR)/spec/spec_vec2f.o: spec/spec_helper.h spec/spec.h
$(BUILDDIR)/spec/spec_vec2f.o: include/vectorial/simd4f.h
$(BUILDDIR)/spec/spec_vec2f.o: include/vectorial/config.h
$(BUILDDIR)/spec/spec_vec2f.o: include/vectorial/simd4f_gnu.h
$(BUILDDIR)/spec/spec_vec2f.o: include/vectorial/simd4f_common.h
$(BUILDDIR)/spec/spec_vec2f.o: include/vectorial/vec4f.h include/vectorial/vec3f.h
$(BUILDDIR)/spec/spec_vec2f.o: include/vectorial/vec2f.h
$(BUILDDIR)/spec/spec_vec2f.o: include/vectorial/simd4x4f.h
$(BUILDDIR)/spec/spec_vec2f.o: include/vectorial/simd4f.h
$(BUILDDIR)/spec/spec_vec2f.o: include/vectorial/simd4x4f_gnu.h
$(BUILDDIR)/spec/spec_vec2f.o: include/vectorial/mat4f.h
$(BUILDDIR)/spec/spec_vec3f.o: spec/spec_helper.h spec/spec.h
$(BUILDDIR)/spec/spec_vec3f.o: include/vectorial/simd4f.h
$(BUILDDIR)/spec/spec_vec3f.o: include/vectorial/config.h
$(BUILDDIR)/spec/spec_vec3f.o: include/vectorial/simd4f_gnu.h
$(BUILDDIR)/spec/spec_vec3f.o: include/vectorial/simd4f_common.h
$(BUILDDIR)/spec/spec_vec3f.o: include/vectorial/vec4f.h include/vectorial/vec3f.h
$(BUILDDIR)/spec/spec_vec3f.o: include/vectorial/vec2f.h
$(BUILDDIR)/spec/spec_vec3f.o: include/vectorial/simd4x4f.h
$(BUILDDIR)/spec/spec_vec3f.o: include/vectorial/simd4f.h
$(BUILDDIR)/spec/spec_vec3f.o: include/vectorial/simd4x4f_gnu.h
$(BUILDDIR)/spec/spec_vec3f.o: include/vectorial/mat4f.h
$(BUILDDIR)/spec/spec_vec4f.o: spec/spec_helper.h spec/spec.h
$(BUILDDIR)/spec/spec_vec4f.o: include/vectorial/simd4f.h
$(BUILDDIR)/spec/spec_vec4f.o: include/vectorial/config.h
$(BUILDDIR)/spec/spec_vec4f.o: include/vectorial/simd4f_gnu.h
$(BUILDDIR)/spec/spec_vec4f.o: include/vectorial/simd4f_common.h
$(BUILDDIR)/spec/spec_vec4f.o: include/vectorial/vec4f.h include/vectorial/vec3f.h
$(BUILDDIR)/spec/spec_vec4f.o: include/vectorial/vec2f.h
$(BUILDDIR)/spec/spec_vec4f.o: include/vectorial/simd4x4f.h
$(BUILDDIR)/spec/spec_vec4f.o: include/vectorial/simd4f.h
$(BUILDDIR)/spec/spec_vec4f.o: include/vectorial/simd4x4f_gnu.h
$(BUILDDIR)/spec/spec_vec4f.o: include/vectorial/mat4f.h
$(BUILDDIR)/bench/add_bench.o: bench/bench.h include/vectorial/vec4f.h
$(BUILDDIR)/bench/bench.o: bench/bench.h include/vectorial/config.h
$(BUILDDIR)/bench/dot_bench.o: bench/bench.h include/vectorial/vec4f.h
$(BUILDDIR)/bench/matrix_bench.o: bench/bench.h include/vectorial/simd4x4f.h
$(BUILDDIR)/bench/matrix_bench.o: include/vectorial/simd4f.h
$(BUILDDIR)/bench/matrix_bench.o: include/vectorial/simd4x4f_gnu.h
$(BUILDDIR)/bench/quad_bench.o: bench/bench.h include/vectorial/simd4x4f.h
$(BUILDDIR)/bench/quad_bench.o: include/vectorial/simd4f.h
$(BUILDDIR)/bench/quad_bench.o: include/vectorial/simd4x4f_gnu.h
