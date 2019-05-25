CXXFLAGS = -Wctor-dtor-privacy \
           -Wdelete-non-virtual-dtor \
	   -Wliteral-suffix \
	   -flto \
	   -Wnarrowing \
	   -Wnoexcept \
	   -Wnon-virtual-dtor \
	   -Weffc++ \
	   -Wstrict-null-sentinel \
	   -Woverloaded-virtual \
	   -Wno-pmf-conversions \
	   -Wsign-promo

CPPFLAGS = -pipe \
	   -ggdb3 -Wstrict-overflow=5 -fstack-protector-all \
		 -g \
	   -W -Wall -Wextra \
	   -Wcast-align \
	   -Wcast-qual \
	   -Wconversion \
	   -Wfloat-equal \
	   -Wformat-y2k \
	   -Winit-self \
	   -Winline \
	   -Winvalid-pch \
	   -Wmissing-declarations \
	   -Wmissing-field-initializers \
	   -Wmissing-format-attribute \
	   -Wmissing-include-dirs \
	   -Wmissing-noreturn \
	   -Wnormalized=nfc \
	   -Woverlength-strings \
	   -Wpacked \
	   -Wpadded \
	   -Wpointer-arith \
	   -Wredundant-decls \
	   -Wshadow \
	   -Wsign-compare \
	   -Wstack-protector \
	   -Wstrict-aliasing=2 \
	   -Wundef \
	   -Wunsafe-loop-optimizations \
	   -Wvolatile-register-var \
	   -Wwrite-strings


CFLAGS  = -std=c99 \
	  -Wbad-function-cast \
	  -Wmissing-prototypes \
	  -Wnested-externs \
	  -Wold-style-definition \
	  -Wstrict-prototypes


#------------------------------------------------------------------------------
.PHONY : all clean

#------------------------------------------------------------------------------
all : teste

teste : teste.o grafo.o

#------------------------------------------------------------------------------
clean :
	$(RM) teste *.o
