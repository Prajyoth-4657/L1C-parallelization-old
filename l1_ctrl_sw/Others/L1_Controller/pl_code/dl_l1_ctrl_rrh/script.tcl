open_project dl_l1_ctrl_rrh_prj
set_top l1_ctrl_rrh_pdsch
add_files dl_l1_ctrl_rrh.cpp
add_files dl_l1_ctrl_rrh_calc.cpp
add_files dl_l1_ctrl_rrh_ecpri_rx.cpp
add_files dl_l1_ctrl_rrh_pop.cpp

add_files -tb dl_l1_ctrl_rrh_tb.cpp
open_solution "solution1"
set_part {xczu28dr-ffvg1517-2-e} -tool vivado
create_clock -period 10 -name default
#source "./project/solution1/directives.tcl"
#csim_design
csynth_design
#cosim_design
export_design -format ip_catalog


