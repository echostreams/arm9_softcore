library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;

library std;
  use std.textio.all;

entity tb is
  generic (
    ROM_FILE : string := "-" -- BOOT ROM IMAGE FILE
  );
end entity tb;

architecture RTL of tb is

  function to_hexstr(slv : std_logic_vector) return string is
    variable l : line;
  begin
    hwrite(l, slv);
    return l.all;
  end function to_hexstr;

  procedure print(str : in string) is
    variable l : line;
  begin
    write(l, str);
    writeline(OUTPUT, l);
  end procedure print;

  component arm9_compatiable_code is
    port (
      clk         : in    std_logic;
      cpu_en      : in    std_logic;
      cpu_restart : in    std_logic;
      fiq         : in    std_logic;
      irq         : in    std_logic;
      ram_abort   : in    std_logic;
      ram_rdata   : in    std_logic_vector(31 downto 0);
      rom_abort   : in    std_logic;
      rom_data    : in    std_logic_vector(31 downto 0);
      rst         : in    std_logic;

      ram_addr    : out   std_logic_vector(31 downto 0);
      ram_cen     : out   std_logic;
      ram_flag    : out   std_logic_vector(3 downto 0);
      ram_wdata   : out   std_logic_vector(31 downto 0);
      ram_wen     : out   std_logic;
      rom_addr    : out   std_logic_vector(31 downto 0);
      rom_en      : out   std_logic
   );
  end component;

  --parameter BINFILE = "./DHRY/Obj/DHRY.bin";
  --parameter BINFILE = "./DHRY.bin";
  --parameter BINFILE = "./hello/hello";
  --parameter BINFILE = "./dhry/dhry";
  --parameter BINFILE = "./testcode/MiniDemo2148.bin";
  --parameter BINFILE = "./lpc2104/hello.bin";

  constant clk_period : time := 10 ns; -- 100 MHz

  constant ram_depth : natural := 4096;   -- 16 kB ON-CHIP STATIC RAM (LPC2104: 0x4000_0000)
  constant ram_width : natural := 32;
  constant rom_depth : natural := 131072; -- 128k kB ON-CHIP FLASH MEMORY (0x0000_0000)
  constant rom_width : natural := 8;

  type rom_type is array(0 to rom_depth - 1) of std_logic_vector(rom_width - 1 downto 0);
  type ram_type is array(0 to ram_depth - 1) of std_logic_vector(ram_width - 1 downto 0);

  signal clk : std_logic;
  signal rst : std_logic;

  -- 128 kB ROM
  signal rom : rom_type;

  signal rom_en    : std_logic;
  signal rom_addr  : std_logic_vector(31 downto 0);
  signal rom_data  : std_logic_vector(31 downto 0) := x"00000000";
  signal ram_cen   : std_logic;
  signal ram_wen   : std_logic;
  signal ram_flag  : std_logic_vector(3 downto 0);
  signal ram_addr  : std_logic_vector(31 downto 0);
  signal ram_wdata : std_logic_vector(31 downto 0);

  -- 16 kB RAM
  signal ram : ram_type;

  signal ram_rdata : std_logic_vector(31 downto 0);

  signal irq : std_logic;

  signal timer_cnt : integer := 0;

  signal stop_condition : std_logic := '0';

