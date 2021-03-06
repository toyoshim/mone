CFLAGS  = -V --Werror -mmcs51 --model-large --xram-size 0x1800 --xram-loc 0x0000 --code-size 0xf000 --stack-auto -D_NO_UART0
CC      = sdcc
FLASHER = ./CH55x_python_flasher/chflasher.py
TARGET  = mone
OBJS	= mone.rel controller.rel descriptors.rel settings.rel ch559.rel flash.rel led.rel timer3.rel usb_device.rel

all: $(TARGET).bin

program: all
	$(FLASHER) -w -f $(TARGET).bin

run: program
	sleep 1
	$(FLASHER) -s

clean:
	rm -f *.asm *.lst *.rel *.rst *.sym $(TARGET).bin $(TARGET).ihx $(TARGET).lk $(TARGET).map $(TARGET).mem

%.rel: %.c chlib/*.h *.h
	$(CC) -c $(CFLAGS) $<

%.rel: chlib/%.c chlib/*.h
	$(CC) -c $(CFLAGS) $<

$(TARGET).ihx: $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.bin: %.ihx
	sdobjcopy -I ihex -O binary $< $@
