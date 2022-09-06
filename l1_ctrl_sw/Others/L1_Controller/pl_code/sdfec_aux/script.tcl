open_project sdfec_aux_prj
set_top sdfec_aux
add_files ./sdfec_aux.cpp
add_files ./sdfec_aux.h
open_solution "solution1"
set_part {xczu28dr-ffvg1517-2-e} -tool vivado
create_clock -period 10 -name default
#source "./project/solution1/directives.tcl"
#csim_design
csynth_design
#cosim_design
export_design -format ip_catalog
