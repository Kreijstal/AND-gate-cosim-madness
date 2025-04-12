library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use std.textio.all;

entity and_gate_tb is
end entity and_gate_tb;

architecture behavior of and_gate_tb is

    -- Signals for testbench stimuli and checking
    signal tb_a   : std_logic := '0';
    signal tb_b   : std_logic := '0';
    signal tb_y   : std_logic;
    signal error_count : integer := 0;

    -- DPI-C Import Function Declarations
    -- These functions will be implemented in C++ (verilator_dpi_glue.cpp)
    -- and linked via the shared library.

    -- Initialize the Verilator simulation model
    procedure dpi_init C_IDENTIFIER => "dpi_init";

    -- Set inputs on the Verilator model
    procedure dpi_set_inputs (
        signal a_in : in std_logic;
        signal b_in : in std_logic
    ) C_IDENTIFIER => "dpi_set_inputs";

    -- Trigger one evaluation cycle of the Verilator model
    procedure dpi_eval C_IDENTIFIER => "dpi_eval";

    -- Get the output 'y' from the Verilator model
    function dpi_get_output return std_logic C_IDENTIFIER => "dpi_get_output";

    -- Finalize/cleanup the Verilator model
    procedure dpi_final C_IDENTIFIER => "dpi_final";


begin

    -- Testbench process
    stimulus_proc : process
        variable expected_y : std_logic;
        variable sim_time : time := 0 ns;
        variable l : line;
    begin
        -- Initialize the Verilator model via DPI
        write(output, string'("TB: Initializing Verilator model via DPI..."));
        writeline(output, l);
        dpi_init;
        wait for 1 ns; -- Allow time for initialization if needed

        -- Test case 1: a=0, b=0
        tb_a <= '0';
        tb_b <= '0';
        expected_y := '0';
        sim_time := sim_time + 10 ns;
        wait for 10 ns;
        dpi_set_inputs(tb_a, tb_b);
        dpi_eval;
        tb_y <= dpi_get_output;
        wait for 1 ns; -- Allow time for output propagation if needed
        write(output, string'("TB: Time=" & time'image(sim_time) & " a=" & std_logic'image(tb_a) & " b=" & std_logic'image(tb_b) & " expected=" & std_logic'image(expected_y) & " got=" & std_logic'image(tb_y)));
        if tb_y /= expected_y then
            write(output, string'(" <<< ERROR"));
            error_count := error_count + 1;
        end if;
        writeline(output, l);

        -- Test case 2: a=0, b=1
        tb_a <= '0';
        tb_b <= '1';
        expected_y := '0';
        sim_time := sim_time + 10 ns;
        wait for 10 ns;
        dpi_set_inputs(tb_a, tb_b);
        dpi_eval;
        tb_y <= dpi_get_output;
        wait for 1 ns;
        write(output, string'("TB: Time=" & time'image(sim_time) & " a=" & std_logic'image(tb_a) & " b=" & std_logic'image(tb_b) & " expected=" & std_logic'image(expected_y) & " got=" & std_logic'image(tb_y)));
        if tb_y /= expected_y then
            write(output, string'(" <<< ERROR"));
            error_count := error_count + 1;
        end if;
        writeline(output, l);

        -- Test case 3: a=1, b=0
        tb_a <= '1';
        tb_b <= '0';
        expected_y := '0';
        sim_time := sim_time + 10 ns;
        wait for 10 ns;
        dpi_set_inputs(tb_a, tb_b);
        dpi_eval;
        tb_y <= dpi_get_output;
        wait for 1 ns;
        write(output, string'("TB: Time=" & time'image(sim_time) & " a=" & std_logic'image(tb_a) & " b=" & std_logic'image(tb_b) & " expected=" & std_logic'image(expected_y) & " got=" & std_logic'image(tb_y)));
        if tb_y /= expected_y then
            write(output, string'(" <<< ERROR"));
            error_count := error_count + 1;
        end if;
        writeline(output, l);

        -- Test case 4: a=1, b=1
        tb_a <= '1';
        tb_b <= '1';
        expected_y := '1';
        sim_time := sim_time + 10 ns;
        wait for 10 ns;
        dpi_set_inputs(tb_a, tb_b);
        dpi_eval;
        tb_y <= dpi_get_output;
        wait for 1 ns;
        write(output, string'("TB: Time=" & time'image(sim_time) & " a=" & std_logic'image(tb_a) & " b=" & std_logic'image(tb_b) & " expected=" & std_logic'image(expected_y) & " got=" & std_logic'image(tb_y)));
        if tb_y /= expected_y then
            write(output, string'(" <<< ERROR"));
            error_count := error_count + 1;
        end if;
        writeline(output, l);

        -- Finalize simulation
        wait for 10 ns;
        write(output, string'("TB: Finalizing Verilator model via DPI..."));
        writeline(output, l);
        dpi_final;

        -- Report results
        if error_count = 0 then
            write(output, string'("TB: *** PASSED ***"));
        else
            write(output, string'("TB: *** FAILED *** (" & integer'image(error_count) & " errors)"));
        end if;
        writeline(output, l);

        -- End simulation
        std.env.finish;
        wait; -- Wait forever

    end process stimulus_proc;

end architecture behavior;