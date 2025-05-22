library ieee;
use ieee.std_logic_1164.all;

package and_gate_pkg is

    -- Component declaration for the Verilog module 'and_gate'
    -- This allows VHDL to instantiate it as if it were a VHDL entity.
    component and_gate is
        port (
            a : in std_logic;
            b : in std_logic;
            y : out std_logic
        );
    end component;

    -- Foreign subprogram declarations to link to C++ functions

    -- Procedure to initialize the Verilated model
    procedure and_gate_init;
    attribute foreign of and_gate_init : procedure is "VHPIDIRECT and_gate_init";

    -- Procedure to set input 'a'
    procedure set_a (val : in character);
    attribute foreign of set_a : procedure is "VHPIDIRECT set_a";

    -- Procedure to set input 'b'
    procedure set_b (val : in character);
    attribute foreign of set_b : procedure is "VHPIDIRECT set_b";

    -- Procedure to evaluate the Verilated model
    procedure and_gate_eval;
    attribute foreign of and_gate_eval : procedure is "VHPIDIRECT and_gate_eval";

    -- Function to get output 'y'
    function get_y return character;
    attribute foreign of get_y : function is "VHPIDIRECT get_y";

    -- Procedure to finalize the Verilated model
    procedure and_gate_finalize;
    attribute foreign of and_gate_finalize : procedure is "VHPIDIRECT and_gate_finalize";

end package and_gate_pkg;
