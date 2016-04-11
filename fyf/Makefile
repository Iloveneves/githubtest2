
include $(TPAPPDIR)/app_cfg.mak 

objs := os/fyf_os_porting.o \
	sql/sql_driver.o \
	mfs/mfs_driver.o \
	dev/fyf_gui_main.o \
	gui/gui_device_hisi.o \
	sys/fyf_sys_porting.o \
	dev/fyf_tuner_porting.o \
	dev/fyf_demux_porting.o \
	dev/fyf_av_porting.o \
	dev/fyf_dev_porting.o \
	dev/fyf_osd_porting.o \
	jpeg/fyf_jpeg_porting.o


ifeq "$(NVOD_TYPE)" "NVOD_YES"
	objs += nvod/nvod_core.o 
endif

##Check if USB feature is enabled
ifeq "$(USB_TYPE)" "USB_YES"
    objs += file/fyf_file_porting.o 
endif

#ifeq "$(JPEG_TYPE" "ENABLE_JPEG"
#	objs += jpeg/fyf_jpeg_porting.o
#endif
			

ifeq "$(PVR_TYPE)" "PVR_YES"
	objs += pvr/fyf_pvr_porting.o
endif
	
ifneq "$(CA_TYPE)" "NO_CA"
	objs += dev/fyf_descr.o \
			dev/fyf_smc_porting.o \
			ca/fyf_ca_porting.o 
endif
ifeq "$(BOX_E2PPROM)" "BOX_ENCRYPT_E2PROM"
    objs += dev/pse_205_api.o 
endif


ifeq "$(CA_TYPE)" "CD_CA"
	objs += ca/cdca/cdca_api.o \
		ca/cdca/cdca_card.o \
		ca/cdca/cdca_childcard.o \
		ca/cdca/cdca_entitle.o \
		ca/cdca/cdca_flash.o \
		ca/cdca/cdca_ippv.o \
		ca/cdca/cdca_lockservice.o \
		ca/cdca/cdca_mm.o \
		ca/cdca/cdca_os.o \
		ca/cdca/cdca_osd.o \
		ca/cdca/cdca_other.o \
		ca/cdca/cdca_securitycontrol.o \
		ca/cdca/cdca_ts.o
endif	

ifeq "$(CA_TYPE)" "DVT_CA"
	objs += ca/ca_dvt/ca_dvt_api.o \
		ca/ca_dvt/ca_dvt_arealock.o \
		ca/ca_dvt/ca_dvt_card.o \
		ca/ca_dvt/ca_dvt_ipp.o \
		ca/ca_dvt/ca_dvt_os.o \
		ca/ca_dvt/ca_dvt_osd.o \
		ca/ca_dvt/ca_dvt_securitycontrol.o \
		ca/ca_dvt/ca_dvt_sto.o \
		ca/ca_dvt/ca_dvt_ts.o \
		ca/ca_dvt/ca_dvt_urgentBrct.o 
endif	

ifeq "$(CA_TYPE)" "TF_CA"
	objs += ca/tfca/tfca_api.o \	
		ca/tfca/tfca_card.o \
		ca/tfca/tfca_childcard.o \
		ca/tfca/tfca_entitle.o \
		ca/tfca/tfca_flash.o \
		ca/tfca/tfca_ippv.o \
		ca/tfca/tfca_lockservice.o \
		ca/tfca/tfca_mm.o \
		ca/tfca/tfca_os.o \
		ca/tfca/tfca_osd.o \
		ca/tfca/tfca_other.o \
		ca/tfca/tfca_securitycontrol.o \
		ca/tfca/tfca_ts.o 
endif
	
	

SLIB=libfyfall.a
DLIB=libfyfall.so

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
	rm -f $(objs)
	rm -f $(target)




