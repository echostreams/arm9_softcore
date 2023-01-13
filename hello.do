vlib work
# compile source files
vcom -work work -2008 arm9_compatiable_code.vhd
vcom -work work -2008 tb.vhd
set generics "-gROM_FILE=dhry/hello.bin"
vsim $generics work.tb
view wave
add wave -hex -r /tb/*

run 500000 ns
quit
