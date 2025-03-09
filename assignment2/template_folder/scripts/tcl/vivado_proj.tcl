################################################################################
# Emerging technologies, Systems & Security
#
#   date: Nov 26 2024
#   author: VlJo
################################################################################
# Project to test
#
# launch with: source /home/jvliegen/vc/github/KULeuven-Diepenbeek/course_hwswcodesign_internal/ch2/scripts/tcl/vivado_proj.tcl
################################################################################

set pname "hwswcd_chapter_2"
set path "/home/jvliegen/Documents/courses/KULeuven/sem8_HWSWCD/vivado/"
set srcpath "/home/jvliegen/vc/github/KULeuven-Diepenbeek/course_hwswcodesign_internal/ch2"

set part "xc7z020clg400-1"
set board "tul.com.tw:pynq-z2:part0:1.0"

# delete older versions
cd $path
exec rm -Rf $pname

# create project
create_project $pname $path/$pname -part $part
set_property board_part $board [current_project]
set_property target_language VHDL [current_project]

# suppress messages that say "don't do inidividual add_files/import_files
set_msg_config -suppress -id {Vivado 12-3645} 

# suppress messages like: WARNING: [Vivado 12-3523] Attempt to change 'Component_Name' from 'icap_buffer' to 'icap_buffer' is not allowed and is ignored.
set_msg_config -suppress -id {Vivado 12-3523} 



# TOP LEVEL COMPONENTS
################################################################################
add_files -norecurse $srcpath/hdl/PKG_hwswcd.vhd

add_files -norecurse $srcpath/hdl/alu.vhd
add_files -norecurse $srcpath/hdl/control.vhd
add_files -norecurse $srcpath/hdl/immediate_gen.vhd
add_files -norecurse $srcpath/hdl/reg_file.vhd
add_files -norecurse $srcpath/hdl/riscv.vhd

add_files -norecurse $srcpath/hdl/two_k_bram_dmem.vhd
add_files -norecurse $srcpath/hdl/two_k_bram_imem.vhd

add_files -norecurse $srcpath/hdl/clock_and_reset_pynq.vhd

add_files -norecurse $srcpath/hdl/riscv_microcontroller.vhd

# TESTBENCHES
################################################################################
add_files -fileset sim_1 -norecurse $srcpath/hdl/tb/riscv_microcontroller_tb.vhd


# CONSTRAINT FILES
################################################################################
add_files -fileset constrs_1 -norecurse $srcpath/xdc/riscv_microcontroller_pynq.xdc