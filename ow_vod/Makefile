objs := ow_rtsp.o \
				ow_dns.o \
				ow_demux.o \
				ow_draw.o \
				ow_main.o
	
	

SLIB=libtpvod.a

CFG_INC=-I$(INCDIR)\
	-I$(INCDIR)/common\
	-I$(INCDIR)/ecs\
	-I$(INCDIR)/vo\
	-I$(INCDIR)/video\
	-I$(INCDIR)/audio\
	-I$(INCDIR)/demux\
	-I$(INCDIR)/sync\
	-I$(INCDIR)/tde\
	-I$(INCDIR)/video\
	-I$(INCDIR)/jpg\
	-I$(INCDIR)/mixer\
	-I$(INCDIR)/osd\
	-I$(INCDIR)/fb\
	-I$(INCDIR)/pvr\
	-I$(INCDIR)/tde\
	-I$(INCDIR)/vbi\
	-I$(INCDIR)/cursor\
	-I$(INCDIR)/mmz\
	-I$(INCDIR)/vd\
	-I$(INCDIR)/player\
	-I$(INCDIR)/unf\
	-I$(INCDIR)/higo\
	-I$(INCDIR)/unf\
	-I$(INCDIR)/event\
	-I$(TPAPPDIR)/fyf/include

ifndef HI_CFLAGS
HI_CFLAGS= -Wall -O2 -march=armv5te -ggdb -DSFP -static 
endif

target= $(SLIB) 

all: $(target)


$(SLIB): ${objs}
	$(AR)  -rc -o $@ $(objs)

%.o : %.c
	$(CC) $(HI_CFLAGS) -c -fPIC -o "$@" "$<" $(CFG_INC)

clean:
	@rm -f *.o
	rm -f $(target)

	
