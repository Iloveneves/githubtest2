objs := ipanel_audio.o \
  ipanel_base.o \
	ipanel_demux.o \
	ipanel_graphics.o \
	ipanel_input.o \
	ipanel_media.o \
	ipanel_nvram.o \
	ipanel_os.o \
	ipanel_osfile.o \
	ipanel_product.o \
	ipanel_socket.o \
	ipanel_sound.o \
	ipanel_tuner.o \
	ipanel_UrlNetCursor.o \
	ipanel_video.o \
	trd_ipanel_init.o 

	
	

SLIB=libtpipanel.a

CFG_INC = -I$(INCDIR)\
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
	-I$(TPAPPDIR)/fyf/include\
	-I$(TPAPPDIR)/fyf/include/dev\
	-I$(TPAPPDIR)/hiadp/include\
	-I$(INCDIR)

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