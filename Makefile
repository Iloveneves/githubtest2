
VERSION = sfp

BASEDIR    = $(shell pwd)/../Hi3110E050
IMAGEDIR   = $(BASEDIR)/../image
TPAPPDIR   = $(shell pwd)
TPLIBDIR = $(TPAPPDIR)/lib
TPVERSIONDIR = $(TPAPPDIR)/tpversion/ui_demo

include app_cfg.mak

ifeq ($(VERSION), sfp)
CROSS_COMPILE = arm-uclibc-linux-
HI_CFLAGS= -Wall -O2 -march=armv5te -ggdb -DSFP
else
CROSS_COMPILE = arm-vfp_uclibc-linux-gnu-
HI_CFLAGS= -Wall -O2 -march=armv5te -ggdb -DHFP
endif


AR      = $(CROSS_COMPILE)ar
AS      = $(CROSS_COMPILE)as
LD      = $(CROSS_COMPILE)ld
CPP     = $(CROSS_COMPILE)cpp
CC      = $(CROSS_COMPILE)gcc
NM      = $(CROSS_COMPILE)nm
STRIP   = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

LIBDIR=$(TPAPPDIR)/hisilib
INCDIR=$(TPAPPDIR)/hisiinclude


APPMODULE = HIADP TPFYF TPMIDDLE TPVERSION


LIB= $(LIBDIR)/libpetimer.a\
     $(LIBDIR)/libhigo.a\
     $(LIBDIR)/libhigoadp.a\
     $(LIBDIR)/libpng.a\
     $(LIBDIR)/libz.a


LINK = STATIC
#LINK = SHARE


ifeq ($(LINK), STATIC)  
LIB+=$(LIBDIR)/libhimpiunf.a\
     $(LIBDIR)/libhiecsunf.a\
     $(LIBDIR)/libhiplayer.a\
     $(LIBDIR)/libavformat.a\
     $(LIBDIR)/libavcodec.a\
     $(LIBDIR)/libavutil.a\
     $(LIBDIR)/libpetimer.a\
     $(LIBDIR)/libmp3dec.a\
     $(LIBDIR)/libhimpi.a\
     $(LIBDIR)/libhiecs.a\
     $(LIBDIR)/libhievent.a\
     $(LIBDIR)/libhimmz.a \
	 $(LIBDIR)/libeffect.a \
	 $(LIBDIR)/libntfs-3g.a
     
TPLIB+=$(TPLIBDIR)/libos.a\
       $(TPLIBDIR)/libca.a\
	   $(TPLIBDIR)/libdev.a\
	   $(TPLIBDIR)/libguiinterpreter.a\
	   $(TPLIBDIR)/libguiwidget.a\
	   $(TPLIBDIR)/libmfs.a\
	   $(TPLIBDIR)/libsys.a\
	   $(TPLIBDIR)/libsql.a\
	   $(TPLIBDIR)/libgui_core.a
	
	  

endif
#	   $(TPLIBDIR)/libipanel.a\
	   $(TPLIBDIR)/Y0422-xindalu-hi3110E-20091119D.a


ifeq ($(LINK), SHARE)   
LIB+=$(LIBDIR)/libhimpiunf.so\
     $(LIBDIR)/libhiecsunf.so\
     $(LIBDIR)/libtest.so\
     $(LIBDIR)/libhiplayer.so\
     $(LIBDIR)/libavformat.so\
     $(LIBDIR)/libavcodec.so\
     $(LIBDIR)/libavutil.so\
     $(LIBDIR)/libpetimer.so\
     $(LIBDIR)/libmp3dec.so\
     $(LIBDIR)/libhimpi.so\
     $(LIBDIR)/libhiecs.so\
     $(LIBDIR)/libhievent.so\
     $(LIBDIR)/libhimmz.so
endif


TPLIB += $(TPLIBDIR)/libhiadp.a \
		 $(TPLIBDIR)/libfyfall.a  \
		 $(TPLIBDIR)/libtpmiddle.a \
		 $(TPLIBDIR)/libtpversion.a

