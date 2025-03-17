--------------------------------------------------------------------------------
-- KU Leuven - ESAT/COSIC - Emerging technologies, Systems & Security
--------------------------------------------------------------------------------
-- Module Name:     riscv_microcontroller - Behavioural
-- Project Name:    riscv_microcontroller
-- Description:     
--
-- Revision     Date       Author     Comments
-- v0.1         20241210   VlJo       Initial version
--
--------------------------------------------------------------------------------
library IEEE;
    use IEEE.STD_LOGIC_1164.ALL;
-- use IEEE.NUMERIC_STD.ALL;

library work;
    use work.PKG_hwswcd.ALL;

entity riscv_microcontroller is
    port(
        sys_clock : in STD_LOGIC;
        sys_reset : in STD_LOGIC;
        gpio_leds : out STD_LOGIC_VECTOR(3 downto 0)
    );
end entity riscv_microcontroller;

architecture Behavioural of riscv_microcontroller is

    component two_k_bram_dmem is
        port(
            clock : in STD_LOGIC;
            init_data_in : in STD_LOGIC_VECTOR(31 downto 0);
            init_write_enable : in STD_LOGIC;
            init_address : in STD_LOGIC_VECTOR(10 downto 0);
            data_in : in STD_LOGIC_VECTOR(31 downto 0);
            write_enable : in STD_LOGIC;
            address : in STD_LOGIC_VECTOR(10 downto 0);
            data_out : out STD_LOGIC_VECTOR(31 downto 0)
        );
    end component two_k_bram_dmem;

    component two_k_bram_imem is
        port(
            clock : in STD_LOGIC;
            init_data_in : in STD_LOGIC_VECTOR(31 downto 0);
            init_write_enable : in STD_LOGIC;
            init_address : in STD_LOGIC_VECTOR(10 downto 0);
            data_in : in STD_LOGIC_VECTOR(31 downto 0);
            write_enable : in STD_LOGIC;
            address : in STD_LOGIC_VECTOR(10 downto 0);
            data_out : out STD_LOGIC_VECTOR(31 downto 0)
        );
    end component two_k_bram_imem;
   
    component clock_and_reset_pynq is
        port(
            sysclock : IN STD_LOGIC;
            sysreset : IN STD_LOGIC;
            sreset : out STD_LOGIC;
            clock : out STD_LOGIC;
            heartbeat : out STD_LOGIC
        );
    end component clock_and_reset_pynq;

    component wrapped_timer is
        generic(
            G_WIDTH : natural := 8
        );
        port(
            clock : in STD_LOGIC;
            reset : in STD_LOGIC;
            iface_di : in STD_LOGIC_VECTOR(C_WIDTH-1 downto 0);
            iface_a : in STD_LOGIC_VECTOR(C_WIDTH-1 downto 0);
            iface_we : in STD_LOGIC;
            iface_do : out STD_LOGIC_VECTOR(C_WIDTH-1 downto 0)
        );
    end component wrapped_timer;

    -- (DE-)LOCALISING IN/OUTPUTS
    signal sys_clock_i : STD_LOGIC;
    signal sys_reset_i : STD_LOGIC;
    signal gpio_leds_o : STD_LOGIC_VECTOR(3 downto 0);

    -- dmem
    signal dmem_do : STD_LOGIC_VECTOR(31 downto 0);
    signal dmem_we : STD_LOGIC;
    signal dmem_a : STD_LOGIC_VECTOR(31 downto 0);
    signal dmem_di : STD_LOGIC_VECTOR(31 downto 0);
    
    --imem
    signal instruction : STD_LOGIC_VECTOR(31 downto 0);
    signal PC : STD_LOGIC_VECTOR(31 downto 0);

    -- CLOCK AND RESET
    signal clock : STD_LOGIC;
    signal reset : STD_LOGIC;

    signal ce, ce_d : STD_LOGIC;
    signal toid, toid_d : STD_LOGIC;

    signal leds : STD_LOGIC_VECTOR(6 downto 0);
    
    signal cycle_count : integer range 0 to 2 := 0;
    
    signal iface_di : STD_LOGIC_VECTOR(31 downto 0);
    signal iface_a : STD_LOGIC_VECTOR(31 downto 0);
    signal iface_we : STD_LOGIC;
    signal iface_do : STD_LOGIC_VECTOR(31 downto 0);
    
    signal riscv_d_in : STD_LOGIC_VECTOR(31 downto 0);
