include $(TPAPPDIR)/app_cfg.mak 
tpversion = ui_demo
objs :=ui_com_api.o \
   $(tpversion)/osd/gui_bmplib.o \
	$(tpversion)/osd/gui_event_table.o \
	$(tpversion)/osd/gui_focus_table.o \
	$(tpversion)/osd/gui_fontlib.o \
	$(tpversion)/osd/gui_maintask.o \
	$(tpversion)/osd/gui_osd_init.o \
	$(tpversion)/osd/gui_state_table.o \
	$(tpversion)/osd/gui_string_table.o \
	$(tpversion)/osd/gui_style_data.o \
	$(tpversion)/osd/gui_osd_table.o \
	$(tpversion)/osd/Logo_nosinal.o \
	$(tpversion)/osd/Logo_radio.o \
	$(tpversion)/osd/Logo_music.o \
	$(tpversion)/osd/logo_start.o \
	$(tpversion)/event/ui_app_mainmenu.o \
	$(tpversion)/event/ui_app_fullscreen.o \
	$(tpversion)/event/ui_app_brdcastlist.o \
	$(tpversion)/event/ui_app_doublehelp.o \
	$(tpversion)/event/ui_app_epg.o \
	$(tpversion)/event/ui_app_msg.o \
	$(tpversion)/event/ui_app_sysinfo.o \
	$(tpversion)/event/vod/ui_app_fulltotvod.o \
	$(tpversion)/event/lock/ui_app_chnlock.o \
	$(tpversion)/event/lock/ui_app_menulock.o \
	$(tpversion)/event/lock/ui_app_startuplock.o \
	$(tpversion)/event/lock/ui_app_unlock.o \
	$(tpversion)/event/progmanager/ui_app_chnedit.o \
	$(tpversion)/event/progmanager/ui_app_chnlist.o \
	$(tpversion)/event/progmanager/ui_app_chnsort.o \
	$(tpversion)/event/progmanager/ui_app_editinfo.o \
	$(tpversion)/event/progmanager/ui_app_mainfreqset.o \
	$(tpversion)/event/progmanager/ui_app_manualsearchset.o \
	$(tpversion)/event/progmanager/ui_app_progmanagermenu.o \
	$(tpversion)/event/progmanager/ui_app_searchinfo.o \
	$(tpversion)/event/progmanager/ui_app_searchprog.o \
	$(tpversion)/event/reserve/ui_app_reserve.o \
	$(tpversion)/event/reserve/ui_app_timeout.o \
	$(tpversion)/event/sysset/ui_app_autoinfo.o \
	$(tpversion)/event/sysset/ui_app_avset.o \
	$(tpversion)/event/sysset/ui_app_default.o \
	$(tpversion)/event/sysset/ui_app_languageset.o \
	$(tpversion)/event/sysset/ui_app_netset.o \
	$(tpversion)/event/sysset/ui_app_passwordset.o \
	$(tpversion)/event/sysset/ui_app_progstate.o \
	$(tpversion)/event/sysset/ui_app_stbinfo.o \
	$(tpversion)/event/sysset/ui_app_syssetmenu.o \
	$(tpversion)/event/sysset/ui_app_sysupgrade.o \
	$(tpversion)/event/sysset/ui_app_timezoneset.o \
	$(tpversion)/event/sysset/ui_app_operateguide.o \
	$(tpversion)/event/sysset/ui_app_parentcontrol.o \
	$(tpversion)/event/sysset/ui_app_signalcheck.o \
	$(tpversion)/event/games/ui_app_gamemenu.o \
	$(tpversion)/event/games/ui_app_gamebox.o \
	$(tpversion)/event/games/ui_app_gamerussia.o \
	$(tpversion)/event/games/ui_app_gamesnake.o\
	$(tpversion)/event/effects/ui_app_effectsinfo.o \
	$(tpversion)/event/factorymenu/ui_app_factorymenu.o \
	$(tpversion)/event/factorymenu/ui_app_freqset.o \
	$(tpversion)/event/factorymenu/ui_app_sequence.o \
	$(tpversion)/event/factorymenu/ui_app_otaset.o \
	$(tpversion)/event/factorymenu/ui_app_comsequence.o \
	$(tpversion)/event/ui_app_stbidset.o \
	$(tpversion)/event/ui_app_regionset.o


ifneq "$(BROADCASTSERVICE_TYPE)" "BROADCAST_NO_SERVICE"

	objs += $(tpversion)/event/ui_app_null.o 
endif

ifeq "$(NVOD_TYPE)" "NVOD_YES"
	objs += $(tpversion)/event/nvod/ui_app_nvod.o  
endif


