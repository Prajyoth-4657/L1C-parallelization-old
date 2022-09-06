open_project sim_dl_l1_ctrl_rrh
set_top ecpri_sim_tx
add_files sim_dl_l1_ctrl_rrh.cpp

open_solution "solution1"
set_part {xczu28dr-ffvg1517-2-e} -tool vivado
create_clock -period 10 -name default
#source "./project/solution1/directives.tcl"
#csim_design
csynth_design
#cosim_design
export_design -format ip_catalog