begin

  read_bf: process is
    type char_file_t is file of character;
    file char_file : char_file_t;
    variable char_v : character;
    subtype byte_t is natural range 0 to 255;
    variable byte_v : byte_t;
    variable byte_index : integer;
    -- variable vec_index : integer;
  begin
    --file_open(char_file, "./DHRY-keil/Obj/DHRY.bin");
    --file_open(char_file, "./dhry/dhry");
    file_open(char_file, ROM_FILE);
    byte_index := 0;
    while not endfile(char_file) loop
      read(char_file, char_v);
      byte_v := character'pos(char_v);
      -- vec_index := byte_index mod 4;
      -- ram(byte_index / 4)( (vec_index + 1) * 8 - 1 downto (vec_index + 1) * 8 - 8) <= std_logic_vector(to_unsigned(byte_v, 8));
      rom(byte_index) <= std_logic_vector(to_unsigned(byte_v, 8));
      -- report "Char: " & " #" & integer'image(byte_v) & " @" & integer'image(byte_index);
      byte_index := byte_index + 1;
    end loop;
    file_close(char_file);
    wait;
  end process;

  clk_gen : process is
  begin
    clk <= '0';
    wait for clk_period/2;
    clk <= '1';
    wait for clk_period/2;
  end process;

  stop_proc : process is
  begin
    wait until stop_condition;
    wait for (10 * clk_period);
    report "STOP SIMULATION";
    std.env.finish;
    wait;
  end process;

  rst <= '1', '0' after (10 * clk_period);

  rom_read : process (clk) is
  begin
    if (rising_edge(clk)) then
      if (rom_en = '1' and rom_addr <= x"1fffc") then
        rom_data <= rom(to_integer(unsigned(rom_addr))+3) &
                    rom(to_integer(unsigned(rom_addr))+2) &
                    rom(to_integer(unsigned(rom_addr))+1) &
                    rom(to_integer(unsigned(rom_addr)));
      else
	      if (rom_addr > x"1fffc") then
          report "ROM ADDR: " & to_hexstr(rom_addr);
          stop_condition <= '1';
        end if;
      end if;
    end if;
  end process;

  --processing_3 : process
  --begin
  --  for i in 0 to 4096 - 1 loop
  --    ram(i) <= X"00000000";
  --  end loop;
  --end process;

  ram_read : process (clk) is
  begin
    if (rising_edge(clk)) then
      if (ram_cen and not ram_wen) then
        if (ram_addr = X"e0000000") then
          ram_rdata <= 32X"0";
        elsif (ram_addr(31 downto 28) = X"0") then
          ram_rdata <= rom(to_integer(unsigned(ram_addr)) + 3) &
            rom(to_integer(unsigned(ram_addr)) + 2) &
            rom(to_integer(unsigned(ram_addr)) + 1) &
            rom(to_integer(unsigned(ram_addr)));
        elsif (ram_addr(31 downto 28) = X"4") then
          ram_rdata <= ram(to_integer(unsigned(ram_addr(27 downto 2))));
        else
          null;
        end if;
      else
        null;
      end if;
    end if;
  end process;

  ram_write : process (clk) is
  begin
    if (rising_edge(clk)) then
      if (ram_cen = '1' and ram_wen = '1' and (ram_addr(31 downto 28) = X"4")) then
        if (ram_flag(3)) then
          ram(to_integer(unsigned(ram_addr(27 downto 2))))(31 downto 24) <= ram_wdata(31 downto 24);
        else
          ram(to_integer(unsigned(ram_addr(27 downto 2))))(31 downto 24) <= ram(to_integer(unsigned(ram_addr(27 downto 2))))(31 downto 24);
        end if;
        if (ram_flag(2)) then
          ram(to_integer(unsigned(ram_addr(27 downto 2))))(23 downto 16) <= ram_wdata(23 downto 16);
        else
          ram(to_integer(unsigned(ram_addr(27 downto 2))))(23 downto 16) <= ram(to_integer(unsigned(ram_addr(27 downto 2))))(23 downto 16);
        end if;
        if (ram_flag(1)) then
          ram(to_integer(unsigned(ram_addr(27 downto 2))))(15 downto 8) <= ram_wdata(15 downto 8);
        else
          ram(to_integer(unsigned(ram_addr(27 downto 2))))(15 downto 8) <= ram(to_integer(unsigned(ram_addr(27 downto 2))))(15 downto 8);
        end if;
        if (ram_flag(0)) then
          ram(to_integer(unsigned(ram_addr(27 downto 2))))(7 downto 0) <= ram_wdata(7 downto 0);
        else
          ram(to_integer(unsigned(ram_addr(27 downto 2))))(7 downto 0) <= ram(to_integer(unsigned(ram_addr(27 downto 2))))(7 downto 0);
        end if;

      else
        -- $display("write: %x: %x", ram_addr[27:2], {
        --   (ram_flag[3] ? ram_wdata[31:24]:ram[ram_addr[27:2]][31:24]),
        --   (ram_flag[2] ? ram_wdata[23:16]:ram[ram_addr[27:2]][23:16]),
        --   (ram_flag[1] ? ram_wdata[15:8]:ram[ram_addr[27:2]][15:8]),
        --   (ram_flag[0] ? ram_wdata[7:0]:ram[ram_addr[27:2]][7:0])
        -- });

        null;
      end if;
    end if;
  end process;

  output_proc : process (clk) is

    --variable L : LINE;

  begin
    if (rising_edge(clk)) then
      if (ram_cen = '1' and ram_wen = '1' and (ram_addr = X"e0000004")) then
        --(null)("%s", ram_wdata(7 downto 0));
        --report to_string(character'val(to_integer(unsigned(ram_wdata(7 downto 0)))));
        --write(L, character'val(to_integer(unsigned(ram_wdata(7 downto 0)))));
        --writeline(OUTPUT, L);
        --write(OUTPUT, to_string(character'val(to_integer(unsigned(ram_wdata(7 downto 0))))));
        --print(to_string(character'val(to_integer(unsigned(ram_wdata(7 downto 0))))));
        write(OUTPUT, (1 => character'val(to_integer(unsigned(ram_wdata(7 downto 0))))));
      else

        null;
      end if;
    end if;
  end process;

  timer_proc : process (clk) is
  begin
    if (rising_edge(clk)) then
      if (timer_cnt = 9999) then
        timer_cnt <= 0;
      else
        timer_cnt <= timer_cnt + 1;
      end if;
    end if;
  end process;

  irq <= '1' when (timer_cnt = 9999) else
         '0';

  u_arm9 : component arm9_compatiable_code
    port map (
      clk         => clk,
      cpu_en      => '1',
      cpu_restart => '0',
      fiq         => '0',
      irq         => irq,
      ram_abort   => '0',
      ram_rdata   => ram_rdata,
      rom_abort   => '0',
      rom_data    => rom_data,
      rst         => rst,

      ram_addr  => ram_addr,
      ram_cen   => ram_cen,
      ram_flag  => ram_flag,
      ram_wdata => ram_wdata,
      ram_wen   => ram_wen,
      rom_addr  => rom_addr,
      rom_en    => rom_en
    );

end architecture RTL;