ifeq "$(CA_TYPE)" "DVT_CA"
	ca = dvtca
	objs +=	$(tpversion)/event/dvtca/cacard/ui_app_camenu.o \
			$(tpversion)/event/dvtca/cacard/ui_app_cardstate.o \
			$(tpversion)/event/dvtca/cacard/ui_app_pinnote.o \
			$(tpversion)/event/dvtca/cacard/ui_app_pinset.o \
			$(tpversion)/event/dvtca/cacard/ui_app_verifypin.o \
			$(tpversion)/event/dvtca/cacard/ui_app_rateset.o \
			$(tpversion)/event/dvtca/cacard/ui_app_worktime.o \
			$(tpversion)/event/dvtca/cacard/ui_app_mail.o \
			$(tpversion)/event/dvtca/cacard/ui_app_cardinfo.o \
			$(tpversion)/event/dvtca/cacard/ui_app_getservicer.o \
			$(tpversion)/event/dvtca/cacard/ui_app_entitleinfo.o \
			$(tpversion)/event/dvtca/cacard/ui_app_slot.o \
			$(tpversion)/event/dvtca/cacard/ui_app_ippinfo.o \
			$(tpversion)/event/dvtca/cacard/ui_app_MasterSlave.o \
			$(tpversion)/event/dvtca/caosd/ui_app_ca_card_in_out.o \
			$(tpversion)/event/dvtca/caosd/ui_app_ca_email.o \
			$(tpversion)/event/dvtca/caosd/ui_app_ca_ippv.o \
			$(tpversion)/event/dvtca/caosd/ui_app_ca_osd.o \
			$(tpversion)/event/dvtca/caosd/ui_app_ca_notify.o \
			$(tpversion)/event/dvtca/caosd/ui_app_ca_feeding.o \
			$(tpversion)/event/dvtca/caosd/ui_app_ca_arealock.o 
endif

ifeq "$(CA_TYPE)" "CD_CA"
	ca = cdca
	objs +=	$(tpversion)/event/cdca/cacard/ui_app_camenu.o \
			$(tpversion)/event/cdca/cacard/ui_app_cardpair.o \
			$(tpversion)/event/cdca/cacard/ui_app_cardstate.o \
			$(tpversion)/event/cdca/cacard/ui_app_pinnote.o \
			$(tpversion)/event/cdca/cacard/ui_app_pinset.o \
			$(tpversion)/event/cdca/cacard/ui_app_rateset.o \
			$(tpversion)/event/cdca/cacard/ui_app_worktime.o \
			$(tpversion)/event/cdca/cacard/ui_app_mail.o \
			$(tpversion)/event/cdca/cacard/ui_app_cardinfo.o \
			$(tpversion)/event/cdca/cacard/ui_app_getservicer.o \
			$(tpversion)/event/cdca/cacard/ui_app_entitleinfo.o \
			$(tpversion)/event/cdca/cacard/ui_app_slot.o \
			$(tpversion)/event/cdca/cacard/ui_app_ippinfo.o \
			$(tpversion)/event/cdca/cacard/ui_app_acList.o \
			$(tpversion)/event/cdca/cacard/ui_app_DetitleChkNums.o \
			$(tpversion)/event/cdca/cacard/ui_app_MasterSlave.o \
			$(tpversion)/event/cdca/caosd/ui_app_ca_card_in_out.o \
			$(tpversion)/event/cdca/caosd/ui_app_ca_email.o \
			$(tpversion)/event/cdca/caosd/ui_app_ca_ippv.o \
			$(tpversion)/event/cdca/caosd/ui_app_ca_osd.o \
			$(tpversion)/event/cdca/caosd/ui_app_ca_progress_strip.o \
			$(tpversion)/event/cdca/caosd/ui_app_ca_detitle.o \
			$(tpversion)/event/cdca/caosd/ui_app_ca_notify.o \
			$(tpversion)/event/cdca/caosd/ui_app_ca_feeding.o 
endif

##Check if USB feature is enabled
ifeq "$(USB_TYPE)" "USB_YES"
    objs += $(tpversion)/event/effects/ui_app_effectsmenu.o \
    		$(tpversion)/event/effects/ui_app_usbmanager.o 
endif

##Check if JPEG feature is enabled
ifeq "$(JPEG_TYPE)" "JPEG_YES"
    objs += $(tpversion)/event/effects/ui_app_pictureshow.o \
			$(tpversion)/event/effects/ui_app_photoframe.o 
endif

##Check if PVR feature is enabled
ifeq "$(PVR_TYPE)" "PVR_YES"
    objs += $(tpversion)/event/effects/ui_app_timeshift.o \
			$(tpversion)/event/effects/ui_app_manualrec.o \
			$(tpversion)/event/effects/ui_app_pvrfilelist.o \
			$(tpversion)/event/effects/ui_app_pvrfullscreen.o \
			$(tpversion)/event/effects/ui_app_player.o 

endif

	
SLIB=$(tpversion)/libtpversion.a
DLIB=$(tpversion)/libtpversion.so

ifndef HI_CFLAGS
HI_CFLAGS= -Wall -O2 -march=armv5te -ggdb -DSFP
#HI_CFLAGS= -Wall -static -march=armv5te -g -DHFP
endif

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

target= $(SLIB) $(DLIB)

all: $(target)

$(DLIB): ${objs}
	$(CC) -shared -o $@ $(objs)

$(SLIB): ${objs}
	$(AR)  -rc -o $@ $(objs)

%.o : %.c
	$(CC) $(HI_CFLAGS) -c -fPIC -o "$@" "$<" $(CFG_INC)

clean:
	@rm -f $(tpversion)/../*.o
	@rm -f $(tpversion)/*.o
	@rm -f $(tpversion)/event/*.o
	@rm -f $(tpversion)/osd/*.o
	@rm -f $(tpversion)/event/games/*.o
	@rm -f $(tpversion)/event/$(ca)/cacard/*.o
	@rm -f $(tpversion)/event/$(ca)/caosd/*.o
	@rm -f $(tpversion)/event/lock/*.o
	@rm -f $(tpversion)/event/progmanager/*.o
	@rm -f $(tpversion)/event/reserve/*.o
	@rm -f $(tpversion)/event/sysset/*.o
	@rm -f $(tpversion)/event/effects/*.o
	@rm -f $(tpversion)/event/nvod/*.o
	@rm -f $(tpversion)/event/factorymenu/*.o
	rm -f $(target)
