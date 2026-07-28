create_clock -period 20.0 [get_ports CLOCK0_50]

create_generated_clock -name {clk} -source [get_pins {pll_module|iopll_0|tennm_ph2_iopll|ref_clk0}] -divide_by 1 -multiply_by 2 [get_pins {pll_module|iopll_0|tennm_ph2_iopll|out_clk[0]}]

derive_clock_uncertainty

set_false_path -from [get_ports button*]
set_false_path -from [get_ports switch*]
set_false_path -to [get_ports led*]

set_output_delay -clock [get_clocks {clk}] -max 0 -source_latency_include [get_ports led*]
set_output_delay -clock [get_clocks {clk}] -min 0 -source_latency_include [get_ports led*]