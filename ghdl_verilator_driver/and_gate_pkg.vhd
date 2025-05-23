-- ghdl_verilator_driver/and_gate_pkg.vhd
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

package and_gate_pkg is
    -- Helper function to convert STD_LOGIC to CHARACTER for C interface
    function to_char(sl : STD_LOGIC) return CHARACTER;
    -- Helper function to convert CHARACTER to STD_LOGIC from C interface
    function from_char(c : CHARACTER) return STD_LOGIC;
    -- Declaration of the C function to be imported
    function and_gate_c(a_in : CHARACTER; b_in : CHARACTER) return CHARACTER;
    -- Link the VHDL function to the C function implementation
    attribute foreign of and_gate_c : function is "VHPIDIRECT and_gate_c";
end package and_gate_pkg;

package body and_gate_pkg is
    function to_char(sl : STD_LOGIC) return CHARACTER is
    begin
        case sl is
            when '0' => return '0';
            when '1' => return '1';
            when others => return 'X'; -- Handle undefined states
        end case;
    end function;

    function from_char(c : CHARACTER) return STD_LOGIC is
    begin
        case c is
            when '0' => return '0';
            when '1' => return '1';
            when others => return 'X'; -- Handle undefined states
        end case;
    end function;

    -- This is a stub body. The actual implementation is in C.
    function and_gate_c(a_in : CHARACTER; b_in : CHARACTER) return CHARACTER is
    begin
        -- This body should not be synthesized or executed by GHDL directly
        assert false report "VHDL stub for and_gate_c called. This should be implemented in C." severity failure;
        return 'X'; 
    end function;
end package body and_gate_pkg;