##e2p类型
ifeq "$(BOX_E2PPROM)" "BOX_ENCRYPT_E2PROM"
	HI_CFLAGS += -DBOX_ENCRYPT_E2PROM#
	TPLIB+=$(TPLIBDIR)/libpse_205_api.a
endif

ifeq "$(FLASH_SIZE)" "FLASH_8M"
	HI_CFLAGS += -DFLASH_8M#
endif

##CA类型 
ifeq "$(CA_TYPE)" "IR_CA"
	HI_CFLAGS += -DIR_CA#
endif

ifeq "$(CA_TYPE)" "CTI_CA"
	HI_CFLAGS += -DCTI_CA#
endif

ifeq "$(CA_TYPE)" "DVT_CA"
	HI_CFLAGS += -DDVT_CA#
	TPLIB+=$(TPLIBDIR)/100817_V5200_3110E_NanPing.a
endif

ifeq "$(CA_TYPE)" "CD_CA"
	HI_CFLAGS += -DCD_CA#
	TPLIB+=$(TPLIBDIR)/Y0422-xindalu-Hi3110E-20091228.a
endif

ifeq "$(CA_TYPE)" "NO_CA"
	HI_CFLAGS += -DNO_CA#
endif

##股票类型 
ifeq "$(STOCK_TYPE)" "HTRD_STOCK"
	HI_CFLAGS += -DUSE_HTRD_STOCK#
endif

ifeq "$(STOCK_TYPE)" "NO_STOCK"
	HI_CFLAGS += -DNO_STOCK#
endif

##OTA的控制 
ifeq "$(OTA_TYPE)" "OTA_YES"
	HI_CFLAGS += -DENABLE_OTA#
endif

ifeq "$(OTA_TYPE)" "OTA_IR"
	HI_CFLAGS += -DENABLE_OTA_IR#
endif


## 数据广播的厂家

ifeq "$(BROADCASTSERVICE_TYPE)" "BROADCAST_NO_SERVICE"

	HI_CFLAGS += -DNO_BROADCAST#
	
endif


ifeq "$(BROADCASTSERVICE_TYPE)" "BROADCAST_ENREACH"

	HI_CFLAGS += -DENABLE_BROADCAST_ENREACH#
	APPMODULE += EVM
	TPLIB += $(TPLIBDIR)/lib_evm_oc.a
	TPLIB += $(TPLIBDIR)/lib_evm_browser.a
	TPLIB += $(TPLIBDIR)/libtpevm.a
  
endif

ifeq "$(BROADCASTSERVICE_TYPE)" "BROADCAST_IPANEL"

	HI_CFLAGS += -DENABLE_BROADCAST_IPANEL#
	APPMODULE += IPANEL
	TPLIB     += $(TPLIBDIR)/libipanel.a
	TPLIB     +=  $(TPLIBDIR)/libtpipanel.a

endif

##NVOD
ifeq "$(NVOD_TYPE)" "NVOD_YES"
	HI_CFLAGS += -DENABLE_NVOD#
endif


##Check if USB feature is enabled
ifeq "$(USB_TYPE)" "USB_YES"
    HI_CFLAGS += -DENABLE_USB#
endif


##Check if PVR feature is enabled
ifeq "$(PVR_TYPE)" "PVR_YES"
    HI_CFLAGS += -DENABLE_PVR#
endif

##Check if JPEG feature is enabled
ifeq "$(JPEG_TYPE)" "JPEG_YES"
    HI_CFLAGS += -DENABLE_JPEG#
endif

##MP3 playback option
ifeq "$(MP3_TYPE)" "MP3_YES"
    HI_CFLAGS += -DENABLE_MP3#
endif




export VERSION CROSS_COMPILE BASEDIR  TPAPPDIR TPLIB INCDIR HI_CFLAGS
export AR AS LD CPP CC NM STRIP OBJCOPY OBJDUMP

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
	
objs := tpmain.o \
		tpkey_led_ir.o



