library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity and_gate_tb is
end and_gate_tb;

architecture Behavioral of and_gate_tb is
    signal a, b, y : STD_LOGIC;
    
    component and_gate is
        Port ( a : in  STD_LOGIC;
               b : in  STD_LOGIC;
               y : out STD_LOGIC);
    end component;

    procedure check_and_gate(
        a_in : in STD_LOGIC;
        b_in : in STD_LOGIC;
        expected_y : in STD_LOGIC
    ) is
    begin
        wait for 1 ns;
        assert (y = expected_y)
            report "ERROR: a=" & STD_LOGIC'image(a_in) & ", b=" & STD_LOGIC'image(b_in) &
                   ", y=" & STD_LOGIC'image(y) & " (expected " & STD_LOGIC'image(expected_y) & ")"
            severity failure;
        report "PASS: a=" & STD_LOGIC'image(a_in) & ", b=" & STD_LOGIC'image(b_in) &
               ", y=" & STD_LOGIC'image(y);
    end procedure;

begin
    uut: and_gate port map (
        a => a,
        b => b,
        y => y
    );

    stimulus: process
    begin
        a <= '0';
        b <= '0';
        
        wait for 100 ns;

        a <= '0'; b <= '0'; check_and_gate('0', '0', '0');
        a <= '0'; b <= '1'; check_and_gate('0', '1', '0');
        a <= '1'; b <= '0'; check_and_gate('1', '0', '0');
        a <= '1'; b <= '1'; check_and_gate('1', '1', '1');

        wait for 10 ns;
        
        report "All tests passed!";
        wait;
    end process;

end Behavioral;
