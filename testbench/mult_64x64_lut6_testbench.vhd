--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   15:47:16 12/27/2015
-- Design Name:   
-- Module Name:   /home/vela/Desktop/Multiplier/test/ozantest/mult_64x64_lut6_testbench.vhd
-- Project Name:  ozantest
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: mult_64x64_lut6
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY mult_64x64_lut6_testbench IS
END mult_64x64_lut6_testbench;
 
ARCHITECTURE behavior OF mult_64x64_lut6_testbench IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT mult_64x64_lut6
    PORT(
         in0 : IN  std_logic_vector(63 downto 0);
         in1 : IN  std_logic_vector(63 downto 0);
         mult_out : OUT  std_logic_vector(128 downto 0)
        );
    END COMPONENT;
    

   --Inputs
   signal in0 : std_logic_vector(63 downto 0) := (others => '0');
   signal in1 : std_logic_vector(63 downto 0) := (others => '0');

 	--Outputs
   signal mult_out : std_logic_vector(128 downto 0);
   -- No clocks detected in port list. Replace <clock> below with 
   -- appropriate port name 
	signal clk : std_logic;
 
   constant clk_period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: mult_64x64_lut6 PORT MAP (
          in0 => in0,
          in1 => in1,
          mult_out => mult_out
        );

   -- Clock process definitions
   clk_process :process
   begin
		clk <= '0';
		wait for clk_period/2;
		clk <= '1';
		wait for clk_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin		
      -- hold reset state for 100 ns.
      wait for 100 ns;	

      wait for clk_period*10;
		
		in0 <= X"00000000FFFFFFFF";
		in1 <= X"00000000FFFFFFFF";
		
		wait for 10 ns;
		
		
		
		wait for 200 ns;
		
		in0 <= X"000000008E37D472";
		in1 <= X"00000000A8DE3B2C";
		
		wait for 200 ns;
		
      -- insert stimulus here 

      wait;
   end process;

END;
