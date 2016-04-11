
include $(TPAPPDIR)/app_cfg.mak 
objs := app_audio.o \
	app_ChnDatabase.o \
	app_clock.o \
	app_com.o \
	app_dba.o \
	app_dba_v2.o \
	app_epg.o \
	app_init.o \
	app_parser.o \
	app_parserBat.o \
	app_parserCat.o \
	app_parserEit.o \
	app_parserNit.o \
	app_parserPat.o \
	app_parserPmt.o \
	app_parserSdt.o \
	app_parserTdt.o \
	app_parserTot.o \
	app_search.o \
	app_signal.o \
	app_ca.o \
	app_sys.o \
	app_parserAdv.o \
	


#app_parserLog.o \

ifeq ($(USE_ENREACH), YES)
	objs += 	app_enrich_vod.o	
endif

ifeq "$(NVOD_TYPE)" "NVOD_YES"
	objs += app_nvod.o 
endif


##Check if USB feature is enabled
ifeq "$(USB_TYPE)" "USB_YES"
    objs += app_usb.o
endif

##Check if JPEG feature is enabled
ifeq "$(JPEG_TYPE)" "JPEG_YES"
    objs += app_picture_decoder.o
endif

##Check if PVR feature is enabled
ifeq "$(PVR_TYPE)" "PVR_YES"
    objs +=  app_pvr.o \
    		app_player.o
endif

ifeq "$(MP3_TYPE)" "MP3_YES"
	#objs += app_player.o
endif


SLIB=libtpmiddle.a
DLIB=libtpmiddle.so

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
	-Iinclude

ifndef HI_CFLAGS
HI_CFLAGS= -Wall -O2 -march=armv5te -ggdb -DSFP
endif

target= $(SLIB) $(DLIB)

all: $(target)

$(DLIB): ${objs}
	$(CC) -shared -o $@ $(objs)

$(SLIB): ${objs}
	$(AR)  -rc -o $@ $(objs)

%.o : %.c
	$(CC) $(HI_CFLAGS) -c -fPIC -o "$@" "$<" $(CFG_INC)

clean:
	@rm -f *.o
	rm -f $(target)
