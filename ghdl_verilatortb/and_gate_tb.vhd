library ieee;
use ieee.std_logic_1164.all;
use std.env.all;

-- This will use a component from and_gate_pkg, to be created later
-- use WORK.and_gate_pkg.all;

entity and_gate_tb is
end entity and_gate_tb;

architecture rtl of and_gate_tb is

    -- Component declaration for the Verilog module
    component and_gate is
        port (
            a : in std_logic;
            b : in std_logic;
            y : out std_logic
        );
    end component and_gate;

    signal tb_a : std_logic := '0';
    signal tb_b : std_logic := '0';
    signal tb_y : std_logic;

begin

    -- Instantiate the and_gate component
    uut: entity work.and_gate(rtl) -- Assuming and_gate will be compiled into work library with rtl architecture
        port map (
            a => tb_a,
            b => tb_b,
            y => tb_y
        );

    -- Test process
    process
    begin
        -- Initialize inputs
        tb_a <= '0';
        tb_b <= '0';
        wait for 10 ns;
        assert tb_y = '0' report "Assertion failed for 00" severity error;

        -- Test case 01
        tb_a <= '0';
        tb_b <= '1';
        wait for 10 ns;
        assert tb_y = '0' report "Assertion failed for 01" severity error;

        -- Test case 10
        tb_a <= '1';
        tb_b <= '0';
        wait for 10 ns;
        assert tb_y = '0' report "Assertion failed for 10" severity error;

        -- Test case 11
        tb_a <= '1';
        tb_b <= '1';
        wait for 10 ns;
        assert tb_y = '1' report "Assertion failed for 11" severity error;

        report "Testbench finished successfully.";
        std.env.finish; -- or std.env.stop(0);
    end process;

end architecture rtl;
