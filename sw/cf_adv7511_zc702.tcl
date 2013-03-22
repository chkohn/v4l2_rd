# tcl script to init arm and run sdk

fpga -debugdevice devicenr 2 -f sw/cf_adv7511_zc702.bit
connect arm hw
source sw/ps7_init.tcl
ps7_init
init_user
dow sw/cf_adv7511_zc702.elf
run
disconnect 64
exit
