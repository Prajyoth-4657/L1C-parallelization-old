open_project l1_ctrl_bbu_prj
set_top l1_ctrl_bbu
add_files ./l1_ctrl_bbu.cpp
add_files ./l1_ctrl_bbu_fapi_rx.cpp
add_files ./l1_ctrl_bbu_calc.cpp
add_files ./l1_ctrl_bbu_pop.cpp
add_files -tb ./l1_ctrl_bbu_tb.cpp
open_solution "solution1"
set_part {xczu28dr-ffvg1517-2-e} -tool vivado
create_clock -period 10 -name default
#source "./project/solution1/directives.tcl"
#csim_design
csynth_design
#cosim_design
export_design -format ip_catalog
