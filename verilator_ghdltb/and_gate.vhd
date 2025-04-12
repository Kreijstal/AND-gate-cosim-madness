-- and_gate.vhd
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.and_gate_pkg.all;

entity and_gate is
    Port ( a : in  STD_LOGIC;
           b : in  STD_LOGIC;
           y : out STD_LOGIC);
end and_gate;

architecture Behavioral of and_gate is
begin
    process(a, b)
    variable result : CHARACTER;
    begin
        result := and_gate_verilated(to_char(a), to_char(b));
        y <= from_char(result);
    end process;
end Behavioral;