ifeq ($(USE_FZVOD), YES)
	CC+= -DUSE_FZVOD
endif

ifeq ($(USE_ENREACH), YES)
	CC+= -DUSE_ENREACH
	APPMODULE += ENREACH
	TPLIB	+= $(TPLIBDIR)/libenreach.a
	TPLIB     +=  $(TPLIBDIR)/libtpenreach.a
endif  

ifeq ($(USE_IPANEL), YES)
	CC+= -DUSE_IPANEL
endif

ifeq ($(USE_FZVOD), YES)
APPMODULE += OW_VOD
TPLIB += $(TPLIBDIR)/libtpvod.a
endif
	
CC+= -D_GNU_SOURCE
 	
#export LIB CFG_INC
target = Hi3110ETpapp



Hi3110ETpapp:$(objs) $(LIB) $(APPMODULE) #HIADP TPFYF TPMIDDLE TPVERSION IPANEL $(TPLIBDIR)/libhiadp.a $(TPLIBDIR)/libfyfall.a  $(TPLIBDIR)/libtpipanel.a  $(TPLIBDIR)/libtpmiddle.a $(TPLIBDIR)/libtpversion.a
	$(CC) $(HI_CFLAGS) -o $@ $(objs) $(CFG_INC) -Wl,--start-group $(LIB) $(TPLIB)  -lpthread -lm -Wl,--end-group
	arm-uclibc-linux-strip $(TPAPPDIR)/Hi3110ETpapp
	cp $(TPAPPDIR)/Hi3110ETpapp /home
	
all: $(target)

%.o : %.c
	$(CC) $(HI_CFLAGS) -c -fPIC -o "$@" "$<" $(CFG_INC)

.PHONY :HIADP
HIADP:
	make -C hiadp
	cp $(TPAPPDIR)/hiadp/libhiadp.* $(TPLIBDIR)/

.PHONY :TPFYF
TPFYF:
	make -C fyf
	cp $(TPAPPDIR)/fyf/libfyfall.* $(TPLIBDIR)/
.PHONY :ENREACH
ENREACH:
	make -C enreach
	cp $(TPAPPDIR)/enreach/libtpenreach.* $(TPLIBDIR)/

.PHONY :IPANEL		
IPANEL:
	make -C ipanel2.0
	cp $(TPAPPDIR)/ipanel2.0/libtpipanel.* $(TPLIBDIR)/

.PHONY :TPMIDDLE	
TPMIDDLE:
	make -C tpmiddle
	cp $(TPAPPDIR)/tpmiddle/libtpmiddle.* $(TPLIBDIR)/

.PHONY :TPVERSION	
TPVERSION:
	make -C tpversion
	cp $(TPVERSIONDIR)/libtpversion.* $(TPLIBDIR)/

.PHONY :EVM
EVM:
	make -C evm
	cp $(TPAPPDIR)/evm/libtpevm.a $(TPLIBDIR)/

.PHONY :OW_VOD
OW_VOD:
	make -C ow_vod
	cp $(TPAPPDIR)/ow_vod/libtpvod.* $(TPLIBDIR)/

.PHONY :install
install:
	cp $(TPAPPDIR)/Hi3110ETpapp $(IMAGEDIR)/app_image/Hi3110ETpapp
	mkcramfs $(IMAGEDIR)/app_image/ $(IMAGEDIR)/app_image.cramfs

.PHONY :enreach_clean
enreach_clean:
	make -C enreach clean

.PHONY :ipanel_clean
ipanel_clean:
	make -C ipanel2.0 clean

.PHONY :tpmiddle_clean
tpmiddle_clean:
	make -C tpmiddle clean

.PHONY :evm_clean
evm_clean:
	make -C evm clean

clean:
	@rm -f *.o
	rm -f $(target)
	make -C hiadp clean
	make -C fyf clean
	make -C tpmiddle clean
	make -C tpversion clean
	make -C enreach clean
	make -C ipanel2.0 clean
	make -C ow_vod clean
	make -C evm clean
	

