# tcl script to init arm and run sdk

fpga -debugdevice devicenr 2 -f cf_adv7511_zc702.bit
connect arm hw
source ps7_init.tcl
ps7_init
init_user
dow cf_adv7511_zc702.elf
run
disconnect 64
exit
