library ieee;
use ieee.std_logic_1164.all;
use std.env.all;
use WORK.and_gate_pkg.all;

entity and_gate_tb is
end entity and_gate_tb;

architecture rtl of and_gate_tb is

    -- No component declaration or signals needed here for VHPIDIRECT

begin

    -- Test process
    process
        variable y_char : character;
    begin
        -- Initialize the Verilated model
        and_gate_init;

        -- Test case 00
        set_a('0');
        set_b('0');
        and_gate_eval;
        y_char := get_y;
        wait for 10 ns;
        assert y_char = '0' report "Assertion failed for 00" severity error;

        -- Test case 01
        set_a('0');
        set_b('1');
        and_gate_eval;
        y_char := get_y;
        wait for 10 ns;
        assert y_char = '0' report "Assertion failed for 01" severity error;

        -- Test case 10
        set_a('1');
        set_b('0');
        and_gate_eval;
        y_char := get_y;
        wait for 10 ns;
        assert y_char = '0' report "Assertion failed for 10" severity error;

        -- Test case 11
        set_a('1');
        set_b('1');
        and_gate_eval;
        y_char := get_y;
        wait for 10 ns;
        assert y_char = '1' report "Assertion failed for 11" severity error;

        report "Testbench finished successfully.";
        -- Finalize the Verilated model
        and_gate_finalize;
        std.env.finish; -- or std.env.stop(0);
    end process;

end architecture rtl;
