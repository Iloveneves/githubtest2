objs := enreach_av.o \
	enreach_base.o \
	enreach_os.o \
	hi_base.o \
	enreach_demx.o \
	enreach_oc.o \
	enreach_open.o \
	enreach_sock.o \
	enreach_osd.o \
	enreach_tuner.o \
	trd_enreach_init.o  

SLIB=libtpevm.a

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
	-I$(TPAPPDIR)/fyf/include/gui\
	-I$(TPAPPDIR)/hiadp/include\
	-I$(TPAPPDIR)/tpmiddle\
	-I$(TPAPPDIR)/tpmiddle/include\
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
	rm -f $(objs)
	rm -f $(target)
