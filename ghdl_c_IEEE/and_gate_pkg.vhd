-- and_gate_pkg.vhd
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

package and_gate_pkg is
    function to_char(sl : STD_LOGIC) return CHARACTER;
    function from_char(c : CHARACTER) return STD_LOGIC;
    function and_gate_c(a_in : CHARACTER; b_in : CHARACTER) return CHARACTER;
    attribute foreign of and_gate_c : function is "VHPIDIRECT and_gate_c";
end package;

package body and_gate_pkg is
    function to_char(sl : STD_LOGIC) return CHARACTER is
    begin
        case sl is
            when '0' => return '0';
            when '1' => return '1';
            when others => return 'X';
        end case;
    end function;

    function from_char(c : CHARACTER) return STD_LOGIC is
    begin
        case c is
            when '0' => return '0';
            when '1' => return '1';
            when others => return 'X';
        end case;
    end function;

    function and_gate_c(a_in : CHARACTER; b_in : CHARACTER) return CHARACTER is
    begin
        assert false report "and_gate_c body called" severity failure;
        return '0';
    end function;
end package body;

-- and_gate.vhd