begin

    -------------------------------------------------------------------------------
    -- (DE-)LOCALISING IN/OUTPUTS
    -------------------------------------------------------------------------------
    sys_clock_i <= sys_clock;
    sys_reset_i <= sys_reset;
    gpio_leds <= gpio_leds_o;

    gpio_leds_o <= leds(3 downto 0);

    -------------------------------------------------------------------------------
    -- MICROPROCESSOR
    -------------------------------------------------------------------------------
    riscv_inst00: component riscv port map(
        clock => clock,
        reset => reset,
        ce => ce,
        dmem_do => riscv_d_in,
        dmem_we => dmem_we,
        dmem_a => dmem_a,
        dmem_di => dmem_di,
        instruction => instruction,
        PC => PC
    );

    PREG_CPU_CTRL: process(clock)
    begin
        if rising_edge(clock) then
            if reset = '1' then 
                ce <= '0';
                cycle_count <= 0;
            else
                if cycle_count = 2 then
                    ce <= '1';
                    cycle_count <= 0;
                else
                    ce <= '0';
                    cycle_count <= cycle_count + 1;
                end if;
            end if;
        end if;
    end process;


    -------------------------------------------------------------------------------
    -- MEMORIES
    -------------------------------------------------------------------------------
    two_k_bram_dmem_inst00: component two_k_bram_dmem port map(
        clock => clock,
        init_data_in => C_GND,
        init_write_enable => C_GND(0),
        init_address => C_GND(10 downto 0),
        data_in => dmem_di,
        write_enable => dmem_we,
        address => dmem_a(10 downto 0),
        data_out  => dmem_do
    );


    two_k_bram_imem_inst00: component two_k_bram_imem port map(
        clock => clock,
        init_data_in => C_GND,
        init_write_enable => C_GND(0),
        init_address => C_GND(10 downto 0),
        data_in => C_GND,
        write_enable => C_GND(0),
        address => PC(12 downto 2),
        data_out => instruction
    );

    -------------------------------------------------------------------------------
    -- PERIPHERALS
    -------------------------------------------------------------------------------
    PREG_LEDS: process(clock)
    begin
        if rising_edge(clock) then 
            if reset = '1' then 
                leds <= "0000000";
            else
                if dmem_we = '1' and dmem_a = x"80000000" then 
                    leds <= dmem_di(6 downto 0);
                end if;
            end if;
        end if;
    end process;
    
    PREG_TIMER: process(riscv_d_in, iface_do, dmem_do)
    begin
        if dmem_a(C_WIDTH-1 downto 12) = C_TIMER_BASE_ADDRESS_MASK then 
            riscv_d_in <= iface_do;
        else
            riscv_d_in <= dmem_do;
        end if;
    end process;
    
    


    -------------------------------------------------------------------------------
    -- CLOCK AND RESET
    -------------------------------------------------------------------------------
    clock_and_reset_pynq_inst00: component clock_and_reset_pynq port map(
        sysclock => sys_clock_i,
        sysreset => sys_reset_i,
        sreset => reset,
        clock => clock,
        heartbeat => open
    );
    
    
    -------------------------------------------------------------------------------
    -- TIMER WRAPPER
    -------------------------------------------------------------------------------
    wrapped_timer00: component wrapped_timer port map(
        clock => clock,
        reset => reset,
        iface_di => dmem_di,
        iface_a => dmem_a,
        iface_we => dmem_we,
        iface_do => iface_do
    );
    

end Behavioural;
