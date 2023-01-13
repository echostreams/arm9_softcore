-- ARM7: Von-Neumann architecture (single bus for data and instructions)
--       3-stage pipeline (fetch/decode/execute)
-- ARM9: Harvard architecture (separate bus for data and instructions)
--       5-stage pipeline (fetch/decode/execute/memory/write)

library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;
  use ieee.numeric_std_unsigned.all;

entity arm9_compatiable_code is
  port (
    clk : in std_logic;
    rst : in std_logic;

    cpu_en      : in std_logic;
    cpu_restart : in std_logic;

    fiq : in std_logic;
    irq : in std_logic;

    ram_abort : in std_logic;
    ram_cen   : out std_logic;
    ram_flag  : out std_logic_vector(3 downto 0);
    ram_addr  : out std_logic_vector(31 downto 0);
    ram_rdata : in std_logic_vector(31 downto 0);    
    ram_wdata : out std_logic_vector(31 downto 0);
    ram_wen   : out std_logic;

    rom_abort : in std_logic;
    rom_data  : in std_logic_vector(31 downto 0);
    rom_addr  : out std_logic_vector(31 downto 0);
    rom_en    : out std_logic
  );
end arm9_compatiable_code;

architecture RTL of arm9_compatiable_code is
  --*****************************************************/
  --register definition area
  --*****************************************************/
  signal add_b : std_logic_vector(31 downto 0);
  signal add_c : std_logic;
  signal all_code : std_logic;
  signal cha_num : std_logic_vector(3 downto 0);
  signal cha_vld : std_logic;
  signal cmd : std_logic_vector(31 downto 0);
  signal cmd_addr : std_logic_vector(31 downto 0);
  signal cmd_flag : std_logic;
  signal code_abort : std_logic;
  signal code_flag : std_logic;
  signal code_rm : std_logic_vector(31 downto 0);
  signal code_rma : std_logic_vector(31 downto 0);
  signal code_rot_num : std_logic_vector(4 downto 0);
  signal code_rs : std_logic_vector(31 downto 0);
  signal code_rs_flag : std_logic_vector(2 downto 0);
  signal code_rsa : std_logic_vector(31 downto 0);
  signal code_und : std_logic;
  signal cond_satisfy : std_logic;
  signal cpsr_c : std_logic;
  signal cpsr_f : std_logic;
  signal cpsr_i : std_logic;
  signal cpsr_m : std_logic_vector(4 downto 0);
  signal cpsr_n : std_logic;
  signal cpsr_v : std_logic;
  signal cpsr_z : std_logic;
  signal dp_ans : std_logic_vector(31 downto 0);
  signal fiq_flag : std_logic;
  signal go_data : std_logic_vector(31 downto 0);
  signal go_fmt : std_logic_vector(5 downto 0);
  signal go_num : std_logic_vector(3 downto 0);
  signal go_vld : std_logic;
  signal hold_en_dly : std_logic;
  signal irq_flag : std_logic;
  signal ldm_change : std_logic;
  signal ldm_num : std_logic_vector(3 downto 0);
  signal ldm_sel : std_logic_vector(3 downto 0);
  signal ldm_usr : std_logic;
  signal ldm_vld : std_logic;
  signal mult_z : std_logic;
  signal multl_extra_num : std_logic;
  signal r0 : std_logic_vector(31 downto 0);
  signal r1 : std_logic_vector(31 downto 0);
  signal r2 : std_logic_vector(31 downto 0);
  signal r3 : std_logic_vector(31 downto 0);
  signal r4 : std_logic_vector(31 downto 0);
  signal r5 : std_logic_vector(31 downto 0);
  signal r6 : std_logic_vector(31 downto 0);
  signal r7 : std_logic_vector(31 downto 0);
  signal r8_fiq : std_logic_vector(31 downto 0);
  signal r8_usr : std_logic_vector(31 downto 0);
  signal r9_fiq : std_logic_vector(31 downto 0);
  signal r9_usr : std_logic_vector(31 downto 0);
  signal ra_fiq : std_logic_vector(31 downto 0);
  signal ra_usr : std_logic_vector(31 downto 0);
  -- signal ram_flag : std_logic_vector(3 downto 0);
  -- signal ram_wdata : std_logic_vector(31 downto 0);
  signal rb_fiq : std_logic_vector(31 downto 0);
  signal rb_usr : std_logic_vector(31 downto 0);
  signal rc_fiq : std_logic_vector(31 downto 0);
  signal rc_usr : std_logic_vector(31 downto 0);
  signal rd : std_logic_vector(31 downto 0);
  signal rd_abt : std_logic_vector(31 downto 0);
  signal rd_fiq : std_logic_vector(31 downto 0);
  signal rd_irq : std_logic_vector(31 downto 0);
  signal rd_svc : std_logic_vector(31 downto 0);
  signal rd_und : std_logic_vector(31 downto 0);
  signal rd_usr : std_logic_vector(31 downto 0);
  signal re : std_logic_vector(31 downto 0);
  signal re_abt : std_logic_vector(31 downto 0);
  signal re_fiq : std_logic_vector(31 downto 0);
  signal re_irq : std_logic_vector(31 downto 0);
  signal re_svc : std_logic_vector(31 downto 0);
  signal re_und : std_logic_vector(31 downto 0);
  signal re_usr : std_logic_vector(31 downto 0);
  signal reg_ans : std_logic_vector(63 downto 0);
  signal rf : std_logic_vector(31 downto 0);
  signal rm_msb : std_logic;
  signal rn : std_logic_vector(31 downto 0);
  signal rn_register : std_logic_vector(31 downto 0);
  signal rna : std_logic_vector(31 downto 0);
  signal rnb : std_logic_vector(31 downto 0);
  signal rs_msb : std_logic;
  signal sec_operand : std_logic_vector(31 downto 0);
  signal spsr : std_logic_vector(10 downto 0);
  signal spsr_abt : std_logic_vector(10 downto 0);
  signal spsr_fiq : std_logic_vector(10 downto 0);
  signal spsr_irq : std_logic_vector(10 downto 0);
  signal spsr_svc : std_logic_vector(10 downto 0);
  signal spsr_und : std_logic_vector(10 downto 0);
  signal sum_m : std_logic_vector(4 downto 0);
  signal to_data : std_logic_vector(31 downto 0);
  signal to_num : std_logic_vector(3 downto 0);


  --*****************************************************/


  --*****************************************************/
  --wire definition area
  --*****************************************************/
  signal add_a : std_logic_vector(31 downto 0);
  signal and_ans : std_logic_vector(31 downto 0);
  signal bic_ans : std_logic_vector(31 downto 0);
  signal bit_cy : std_logic;
  signal bit_ov : std_logic;
  signal cha_rf_vld : std_logic;
  signal cmd_is_b : std_logic;
  signal cmd_is_bx : std_logic;
  signal cmd_is_dp0 : std_logic;
  signal cmd_is_dp1 : std_logic;
  signal cmd_is_dp2 : std_logic;
  signal cmd_is_ldm : std_logic;
  signal cmd_is_ldr0 : std_logic;
  signal cmd_is_ldr1 : std_logic;
  signal cmd_is_ldrh0 : std_logic;
  signal cmd_is_ldrh1 : std_logic;
  signal cmd_is_ldrsb0 : std_logic;
  signal cmd_is_ldrsb1 : std_logic;
  signal cmd_is_ldrsh0 : std_logic;
  signal cmd_is_ldrsh1 : std_logic;
  signal cmd_is_mrs : std_logic;
  signal cmd_is_msr0 : std_logic;
  signal cmd_is_msr1 : std_logic;
  signal cmd_is_mult : std_logic;
  signal cmd_is_multl : std_logic;
  signal cmd_is_multlx : std_logic;
  signal cmd_is_swi : std_logic;
  signal cmd_is_swp : std_logic;
  signal cmd_is_swpx : std_logic;
  signal cmd_ok : std_logic;
  signal cmd_sum_m : std_logic_vector(4 downto 0);
  signal code : std_logic_vector(31 downto 0);
  signal code_is_b : std_logic;
  signal code_is_bx : std_logic;
  signal code_is_dp0 : std_logic;
  signal code_is_dp1 : std_logic;
  signal code_is_dp2 : std_logic;
  signal code_is_ldm : std_logic;
  signal code_is_ldr0 : std_logic;
  signal code_is_ldr1 : std_logic;
  signal code_is_ldrh0 : std_logic;
  signal code_is_ldrh1 : std_logic;
  signal code_is_ldrsb0 : std_logic;
  signal code_is_ldrsb1 : std_logic;
  signal code_is_ldrsh0 : std_logic;
  signal code_is_ldrsh1 : std_logic;
  signal code_is_mrs : std_logic;
  signal code_is_msr0 : std_logic;
  signal code_is_msr1 : std_logic;
  signal code_is_mult : std_logic;
  signal code_is_multl : std_logic;
  signal code_is_swi : std_logic;
  signal code_is_swp : std_logic;
  signal code_rm_num : std_logic_vector(3 downto 0);
  signal code_rm_vld : std_logic;
  signal code_rn_num : std_logic_vector(3 downto 0);
  signal code_rn_vld : std_logic;
  signal code_rnhi_num : std_logic_vector(3 downto 0);
  signal code_rnhi_vld : std_logic;
  signal code_rs_num : std_logic_vector(3 downto 0);
  signal code_rs_vld : std_logic;
  signal code_sum_m : std_logic_vector(4 downto 0);
  signal cpsr : std_logic_vector(10 downto 0);
  signal eor_ans : std_logic_vector(31 downto 0);
  signal fiq_en : std_logic;
  signal go_rf_vld : std_logic;
  signal high_bit : std_logic;
  signal high_middle : std_logic_vector(1 downto 0);
  signal hold_en : std_logic;
  signal int_all : std_logic;
  signal irq_en : std_logic;
  signal ldm_data : std_logic_vector(31 downto 0);
  signal ldm_rf_vld : std_logic;
  signal mult_ans : std_logic_vector(63 downto 0);
  signal or_ans : std_logic_vector(31 downto 0);
  signal r8 : std_logic_vector(31 downto 0);
  signal r9 : std_logic_vector(31 downto 0);
  signal ra : std_logic_vector(31 downto 0);
  -- signal ram_addr : std_logic_vector(31 downto 0);
  -- signal ram_cen : std_logic;
  -- signal ram_wen : std_logic;
  signal rb : std_logic_vector(31 downto 0);
  signal rc : std_logic_vector(31 downto 0);
  signal rf_b : std_logic_vector(31 downto 0);
  -- signal rom_addr : std_logic_vector(31 downto 0);
  -- signal rom_en : std_logic;
  signal sum_middle : std_logic_vector(31 downto 0);
  signal sum_rn_rm : std_logic_vector(31 downto 0);
  signal to_rf_vld : std_logic;
  signal to_vld : std_logic;
  signal wait_en : std_logic;
  
  function get_spsr(
    cmd_in  : in std_logic_vector(31 downto 0);    
    operand : in std_logic_vector(31 downto 0);
    spsr_in : in std_logic_vector(10 downto 0)
    ) return std_logic_vector is
    variable nzcv : std_logic_vector(3 downto 0);
    variable iftm : std_logic_vector(6 downto 0);
    variable spsr_out : std_logic_vector(10 downto 0);
  begin
    nzcv := operand(31 downto 28) when cmd_in(19) else
            spsr_in(10 downto 7);
    iftm := operand(7 downto 6) & operand(4 downto 0) when cmd_in(16) else 
            spsr_in(6 downto 0);
    spsr_out := nzcv & iftm;  
    return spsr_out;
  end;

  -- bits sum
  -- Loop-based
  function hw_loop(v: std_logic_vector) return natural is
    variable h: natural;
  begin
    h := 0;    
    for i in v'range loop
      if v(i) = '1' then
        h := h + 1;
      end if;
    end loop;
    return h;
  end function hw_loop;

  -- Log-tree-based, using a recursive function:
  function hw_tree(v: std_logic_vector) return natural is
    constant size: natural := v'length;
    constant vv: std_logic_vector(size - 1 downto 0) := v;
    variable h: natural;
  begin
    -- report ("hw_tree length: " & integer'image(size));
    -- report ("hw_tree vector: " & to_string(vv));
    h := 0;    
    if size = 1 and vv(0) = '1' then
      h := 1;
    elsif size > 1 then
      h := hw_tree(vv(size - 1 downto size / 2)) + hw_tree(vv(size / 2 - 1 downto 0));
    end if;
    -- report ("hw_tree return: " & integer'image(h));
    return h;
  end function hw_tree;

  function to_std_logic(i: boolean) return std_logic is
  begin
    if i then
      return '1';
    else
      return '0';
    end if;
  end function;

  function concatenate(
    constant num : in natural;
    constant sl  : in std_logic
  ) return std_logic_vector is
    variable v   : std_logic_vector(num - 1 downto 0);
  begin
    for i in v'range loop
      v(i) := sl;
    end loop;
    return v;
  end function;

begin

  --*****************************************************/
  --wire statement area
  --*****************************************************/
  add_a <= rn;

  and_ans <= rnb and sec_operand;

  bic_ans <= rnb and not sec_operand;

  bit_cy <= high_middle(1);

  bit_ov <= high_middle(1) xor sum_middle(31);

  cha_rf_vld <= '1' when cha_vld = '1' and cha_num = X"f" else
                '0';

  cmd_is_b <= '1' when cmd(27 downto 25) = "101" else
              '0';

  cmd_is_bx <= '1' when ((cmd(27 downto 23) & cmd(20) & cmd(7) & cmd(4)) = "00010001") else '0';

  cmd_is_dp0 <= '1' when ((cmd(27 downto 25) = "000") and (cmd(4) = '0') and ((cmd(24 downto 23) /= "10") or cmd(20) = '1')) else '0';

  cmd_is_dp1 <= '1' when (cmd(27 downto 25) = "000") and cmd(7) = '0' and cmd(4) = '1' and ((cmd(24 downto 23) /= "10") or cmd(20) = '1') else '0';

  cmd_is_dp2 <= '1' when (cmd(27 downto 25) = "001") and ((cmd(24 downto 23) /= "10") or (cmd(20) = '1')) else '0';

  cmd_is_ldm <= '1' when (cmd(27 downto 25) = "100") else '0';

  cmd_is_ldr0 <= '1' when (cmd(27 downto 25) = "010") else '0';

  cmd_is_ldr1 <= '1' when (cmd(27 downto 25) = "011") else '0';

  cmd_is_ldrh0 <= '1' when (cmd(27 downto 25) = "000") and (cmd(7 downto 4) = "1011") and (cmd(22) = '0') else '0';

  cmd_is_ldrh1 <= '1' when (cmd(27 downto 25) = "000") and (cmd(7 downto 4) = "1011") and (cmd(22) = '1') else '0';

  cmd_is_ldrsb0 <= '1' when (cmd(27 downto 25) = "000") and (cmd(7 downto 4) = "1101") and (cmd(22) = '0') else '0';

  cmd_is_ldrsb1 <= '1' when (cmd(27 downto 25) = "000") and (cmd(7 downto 4) = "1101") and (cmd(22) = '1') else '0';

  cmd_is_ldrsh0 <= '1' when (cmd(27 downto 25) = "000") and (cmd(7 downto 4) = "1111") and (cmd(22) = '0') else '0';

  cmd_is_ldrsh1 <= '1' when (cmd(27 downto 25) = "000") and (cmd(7 downto 4) = "1111") and (cmd(22) = '1') else '0';

  cmd_is_mrs <= '1' when ((cmd(27 downto 23) & cmd(21 downto 20) & cmd(7) & cmd(4)) = "000100000") else '0';

  cmd_is_msr0 <= '1' when ((cmd(27 downto 23) & cmd(21 downto 20) & cmd(7) & cmd(4)) = "000101000") else '0';

  cmd_is_msr1 <= '1' when (cmd(27 downto 25) = "001") and (cmd(24 downto 23) = "10") and (cmd(20) = '0') else '0';

  cmd_is_mult <= '1' when (cmd(27 downto 25) = "000") and (cmd(7 downto 4) = "1001") and (cmd(24 downto 23) = "00") else '0';

  cmd_is_multl <= '1' when (cmd(27 downto 25) = "000") and (cmd(7 downto 4) = "1001") and (cmd(24 downto 23) = "01") else '0';

  cmd_is_multlx <= '1' when (cmd(27 downto 24) = "1100") else '0';

  cmd_is_swi <= '1' when (cmd(27 downto 25) = "111") else '0';

  cmd_is_swp <= '1' when (cmd(27 downto 25) = "000") and (cmd(7 downto 4) = "1001") and (cmd(24 downto 23) = "10") else '0';

  cmd_is_swpx <= '1' when (cmd(27 downto 24) = "1101") else '0';

  cmd_ok <= not int_all and cmd_flag and cond_satisfy;

  -- cmd_sum_m <= (cmd(0)+cmd(1)+cmd(2)+cmd(3)+cmd(4)+cmd(5)+cmd(6)+cmd(7)+cmd(8)+cmd(9)+cmd(10)+cmd(11)+cmd(12)+cmd(13)+cmd(14)+cmd(15));
  cmd_sum_m <= std_logic_vector(to_unsigned(hw_tree(cmd(15 downto 0)), cmd_sum_m'length));

  code <= rom_data;

  code_is_b <= '1' when (code(27 downto 25) = "101") else '0';

  code_is_bx <= '1' when ((code(27 downto 23) & code(20) & code(7) & code(4)) = "00010001") else '0';

  code_is_dp0 <= '1' when (code(27 downto 25) = "000") and code(4) = '0' and ((code(24 downto 23) /= "10") or code(20) = '1') else '0';

  code_is_dp1 <= '1' when (code(27 downto 25) = "000") and code(7) = '0' and code(4) = '1' and ((code(24 downto 23) /= "10") or code(20) = '1') else '0';

  code_is_dp2 <= '1' when (code(27 downto 25) = "001") and ((code(24 downto 23) /= "10") or code(20) = '1') else '0';

  code_is_ldm <= '1' when (code(27 downto 25) = "100") else '0';

  code_is_ldr0 <= '1' when (code(27 downto 25) = "010") else '0';

  code_is_ldr1 <= '1' when (code(27 downto 25) = "011") else '0';

  code_is_ldrh0 <= '1' when (code(27 downto 25) = "000") and (code(7 downto 4) = "1011") and code(22) = '0' else '0';

  code_is_ldrh1 <= '1' when (code(27 downto 25) = "000") and (code(7 downto 4) = "1011") and code(22) = '1' else '0';

  code_is_ldrsb0 <= '1' when (code(27 downto 25) = "000") and (code(7 downto 4) = "1101") and code(22) = '0' else '0';

  code_is_ldrsb1 <= '1' when (code(27 downto 25) = "000") and (code(7 downto 4) = "1101") and code(22) = '1' else '0';

  code_is_ldrsh0 <= '1' when (code(27 downto 25) = "000") and (code(7 downto 4) = "1111") and code(22) = '0' else '0';

  code_is_ldrsh1 <= '1' when (code(27 downto 25) = "000") and (code(7 downto 4) = "1111") and code(22) = '1' else '0';

  code_is_mrs <= '1' when ((code(27 downto 23) & code(21 downto 20) & code(7) & code(4)) = "000100000") else '0';

  code_is_msr0 <= '1' when ((code(27 downto 23) & code(21 downto 20) & code(7) & code(4)) = "000101000") else '0';

  code_is_msr1 <= '1' when (code(27 downto 25) = "001") and (code(24 downto 23) = "10") and code(20) = '0' else '0';

  code_is_mult <= '1' when (code(27 downto 25) = "000") and (code(7 downto 4) = "1001") and (code(24 downto 23) = "00") else '0';

  code_is_multl <= '1' when (code(27 downto 25) = "000") and (code(7 downto 4) = "1001") and (code(24 downto 23) = "01") else '0';

  code_is_swi <= '1' when (code(27 downto 25) = "111") else '0';

  code_is_swp <= '1' when (code(27 downto 25) = "000") and (code(7 downto 4) = "1001") and (code(24 downto 23) = "10") else '0';

  code_rm_num <= code(3 downto 0);

  code_rm_vld <= code_flag and (code_is_msr0 or code_is_dp0 or code_is_bx or code_is_dp1 or code_is_mult or code_is_multl or code_is_swp or code_is_ldrh0 or code_is_ldrsb0 or code_is_ldrsh0 or code_is_ldr1);

  code_rn_num <= code(19 downto 16);

  code_rn_vld <= code_flag and (code_is_dp0 or code_is_dp1 or code_is_multl or code_is_swp or code_is_ldrh0 or code_is_ldrh1 or code_is_ldrsb0 or code_is_ldrsb1 or code_is_ldrsh0 or code_is_ldrsh1 or code_is_dp2 or code_is_ldr0 or code_is_ldr1 or code_is_ldm);

  code_rnhi_num <= code(15 downto 12);

  code_rnhi_vld <= code_flag and (code_is_mult or code_is_multl or ((code_is_ldrh0 or code_is_ldrh1 or code_is_ldr0 or code_is_ldr1) and not code(20)));

  code_rs_num <= code(11 downto 8);

  code_rs_vld <= code_flag and (code_is_dp1 or code_is_mult or code_is_multl);

  -- code_sum_m <= (code(0)+code(1)+code(2)+code(3)+code(4)+code(5)+code(6)+code(7)+code(8)+code(9)+code(10)+code(11)+code(12)+code(13)+code(14)+code(15));
  code_sum_m <= std_logic_vector(to_unsigned(hw_tree(code(15 downto 0)), code_sum_m'length));

  cpsr <= (cpsr_n & cpsr_z & cpsr_c & cpsr_v & cpsr_i & cpsr_f & cpsr_m);

  eor_ans <= rnb xor sec_operand;

  fiq_en <= fiq_flag and cmd_flag and not cpsr_f;

  go_rf_vld <= '1' when go_vld = '1' and (go_num = X"f") else '0';

  high_bit <= high_middle(0);

  high_middle <= std_logic_vector(unsigned'('0' & add_a(31)) + unsigned'('0' & add_b(31)) + unsigned'('0' & sum_middle(31)));

  hold_en <= '1' when cmd_ok = '1' and (cmd_is_swp = '1' or cmd_is_multl = '1' or (cmd_is_ldm = '1' and (cmd_sum_m /= "00000"))) else '0';

  int_all <= cpu_restart or ram_abort or fiq_en or irq_en or (cmd_flag and (code_abort or code_und or (cond_satisfy and cmd_is_swi)));

  irq_en <= irq_flag and cmd_flag and not cpsr_i;

  ldm_data <= go_data;

  ldm_rf_vld <= '1' when (ldm_vld = '1' and (ldm_num = X"f")) or ((cmd_ok = '1' and cmd_is_ldm = '1' and cmd(20) = '1') and (ldm_sel = X"f")) else '0';

  --mult_ans <= std_logic_vector(to_unsigned(to_integer(unsigned(code_rm)) * to_integer(unsigned(code_rs)), mult_ans'length));
  mult_ans <= code_rm * code_rs;

  or_ans <= rnb or sec_operand;

  r8 <= r8_fiq when (cpsr_m = "10001") else
        r8_usr;

  r9 <= r9_fiq when (cpsr_m = "10001") else
        r9_usr;

  ra <= ra_fiq when (cpsr_m = "10001") else
        ra_usr;

  ram_addr <= (cmd_addr(31 downto 2) & "00");

  ram_cen <= '1' when cpu_en = '1' and cmd_ok = '1' and (cmd_is_ldrh0 = '1' or cmd_is_ldrh1 = '1' or cmd_is_ldrsb0 = '1' or cmd_is_ldrsb1 = '1' or 
  cmd_is_ldrsh0 = '1' or 
  cmd_is_ldrsh1 = '1' or cmd_is_ldr0 = '1' or cmd_is_ldr1 = '1' or cmd_is_swp = '1' or 
  cmd_is_swpx = '1' or (cmd_is_ldm = '1' and (cmd_sum_m /= "00000"))) else '0';

  ram_wen <= '0' when cmd_is_swp else not cmd(20);

  rb <= rb_fiq when (cpsr_m = "10001") else
        rb_usr;

  rc <= rc_fiq when (cpsr_m = "10001") else
        rc_usr;

  rf_b <= std_logic_vector(unsigned(rf) - x"00000004");
  --rf_b <= rf - 4;

  rom_addr <= rf;

  rom_en <= cpu_en and (not (int_all or to_rf_vld or cha_rf_vld or go_rf_vld or wait_en or hold_en));
  
  -- sum_middle <= add_a(30 downto 0) + add_b(30 downto 0) + addc
  sum_middle <= std_logic_vector(resize(unsigned(add_a(30 downto 0)), sum_middle'length) + 
                                 resize(unsigned(add_b(30 downto 0)), sum_middle'length) + 
                                 resize(unsigned'('0' & add_c), 32));

  sum_rn_rm <= (high_bit & sum_middle(30 downto 0));

  to_rf_vld <= '1' when cmd_ok = '1' and (((cmd(15 downto 12) = X"f") and ((cmd_is_dp0 = '1' or cmd_is_dp1 = '1' or cmd_is_dp2 = '1') and (cmd(24 downto 23) /= "10"))) or (cmd_is_b = '1' or cmd_is_bx = '1')) else '0';

  to_vld <= '1' when cmd_ok = '1' and (cmd_is_mrs = '1' or ((cmd_is_dp0 = '1' or cmd_is_dp1 = '1' or cmd_is_dp2 = '1') and 
  (cmd(24 downto 23) /= "10")) or cmd_is_mult = '1' or cmd_is_multl = '1' or cmd_is_multlx = '1' or 
  ((cmd_is_ldrh0 = '1' or cmd_is_ldrh1 = '1' or cmd_is_ldrsb0 = '1' or cmd_is_ldrsb1 = '1' or cmd_is_ldrsh0 = '1' or cmd_is_ldrsh1 = '1' 
  or cmd_is_ldr0 = '1' or cmd_is_ldr1 = '1') and (cmd(21) = '1' or cmd(24) = '0')) or (cmd_is_ldm = '1' and (cmd_sum_m = "00000") and cmd(21) = '1')) else '0';

  wait_en <= '1' when (code_rm_vld = '1' and cha_vld = '1' and (cha_num = code_rm_num)) or (code_rm_vld = '1' and to_vld = '1' and (to_num = code_rm_num)) or 
  (code_rm_vld = '1' and go_vld = '1' and (go_num = code_rm_num)) or (code_rs_vld = '1' and cha_vld = '1' and (cha_num = code_rs_num)) or 
  (code_rs_vld = '1' and to_vld = '1' and (to_num = code_rs_num)) or (code_rs_vld = '1' and go_vld = '1' and (go_num = code_rs_num)) or 
  (code_rn_vld = '1' and cha_vld = '1' and (code_rn_num = cha_num)) or (code_rnhi_vld = '1' and cha_vld = '1' and (code_rnhi_num = cha_num)) or
  (code_rm_vld = '1' and (ldm_vld = '1' and hold_en = '0') and (ldm_num = code_rm_num)) or (code_rs_vld = '1' and 
  (ldm_vld = '1' and hold_en = '0') and (ldm_num = code_rs_num)) else '0';

  --*****************************************************/
  --register statement area
  --*****************************************************/
  processing_0 : process (all)
  begin

    if (cmd_is_mult or cmd_is_b or cmd_is_bx) then
      add_b <= sec_operand;
    elsif (cmd_is_multl or cmd_is_multlx) then
      if (cmd(22) and (rm_msb xor rs_msb)) then
        add_b <= not sec_operand;
      else
        add_b <= sec_operand;
      end if;
    elsif (cmd_is_dp0 or cmd_is_dp1 or cmd_is_dp2) then
      if ((cmd(24 downto 21) = "0010") or (cmd(24 downto 21) = "0110") or (cmd(24 downto 21) = "1010") or (cmd(24 downto 21) = "1111")) then
        add_b <= not sec_operand;
      else
        add_b <= sec_operand;
      end if;
    elsif (not cmd(23)) then
      add_b <= not sec_operand;
    else
      add_b <= sec_operand;
    end if;
  end process;

  processing_1 : process (all)
  begin
    if (cmd_is_mult or cmd_is_b or cmd_is_bx) then
      add_c <= '0';
    elsif (cmd_is_multl) then
      if (cmd(22) and (rm_msb xor rs_msb)) then
        add_c <= '1';
      else
        add_c <= '0';
      end if;
    elsif (cmd_is_multlx) then
      add_c <= multl_extra_num;
    elsif (cmd_is_dp0 or cmd_is_dp1 or cmd_is_dp2) then
      if ((cmd(24 downto 21) = "0101") or (cmd(24 downto 21) = "0110") or (cmd(24 downto 21) = "0111")) then
        add_c <= cpsr_c;
      elsif ((cmd(24 downto 21) = "0010") or (cmd(24 downto 21) = "0011") or (cmd(24 downto 21) = "1010")) then
        add_c <= '1';
      else
        add_c <= '0';
      end if;
    elsif (not cmd(23)) then
      add_c <= '1';
    else

      add_c <= '0';
    end if;
  end process;

  processing_2 : process (all)
  begin
    if (code(27 downto 25) = "000") then
      if (not code(4)) then
        if ((code(24 downto 23) = "10") and code(20) = '0') then
          if (not code(21)) then
            all_code <= '1' when (code(19 downto 16) = X"f") and (code(11 downto 0) = "000000000000") else '0';
          else
            all_code <= '1' when (code(18 downto 17) = "00") and (code(15 downto 12) = X"f") and (code(11 downto 4) = X"0") else '0';
          end if;
        else
          all_code <= '1' when (code(24 downto 23) /= "10") or code(20) = '1' else '0';
        end if;
      elsif (not code(7)) then
        if (code(24 downto 20) = "10010") then
          all_code <= '1' when (code(19 downto 4) = X"fff1") else '0';
        else
          all_code <= '1' when (code(24 downto 23) /= "10") or code(20) = '1' else '0';
        end if;
      elsif (code(6 downto 5) = "00") then
        if (code(24 downto 22) = "000") then
          all_code <= '1';
        elsif (code(24 downto 23) = "01") then
          all_code <= '1';
        elsif (code(24 downto 23) = "10") then
          all_code <= '1' when (code(21 downto 20) = "00") and (code(11 downto 8) = "0000") else '0';
        else
          all_code <= '0';
        end if;
      elsif (code(6 downto 5) = "01") then
        if (not code(22)) then
          all_code <= '1' when (code(11 downto 8) = "0000") else '0';
        else
          all_code <= '1';
        end if;
      --if ( ( code[6:5]==2'b10 )|(code[6:5]==2'b11) )
      elsif (code(20)) then
        if (not code(22)) then
          all_code <= '1' when (code(11 downto 8) = "0000") else '0';
        else
          all_code <= '1';
        end if;
      else
        all_code <= '0';
      end if;
    elsif (code(27 downto 25) = "001") then
      if ((code(24 downto 23) = "10") and code(20) = '0') then
        all_code <= '1' when code(21) = '1' and (code(18 downto 17) = "00") and (code(15 downto 12) = X"f") else '0';
      else
        all_code <= '1' when (code(24 downto 23) /= "10") or code(20) = '1' else '0';
      end if;
    elsif (code(27 downto 25) = "010") then
      all_code <= '1';
    elsif (code(27 downto 25) = "011") then
      all_code <= not code(4);
    elsif (code(27 downto 25) = "100") then
      all_code <= '1';
    elsif (code(27 downto 25) = "101") then
      all_code <= '1';
    elsif (code(27 downto 25) = "111") then
      all_code <= code(24);
    else
      all_code <= '0';
    end if;
  end process;

  processing_3 : process (all)
  begin
    cha_num <= cmd(15 downto 12);
  end process;

  processing_4 : process (all)
  begin
    if (cmd_ok) then
      cha_vld <= ((cmd_is_ldrh0 or cmd_is_ldrh1 or cmd_is_ldrsb0 or cmd_is_ldrsb1 or cmd_is_ldrsh0 or cmd_is_ldrsh1 or cmd_is_ldr0 or cmd_is_ldr1) and cmd(20)) or cmd_is_swp;
    else
      cha_vld <= '0';
    end if;
  end process;

  processing_5 : process (clk, rst)
  begin
    if (rst) then
      cmd <= x"00000000";
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (not hold_en) then
          cmd <= code;
        elsif (cmd_is_swp) then
          cmd(27 downto 25) <= "110";
          cmd(15 downto 12) <= cmd(3 downto 0);
        elsif (cmd_is_multl) then
          cmd(27 downto 25) <= "110";
        elsif (cmd_is_ldm) then
          cmd(0) <= '0';
          cmd(1) <= cmd(1)
        when cmd(0) else '0';
          cmd(2) <= cmd(2)
        when (or (cmd(1 downto 0))) else '0';
          cmd(3) <= cmd(3)
        when (or (cmd(2 downto 0))) else '0';
          cmd(4) <= cmd(4)
        when (or (cmd(3 downto 0))) else '0';
          cmd(5) <= cmd(5)
        when (or (cmd(4 downto 0))) else '0';
          cmd(6) <= cmd(6)
        when (or (cmd(5 downto 0))) else '0';
          cmd(7) <= cmd(7)
        when (or (cmd(6 downto 0))) else '0';
          cmd(8) <= cmd(8)
        when (or (cmd(7 downto 0))) else '0';
          cmd(9) <= cmd(9)
        when (or (cmd(8 downto 0))) else '0';
          cmd(10) <= cmd(10)
        when (or (cmd(9 downto 0))) else '0';
          cmd(11) <= cmd(11)
        when (or (cmd(10 downto 0))) else '0';
          cmd(12) <= cmd(12)
        when (or (cmd(11 downto 0))) else '0';
          cmd(13) <= cmd(13)
        when (or (cmd(12 downto 0))) else '0';
          cmd(14) <= cmd(14)
        when (or (cmd(13 downto 0))) else '0';
          cmd(15) <= cmd(15)
        when (or (cmd(14 downto 0))) else '0';
        else
          null;
        end if;
      else
        null;
      end if;
    end if;
  end process;

  processing_6 : process (all)
  begin
    if (cmd_is_ldm) then
      cmd_addr <= sum_rn_rm;
    elsif (cmd_is_swp or cmd_is_swpx) then
      cmd_addr <= rn;
    elsif (cmd(24)) then
      cmd_addr <= sum_rn_rm;
    else
      cmd_addr <= rn;
    end if;
  end process;

  processing_7 : process (clk, rst)
  begin
    if (rst) then
      cmd_flag <= '0';
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (int_all) then
          cmd_flag <= '0';
        elsif (not hold_en) then
          if (wait_en or to_rf_vld or cha_rf_vld or go_rf_vld) then
            cmd_flag <= '0';
          else
            cmd_flag <= code_flag;
          end if;
        else
          null;
        end if;
      else
        null;
      end if;
    end if;
  end process;
  processing_8 : process (clk, rst)
  begin
    if (rst) then
      code_abort <= '0';
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (not hold_en) then
          code_abort <= rom_abort;
        else
          null;
        end if;
      else
        null;
      end if;
    end if;
  end process;
  processing_9 : process (clk, rst)
  begin
    if (rst) then
      code_flag <= '0';
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (int_all or to_rf_vld or cha_rf_vld or go_rf_vld or ldm_rf_vld) then
          code_flag <= '0';
        else
          code_flag <= '1';
        end if;
      else
        null;
      end if;
    end if;
  end process;
  processing_10 : process (all)
  begin
    if (code_is_ldrh1 or code_is_ldrsb1 or code_is_ldrsh1) then
      --code_rm <= resize(code(11 downto 8) & code(3 downto 0), code_rm'length);
      code_rm <= (7 downto 4 => code(11 downto 8), 3 downto 0 => code(3 downto 0), others => '0');
    elsif (code_is_b) then      
      -- code_rm <= (concatenate(6, code(23)) & code(23 downto 0) & '0');      
      code_rm <= code(23) & code(23) & code(23) & code(23) & code(23) & code(23) & code(23 downto 0) & "00";
    elsif (code_is_ldm) then
      case ((code(24 downto 23))) is
      when "00" =>
        code_rm <= std_logic_vector(to_unsigned(to_integer(unsigned(code_sum_m))-1, 30)) & "00";
      when "01" =>
        code_rm <= x"00000000";
      when "10" =>
        code_rm <= (25b"0" & code_sum_m & "00");
      when "11" =>
        code_rm <= 32b"100";
      when others => code_rm <= x"00000000";
      end case;
    elsif (code_is_ldr0) then
      code_rm <= 20b"0" & code(11 downto 0);
    elsif (code_is_msr1 or code_is_dp2) then
      code_rm <= (7 downto 0 => code(7 downto 0), others => '0');
    elsif (code_is_multl and code(22) and code_rma(31)) then
      code_rm <= std_logic_vector(to_unsigned(to_integer(unsigned(not code_rma)) + 1, 32));
    elsif (((code(6 downto 5) = "10") and code_rma(31) = '1') and (code_is_dp0 = '1' or code_is_dp1 = '1' or code_is_ldr1 = '1')) then
      code_rm <= not code_rma;
    else
      code_rm <= code_rma;
    end if;
  end process;
  processing_11 : process (all)
  begin
    case ((code(3 downto 0))) is
    when X"0" =>
      code_rma <= r0;
    when X"1" =>
      code_rma <= r1;
    when X"2" =>
      code_rma <= r2;
    when X"3" =>
      code_rma <= r3;
    when X"4" =>
      code_rma <= r4;
    when X"5" =>
      code_rma <= r5;
    when X"6" =>
      code_rma <= r6;
    when X"7" =>
      code_rma <= r7;
    when X"8" =>
      code_rma <= r8;
    when X"9" =>
      code_rma <= r9;
    when X"a" =>
      code_rma <= ra;
    when X"b" =>
      code_rma <= rb;
    when X"c" =>
      code_rma <= rc;
    when X"d" =>
      code_rma <= rd;
    when X"e" =>
      code_rma <= re;
    when X"f" =>
      code_rma <= std_logic_vector(to_unsigned(to_integer(unsigned(rf))+4, 32));
    when others =>
      code_rma <= (others => '0');
    end case;
  end process;


  processing_12 : process (all)
  begin
    if (code_is_dp0 or code_is_ldr1) then
      code_rot_num <= code(11 downto 7) when (code(6 downto 5) = "00") else 
                      std_logic_vector(resize(unsigned(not code(11 downto 7)) + 1, code_rot_num'length));
    elsif (code_is_dp1) then
      code_rot_num <= code_rsa(4 downto 0) when (code(6 downto 5) = "00") else 
                      std_logic_vector(resize(unsigned(not code_rsa(4 downto 0)) + 1, code_rot_num'length));
    elsif (code_is_msr1 or code_is_dp2) then
      -- code_rot_num <= std_logic_vector(to_unsigned(to_integer(unsigned(not code(11 downto 8)))+1, 4)) & '0';
      code_rot_num <= std_logic_vector(unsigned(not code(11 downto 8)) + 1) & '0';
    else
      code_rot_num <= "00000";
    end if;
  end process;
  
  processing_13 : process ( code_is_multl, code, code_rsa, code_is_mult, code_rot_num)
  begin
    if (code_is_multl) then
      if (code(22) and code_rsa(31)) then
        code_rs <= std_logic_vector(to_unsigned(to_integer(unsigned(not code_rsa))+1, 32));
      else
        code_rs <= code_rsa;
      end if;
    elsif (code_is_mult) then
      code_rs <= code_rsa;
    else
      code_rs <= (others => '0');
      code_rs(to_integer(unsigned(code_rot_num))) <= '1';
    end if;
  end process;


  processing_14 : process (clk, rst)
  begin
    if (rst) then
      code_rs_flag <= "000";
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (not hold_en) then
          if (code_is_dp1) then
            code_rs_flag <= (to_std_logic(code_rsa(7 downto 0) > "100000") & 
                             to_std_logic(code_rsa(7 downto 0) = "00100000") & 
                             to_std_logic(code_rsa(7 downto 0) = "00000000"));
          else
            code_rs_flag <= "000";
          end if;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_15 : process (all)
  begin
    case ((code(11 downto 8))) is
    when X"0" =>
      code_rsa <= r0;
    when X"1" =>
      code_rsa <= r1;
    when X"2" =>
      code_rsa <= r2;
    when X"3" =>
      code_rsa <= r3;
    when X"4" =>
      code_rsa <= r4;
    when X"5" =>
      code_rsa <= r5;
    when X"6" =>
      code_rsa <= r6;
    when X"7" =>
      code_rsa <= r7;
    when X"8" =>
      code_rsa <= r8;
    when X"9" =>
      code_rsa <= r9;
    when X"a" =>
      code_rsa <= ra;
    when X"b" =>
      code_rsa <= rb;
    when X"c" =>
      code_rsa <= rc;
    when X"d" =>
      code_rsa <= rd;
    when X"e" =>
      code_rsa <= re;
    when X"f" =>
      code_rsa <= std_logic_vector(to_unsigned(to_integer(unsigned(rf))+4, 32));
    when others =>
      code_rsa <= (others => '0');
    end case;
  end process;


  processing_16 : process (clk, rst)
  begin
    if (rst) then
      code_und <= '0';
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (not hold_en) then
          code_und <= not all_code;
        else
          null;
        end if;
      else
        null;
      end if;
    end if;
  end process;
  processing_17 : process (all)
  begin
    case ((cmd(31 downto 28))) is
    when X"0" =>
      cond_satisfy <= '1' when (cpsr_z = '1') else '0';
    when X"1" =>
      cond_satisfy <= '1' when (cpsr_z = '0') else '0';
    when X"2" =>
      cond_satisfy <= '1' when (cpsr_c = '1') else '0';
    when X"3" =>
      cond_satisfy <= '1' when (cpsr_c = '0') else '0';
    when X"4" =>
      cond_satisfy <= '1' when (cpsr_n = '1') else '0';
    when X"5" =>
      cond_satisfy <= '1' when (cpsr_n = '0') else '0';
    when X"6" =>
      cond_satisfy <= '1' when (cpsr_v = '1') else '0';
    when X"7" =>
      cond_satisfy <= '1' when (cpsr_v = '0') else '0';
    when X"8" =>
      cond_satisfy <= '1' when (cpsr_c = '1') and (cpsr_z = '0') else '0';
    when X"9" =>
      cond_satisfy <= '1' when (cpsr_c = '0') or (cpsr_z = '1') else '0';
    when X"a" =>
      cond_satisfy <= '1' when (cpsr_n = cpsr_v) else '0';
    when X"b" =>
      cond_satisfy <= '1' when (cpsr_n /= cpsr_v) else '0';
    when X"c" =>
      cond_satisfy <= '1' when (cpsr_z = '0') and (cpsr_n = cpsr_v) else '0';
    when X"d" =>
      cond_satisfy <= '1' when (cpsr_z = '1') or (cpsr_n /= cpsr_v) else '0';
    when X"e" =>
      cond_satisfy <= '1';
    when X"f" =>
      cond_satisfy <= '0';
    when others =>
      cond_satisfy <= '0';
    end case;
  end process;


  processing_18 : process (clk, rst)
  begin
    if (rst) then
      cpsr_c <= '0';
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (cmd_ok) then
          if (cmd_is_msr0 or cmd_is_msr1) then
            if (not cmd(22) and cmd(19)) then
              cpsr_c <= sec_operand(29);
            else
              null;
            end if;
          elsif (cmd_is_dp0 or cmd_is_dp1 or cmd_is_dp2) then
            if (cmd(20)) then
              if (cmd(15 downto 12) = X"f") then
                cpsr_c <= spsr(8);
              elsif ((cmd(24 downto 21) = "1011") or (cmd(24 downto 21) = "0100") or (cmd(24 downto 21) = "0101") or (cmd(24 downto 21) = "0011") or (cmd(24 downto 21) = "0111")) then
                cpsr_c <= bit_cy;
              elsif ((cmd(24 downto 21) = "1010") or (cmd(24 downto 21) = "0010") or (cmd(24 downto 21) = "0110")) then
                cpsr_c <= bit_cy;
              elsif (cmd_is_dp1 and not code_rs_flag(0)) then
                case ((cmd(6 downto 5))) is
                when "00" =>
                  cpsr_c <= '0'
                when code_rs_flag(2) else reg_ans(0)
                when code_rs_flag(1) else reg_ans(32);
                when "01" =>
                  cpsr_c <= '0'
                when code_rs_flag(2) else reg_ans(31)
                when code_rs_flag(1) else reg_ans(31);
                when "10" =>
                  cpsr_c <= rm_msb when code_rs_flag(2) else 
                            rm_msb when code_rs_flag(1) else
                            not reg_ans(31) when rm_msb else
                            reg_ans(31);
                when "11" =>
                  cpsr_c <= cpsr_c
                when code_rs_flag(1) else reg_ans(31);
                when others =>
                  cpsr_c <= '0';
                end case;
              elsif (cmd_is_dp2) then
                cpsr_c <= reg_ans(31);
              elsif (cmd_is_dp0) then
                case ((cmd(6 downto 5))) is
                when "00" =>
                  cpsr_c <= cpsr_c
                when (cmd(11 downto 7) = "00000") else reg_ans(32);
                when "01" =>
                  cpsr_c <= reg_ans(31);
                when "10" =>
                  cpsr_c <= not reg_ans(31) when (rm_msb) else
                            reg_ans(31);
                when "11" =>
                  cpsr_c <= reg_ans(0) when (cmd(11 downto 7) = "00000") else
                            reg_ans(31);
                when others =>
                  cpsr_c <= '0';
                end case;
              else
                null;
              end if;
            else
              null;
            end if;
          elsif (cmd_is_ldm = '1' and (cmd_sum_m = "00000") and ldm_change = '1') then
            cpsr_c <= spsr(8);
          else
            null;
          end if;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_19 : process (clk, rst)
  begin
    if (rst) then
      cpsr_f <= '0';
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (cpu_restart or fiq_en) then
          cpsr_f <= '1';
        elsif (cmd_ok = '1' and (cpsr_m /= "10000")) then
          if (cmd_is_msr0 or cmd_is_msr1) then
            if (not cmd(22) and cmd(16)) then
              cpsr_f <= sec_operand(6);
            else
              null;
            end if;
          elsif (cmd_is_dp0 or cmd_is_dp1 or cmd_is_dp2) then
            if (cmd(20)) then
              if (cmd(15 downto 12) = X"f") then
                cpsr_f <= spsr(5);
              else
                null;
              end if;
            else
              null;
            end if;
          elsif (cmd_is_ldm = '1' and (cmd_sum_m = "00000") and ldm_change = '1') then
            cpsr_f <= spsr(5);
          else
            null;
          end if;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_20 : process (clk, rst)
  begin
    if (rst) then
      cpsr_i <= '0';
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (int_all) then
          cpsr_i <= '1';
        elsif (cmd_ok = '1' and (cpsr_m /= "10000")) then
          if (cmd_is_msr0 or cmd_is_msr1) then
            if (not cmd(22) and cmd(16)) then
              cpsr_i <= sec_operand(7);
            else
              null;
            end if;
          elsif (cmd_is_dp0 or cmd_is_dp1 or cmd_is_dp2) then
            if (cmd(20)) then
              if (cmd(15 downto 12) = X"f") then
                cpsr_i <= spsr(6);
              else
                null;
              end if;
            else
              null;
            end if;
          elsif (cmd_is_ldm = '1' and (cmd_sum_m = "00000") and ldm_change = '1') then
            cpsr_i <= spsr(6);
          else
            null;
          end if;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_21 : process (clk, rst)
  begin
    if (rst) then
      cpsr_m <= "10011";
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (cpu_restart) then
          cpsr_m <= "10011";
        elsif (fiq_en) then
          cpsr_m <= "10001";
        elsif (ram_abort) then
          cpsr_m <= "10111";
        elsif (irq_en) then
          cpsr_m <= "10010";
        elsif (cmd_flag and code_abort) then
          cpsr_m <= "10111";
        elsif (cmd_flag and code_und) then
          cpsr_m <= "11011";
        elsif (cmd_flag and cond_satisfy and cmd_is_swi) then
          cpsr_m <= "10011";
        elsif (cmd_ok = '1' and (cpsr_m /= "10000")) then
          if (cmd_is_msr0 or cmd_is_msr1) then
            if (not cmd(22) and cmd(16)) then
              cpsr_m <= sec_operand(4 downto 0);
            else
              null;
            end if;
          elsif (cmd_is_dp0 or cmd_is_dp1 or cmd_is_dp2) then
            if (cmd(20)) then
              if (cmd(15 downto 12) = X"f") then
                cpsr_m <= spsr(4 downto 0);
              else
                null;
              end if;
            else
              null;
            end if;
          elsif (cmd_is_ldm = '1' and (cmd_sum_m = "00000") and ldm_change = '1') then
            cpsr_m <= spsr(4 downto 0);
          else
            null;
          end if;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_22 : process (clk, rst)
  begin
    if (rst) then
      cpsr_n <= '0';
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (cmd_ok) then
          if (cmd_is_msr0 or cmd_is_msr1) then
            if (not cmd(22) and cmd(19)) then
              cpsr_n <= sec_operand(31);
            else
              null;
            end if;
          elsif (cmd_is_dp0 or cmd_is_dp1 or cmd_is_dp2) then
            if (cmd(20)) then
              if (cmd(15 downto 12) = X"f") then
                cpsr_n <= spsr(10);
              else
                cpsr_n <= dp_ans(31);
              end if;
            else
              null;
            end if;
          elsif (cmd_is_mult or cmd_is_multlx) then
            if (cmd(20)) then
              cpsr_n <= sum_rn_rm(31);
            else
              null;
            end if;
          elsif (cmd_is_ldm = '1' and (cmd_sum_m = "00000") and ldm_change = '1') then
            cpsr_n <= spsr(10);
          else
            null;
          end if;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_23 : process (clk, rst)
  begin
    if (rst) then
      cpsr_v <= '0';
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (cmd_ok) then
          if (cmd_is_msr0 or cmd_is_msr1) then
            if (not cmd(22) and cmd(19)) then
              cpsr_v <= sec_operand(28);
            else
              null;
            end if;
          elsif (cmd_is_dp0 or cmd_is_dp1 or cmd_is_dp2) then
            if (cmd(20)) then
              if (cmd(15 downto 12) = X"f") then
                cpsr_v <= spsr(7);
              elsif ((cmd(24 downto 21) = "0010") or (cmd(24 downto 21) = "0010") or (cmd(24 downto 21) = "0100") or 
              (cmd(24 downto 21) = "0101") or (cmd(24 downto 21) = "0110") or 
              (cmd(24 downto 21) = "0111") or (cmd(24 downto 21) = "1010") or 
              (cmd(24 downto 21) = "1011")) then
                cpsr_v <= bit_ov;
              else
                null;
              end if;
            else
              null;
            end if;
          elsif (cmd_is_ldm = '1' and (cmd_sum_m = "00000") and ldm_change = '1') then
            cpsr_v <= spsr(7);
          else
            null;
          end if;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_24 : process (clk, rst)
  begin
    if (rst) then
      cpsr_z <= '0';
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (cmd_ok) then
          if (cmd_is_msr0 or cmd_is_msr1) then
            if (not cmd(22) and cmd(19)) then
              cpsr_z <= sec_operand(30);
            else
              null;
            end if;
          elsif (cmd_is_dp0 or cmd_is_dp1 or cmd_is_dp2) then
            if (cmd(20)) then
              if (cmd(15 downto 12) = X"f") then
                cpsr_z <= spsr(9);
              else
                cpsr_z <= '1' when (dp_ans = 32x"0") else '0';
              end if;
            else
              null;
            end if;
          elsif (cmd_is_mult and cmd(20)) then
            cpsr_z <= '1' when (sum_rn_rm = 32x"0") else '0';
          elsif (cmd_is_multlx and cmd(20)) then
            cpsr_z <= '1' when mult_z = '1' and (sum_rn_rm = 32x"0") else '0';
          elsif (cmd_is_ldm = '1' and (cmd_sum_m = "00000") and ldm_change = '1') then
            cpsr_z <= spsr(9);
          else
            null;
          end if;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_25 : process (all)
  begin
    case ((cmd(24 downto 21))) is
    when X"0" =>
      dp_ans <= and_ans;
    when X"1" =>
      dp_ans <= eor_ans;
    when X"2" =>
      dp_ans <= sum_rn_rm;
    when X"3" =>
      dp_ans <= sum_rn_rm;
    when X"4" =>
      dp_ans <= sum_rn_rm;
    when X"5" =>
      dp_ans <= sum_rn_rm;
    when X"6" =>
      dp_ans <= sum_rn_rm;
    when X"7" =>
      dp_ans <= sum_rn_rm;
    when X"8" =>
      dp_ans <= and_ans;
    when X"9" =>
      dp_ans <= eor_ans;
    when X"a" =>
      dp_ans <= sum_rn_rm;
    when X"b" =>
      dp_ans <= sum_rn_rm;
    when X"c" =>
      dp_ans <= or_ans;
    when X"d" =>
      dp_ans <= sum_rn_rm;
    when X"e" =>
      dp_ans <= bic_ans;
    when X"f" =>
      dp_ans <= sum_rn_rm;
    when others =>
      dp_ans <= 32x"0";
    end case;
  end process;


  processing_26 : process (clk, rst)
  begin
    if (rst) then
      fiq_flag <= '0';
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (fiq) then
          fiq_flag <= '1';
        elsif (cmd_flag) then
          fiq_flag <= '0';
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_27 : process (all)
  begin
    if (go_fmt(5)) then
      go_data <= ram_rdata;
    elsif (go_fmt(4)) then
      if (go_fmt(1)) then
        go_data <= (concatenate(16, go_fmt(2) and ram_rdata(31)) & ram_rdata(31 downto 16));
      else
        go_data <= (concatenate(16, go_fmt(2) and ram_rdata(15)) & ram_rdata(15 downto 0));
      end if;
    else    -- if ( cha_reg_fmt[3] )
      case ((go_fmt(1 downto 0))) is
      when "00" =>
        go_data <= (concatenate(24, go_fmt(2) and ram_rdata(7)) & ram_rdata(7 downto 0));
      when "01" =>
        go_data <= (concatenate(24, go_fmt(2) and ram_rdata(15)) & ram_rdata(15 downto 8));
      when "10" =>
        go_data <= (concatenate(24, go_fmt(2) and ram_rdata(23)) & ram_rdata(23 downto 16));
      when "11" =>
        go_data <= (concatenate(24, go_fmt(2) and ram_rdata(31)) & ram_rdata(31 downto 24));
      when others =>
        go_data <= (others => '0');
      end case;
    end if;
  end process;


  processing_28 : process (clk, rst)
  begin
    if (rst) then
      go_fmt <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (cmd_is_ldr0 or cmd_is_ldr1 or cmd_is_swp) then
          go_fmt <= ("0010" & cmd_addr(1 downto 0))
        when cmd(22) else ("1000" & cmd_addr(1 downto 0));
        elsif (cmd_is_ldrh0 or cmd_is_ldrh1) then
          go_fmt <= ("0100" & cmd_addr(1 downto 0));
        elsif (cmd_is_ldrsb0 or cmd_is_ldrsb1) then
          go_fmt <= ("0011" & cmd_addr(1 downto 0));
        elsif (cmd_is_ldrsh0 or cmd_is_ldrsh1) then
          go_fmt <= ("0101" & cmd_addr(1 downto 0));
        elsif (cmd_is_ldm) then
          go_fmt <= ("1000" & cmd_addr(1 downto 0));
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_29 : process (clk, rst)
  begin
    if (rst) then
      go_num <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        go_num <= cha_num;
      else

        null;
      end if;
    end if;
  end process;
  processing_30 : process (clk, rst)
  begin
    if (rst) then
      go_vld <= '0';
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        go_vld <= cha_vld;
      else

        null;
      end if;
    end if;
  end process;
  processing_31 : process (clk, rst)
  begin
    if (rst) then
      hold_en_dly <= '0';
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        hold_en_dly <= hold_en;
      else

        null;
      end if;
    end if;
  end process;
  processing_32 : process (clk, rst)
  begin
    if (rst) then
      irq_flag <= '0';
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (irq) then
          irq_flag <= '1';
        elsif (cmd_flag) then
          irq_flag <= '0';
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_33 : process (clk, rst)
  begin
    if (rst) then
      ldm_change <= '0';
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (not hold_en) then
          ldm_change <= code(22) and code(20) and code(15);
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_34 : process (clk, rst)
  begin
    if (rst) then
      ldm_num <= "0000";
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (cmd_is_ldm) then
          ldm_num <= ldm_sel;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_35 : process (all)
  begin
    if (cmd(0)) then
      ldm_sel <= X"0";
    elsif (cmd(1)) then
      ldm_sel <= X"1";
    elsif (cmd(2)) then
      ldm_sel <= X"2";
    elsif (cmd(3)) then
      ldm_sel <= X"3";
    elsif (cmd(4)) then
      ldm_sel <= X"4";
    elsif (cmd(5)) then
      ldm_sel <= X"5";
    elsif (cmd(6)) then
      ldm_sel <= X"6";
    elsif (cmd(7)) then
      ldm_sel <= X"7";
    elsif (cmd(8)) then
      ldm_sel <= X"8";
    elsif (cmd(9)) then
      ldm_sel <= X"9";
    elsif (cmd(10)) then
      ldm_sel <= X"a";
    elsif (cmd(11)) then
      ldm_sel <= X"b";
    elsif (cmd(12)) then
      ldm_sel <= X"c";
    elsif (cmd(13)) then
      ldm_sel <= X"d";
    elsif (cmd(14)) then
      ldm_sel <= X"e";
    elsif (cmd(15)) then
      ldm_sel <= X"f";
    else

      ldm_sel <= X"0";
    end if;
  end process;
  processing_36 : process (clk, rst)
  begin
    if (rst) then
      ldm_usr <= '0';
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        ldm_usr <= cmd_ok and cmd_is_ldm and cmd(20) and cmd(22) and not cmd(15);
      else

        null;
      end if;
    end if;
  end process;
  processing_37 : process (clk, rst)
  begin
    if (rst) then
      ldm_vld <= '0';
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        ldm_vld <= '1' when cmd_ok = '1' and cmd_is_ldm = '1' and cmd(20) = '1' and (cmd_sum_m /= "00000") else '0';
      else

        null;
      end if;
    end if;
  end process;
  processing_38 : process (clk, rst)
  begin
    if (rst) then
      mult_z <= '0';
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (cmd_ok and cmd_is_multl and cmd(20)) then
          mult_z <= '1' when (sum_rn_rm = x"00000000") else '0';
        else
          mult_z <= '0';
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_39 : process (clk, rst)
  begin
    if (rst) then
      multl_extra_num <= '0';
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (cmd_ok and cmd_is_multl) then
          multl_extra_num <= bit_cy;
        else
          multl_extra_num <= '0';
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_40 : process (clk, rst)
  begin
    if (rst) then
      r0 <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"0")) then
          r0 <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"0")) then
          r0 <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"0")) then
          r0 <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_41 : process (clk, rst)
  begin
    if (rst) then
      r1 <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"1")) then
          r1 <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"1")) then
          r1 <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"1")) then
          r1 <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_42 : process (clk, rst)
  begin
    if (rst) then
      r2 <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"2")) then
          r2 <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"2")) then
          r2 <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"2")) then
          r2 <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_43 : process (clk, rst)
  begin
    if (rst) then
      r3 <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"3")) then
          r3 <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"3")) then
          r3 <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"3")) then
          r3 <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_44 : process (clk, rst)
  begin
    if (rst) then
      r4 <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"4")) then
          r4 <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"4")) then
          r4 <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"4")) then
          r4 <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_45 : process (clk, rst)
  begin
    if (rst) then
      r5 <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"5")) then
          r5 <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"5")) then
          r5 <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"5")) then
          r5 <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_46 : process (clk, rst)
  begin
    if (rst) then
      r6 <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"6")) then
          r6 <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"6")) then
          r6 <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"6")) then
          r6 <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_47 : process (clk, rst)
  begin
    if (rst) then
      r7 <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"7")) then
          r7 <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"7")) then
          r7 <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"7")) then
          r7 <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_48 : process (clk, rst)
  begin
    if (rst) then
      r8_fiq <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"8") and (ldm_usr = '0' and (cpsr_m = "10001"))) then
          r8_fiq <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"8") and (cpsr_m = "10001")) then
          r8_fiq <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"8") and (cpsr_m = "10001")) then
          r8_fiq <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_49 : process (clk, rst)
  begin
    if (rst) then
      r8_usr <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"8") and (ldm_usr = '1' or (cpsr_m /= "10001"))) then
          r8_usr <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"8") and (cpsr_m /= "10001")) then
          r8_usr <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"8") and (cpsr_m /= "10001")) then
          r8_usr <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_50 : process (clk, rst)
  begin
    if (rst) then
      r9_fiq <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"9") and (ldm_usr = '0' and (cpsr_m = "10001"))) then
          r9_fiq <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"9") and (cpsr_m = "10001")) then
          r9_fiq <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"9") and (cpsr_m = "10001")) then
          r9_fiq <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_51 : process (clk, rst)
  begin
    if (rst) then
      r9_usr <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"9") and (ldm_usr = '1' or (cpsr_m /= "10001"))) then
          r9_usr <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"9") and (cpsr_m /= "10001")) then
          r9_usr <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"9") and (cpsr_m /= "10001")) then
          r9_usr <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_52 : process (clk, rst)
  begin
    if (rst) then
      ra_fiq <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"a") and (ldm_usr = '0' and (cpsr_m = "10001"))) then
          ra_fiq <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"a") and (cpsr_m = "10001")) then
          ra_fiq <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"a") and (cpsr_m = "10001")) then
          ra_fiq <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_53 : process (clk, rst)
  begin
    if (rst) then
      ra_usr <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"a") and (ldm_usr = '1' or (cpsr_m /= "10001"))) then
          ra_usr <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"a") and (cpsr_m /= "10001")) then
          ra_usr <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"a") and (cpsr_m /= "10001")) then
          ra_usr <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_54 : process (all)
  begin
    if (cmd_is_ldr0 or cmd_is_ldr1 or cmd_is_swp or cmd_is_swpx) then
      -- ram_flag <= ('1' sll cmd_addr(1 downto 0)) when cmd(22) else 
      --            "1111";
      ram_flag <= std_logic_vector(shift_left(to_unsigned(1, 4), to_integer(unsigned(cmd_addr(1 downto 0))))) when cmd(22) else
                  "1111";
    elsif (cmd_is_ldrh0 or cmd_is_ldrh1 or cmd_is_ldrsh0 or cmd_is_ldrsh1) then
      ram_flag <= "1100" when cmd_addr(1) else 
                  "0011";
    elsif (cmd_is_ldrsb0 or cmd_is_ldrsb1) then
      -- ram_flag <= '1' sll cmd_addr(1 downto 0);
      ram_flag <= std_logic_vector(shift_left(to_unsigned(1, 4), to_integer(unsigned(cmd_addr(1 downto 0)))));
    else
      ram_flag <= "1111";
    end if;
  end process;
  processing_55 : process (all)
  begin
    if (cmd_is_ldm) then
      if (cmd(0)) then
        ram_wdata <= r0;
      elsif (cmd(1)) then
        ram_wdata <= r1;
      elsif (cmd(2)) then
        ram_wdata <= r2;
      elsif (cmd(3)) then
        ram_wdata <= r3;
      elsif (cmd(4)) then
        ram_wdata <= r4;
      elsif (cmd(5)) then
        ram_wdata <= r5;
      elsif (cmd(6)) then
        ram_wdata <= r6;
      elsif (cmd(7)) then
        ram_wdata <= r7;
      elsif (cmd(8)) then
        ram_wdata <= r8_usr
        when cmd(22) else r8;
      elsif (cmd(9)) then
        ram_wdata <= r9_usr
        when cmd(22) else r9;
      elsif (cmd(10)) then
        ram_wdata <= ra_usr
        when cmd(22) else ra;
      elsif (cmd(11)) then
        ram_wdata <= rb_usr
        when cmd(22) else rb;
      elsif (cmd(12)) then
        ram_wdata <= rc_usr
        when cmd(22) else rc;
      elsif (cmd(13)) then
        ram_wdata <= rd_usr
        when cmd(22) else rd;
      elsif (cmd(14)) then
        ram_wdata <= re_usr
        when cmd(22) else re;
      elsif (cmd(15)) then
        ram_wdata <= rf;
      else
        ram_wdata <= 32X"0";
      end if;
    elsif (cmd_is_ldr0 or cmd_is_ldr1 or cmd_is_swpx) then
      if (cmd(22)) then
        ram_wdata <= (rna(7 downto 0) & rna(7 downto 0) & rna(7 downto 0) & rna(7 downto 0));
      else
        ram_wdata <= rna;
      end if;
    elsif (cmd_is_ldrh0 or cmd_is_ldrh1) then
      ram_wdata <= (rna(15 downto 0) & rna(15 downto 0));
    else
      ram_wdata <= rna;
    end if;
  end process;
  processing_56 : process (clk, rst)
  begin
    if (rst) then
      rb_fiq <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"b") and (ldm_usr = '0' and (cpsr_m = "10001"))) then
          rb_fiq <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"b") and (cpsr_m = "10001")) then
          rb_fiq <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"b") and (cpsr_m = "10001")) then
          rb_fiq <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_57 : process (clk, rst)
  begin
    if (rst) then
      rb_usr <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"b") and (ldm_usr = '1' or (cpsr_m /= "10001"))) then
          rb_usr <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"b") and (cpsr_m /= "10001")) then
          rb_usr <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"b") and (cpsr_m /= "10001")) then
          rb_usr <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_58 : process (clk, rst)
  begin
    if (rst) then
      rc_fiq <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"c") and (ldm_usr = '0' and (cpsr_m = "10001"))) then
          rc_fiq <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"c") and (cpsr_m = "10001")) then
          rc_fiq <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"c") and (cpsr_m = "10001")) then
          rc_fiq <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_59 : process (clk, rst)
  begin
    if (rst) then
      rc_usr <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"c") and (ldm_usr = '1' or (cpsr_m /= "10001"))) then
          rc_usr <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"c") and (cpsr_m /= "10001")) then
          rc_usr <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"c") and (cpsr_m /= "10001")) then
          rc_usr <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_60 : process (all)
  begin
    case ((cpsr_m)) is
    when "10001" =>
      rd <= rd_fiq;
    when "11011" =>
      rd <= rd_und;
    when "10010" =>
      rd <= rd_irq;
    when "10111" =>
      rd <= rd_abt;
    when "10011" =>
      rd <= rd_svc;
    when others =>
      rd <= rd_usr;
    end case;
  end process;


  processing_61 : process (clk, rst)
  begin
    if (rst) then
      rd_abt <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"d") and (ldm_usr = '0' and (cpsr_m = "10111"))) then
          rd_abt <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"d") and (cpsr_m = "10111")) then
          rd_abt <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"d") and (cpsr_m = "10111")) then
          rd_abt <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_62 : process (clk, rst)
  begin
    if (rst) then
      rd_fiq <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"d") and (ldm_usr = '0' and (cpsr_m = "10001"))) then
          rd_fiq <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"d") and (cpsr_m = "10001")) then
          rd_fiq <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"d") and (cpsr_m = "10001")) then
          rd_fiq <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_63 : process (clk, rst)
  begin
    if (rst) then
      rd_irq <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"d") and (ldm_usr = '0' and (cpsr_m = "10010"))) then
          rd_irq <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"d") and (cpsr_m = "10010")) then
          rd_irq <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"d") and (cpsr_m = "10010")) then
          rd_irq <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_64 : process (clk, rst)
  begin
    if (rst) then
      rd_svc <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"d") and (ldm_usr = '0' and (cpsr_m = "10011"))) then
          rd_svc <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"d") and (cpsr_m = "10011")) then
          rd_svc <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"d") and (cpsr_m = "10011")) then
          rd_svc <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_65 : process (clk, rst)
  begin
    if (rst) then
      rd_und <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"d") and (ldm_usr = '0' and (cpsr_m = "11011"))) then
          rd_und <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"d") and (cpsr_m = "11011")) then
          rd_und <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"d") and (cpsr_m = "11011")) then
          rd_und <= to_data;
        else
          null;
        end if;
      else
        null;
      end if;
    end if;
  end process;
  processing_66 : process (clk, rst)
  begin
    if (rst) then
      rd_usr <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"d") and (ldm_usr = '1' or ((cpsr_m /= "10001") and (cpsr_m /= "11011") and (cpsr_m /= "10010") and (cpsr_m /= "10111") and (cpsr_m /= "10011")))) then
          rd_usr <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"d") and ((cpsr_m /= "10001") and (cpsr_m /= "11011") and (cpsr_m /= "10010") and (cpsr_m /= "10111") and (cpsr_m /= "10011"))) then
          rd_usr <= go_data;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"d") and ((cpsr_m /= "10001") and (cpsr_m /= "11011") and (cpsr_m /= "10010") and (cpsr_m /= "10111") and (cpsr_m /= "10011"))) then
          rd_usr <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;

  processing_67 : process (all)
  begin
    case ((cpsr_m)) is
    when "10001" =>
      re <= re_fiq;
    when "11011" =>
      re <= re_und;
    when "10010" =>
      re <= re_irq;
    when "10111" =>
      re <= re_abt;
    when "10011" =>
      re <= re_svc;
    when others =>
      re <= re_usr;
    end case;
  end process;


  processing_68 : process (clk, rst)
  begin
    if (rst) then
      re_abt <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ram_abort or (not fiq_en and not irq_en and (cmd_flag and code_abort))) then
          re_abt <= rf_b;
        elsif (ldm_vld = '1' and (ldm_num = X"e") and (ldm_usr = '0' and (cpsr_m = "10111"))) then
          re_abt <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"e") and (cpsr_m = "10111")) then
          re_abt <= go_data;
        elsif (cmd_ok = '1' and cmd_is_b = '1' and cmd(24) = '1' and (cpsr_m = "10111")) then
          re_abt <= rf_b;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"e") and (cpsr_m = "10111")) then
          re_abt <= to_data;
        else
          null;
        end if;
      else
        null;
      end if;
    end if;
  end process;
  processing_69 : process (clk, rst)
  begin
    if (rst) then
      re_fiq <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (fiq_en) then
          if (ram_abort) then
            re_fiq <= 32X"10";
          else
            re_fiq <= rf_b;
          end if;
        elsif (ldm_vld = '1' and (ldm_num = X"e") and (ldm_usr = '0' and (cpsr_m = "10001"))) then
          re_fiq <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"e") and (cpsr_m = "10001")) then
          re_fiq <= go_data;
        elsif (cmd_ok = '1' and cmd_is_b = '1' and cmd(24) = '1' and (cpsr_m = "10001")) then
          re_fiq <= rf_b;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"e") and (cpsr_m = "10001")) then
          re_fiq <= to_data;
        else
          null;
        end if;
      else
        null;
      end if;
    end if;
  end process;
  processing_70 : process (clk, rst)
  begin
    if (rst) then
      re_irq <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (not ram_abort and not fiq_en and irq_en) then
          re_irq <= rf_b;
        elsif (ldm_vld = '1' and (ldm_num = X"e") and (ldm_usr = '0' and (cpsr_m = "10010"))) then
          re_irq <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"e") and (cpsr_m = "10010")) then
          re_irq <= go_data;
        elsif (cmd_ok = '1' and cmd_is_b = '1' and cmd(24) = '1' and (cpsr_m = "10010")) then
          re_irq <= rf_b;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"e") and (cpsr_m = "10010")) then
          re_irq <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_71 : process (clk, rst)
  begin
    if (rst) then
      re_svc <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (not ram_abort and not fiq_en and not irq_en and (cmd_flag and not code_abort and not code_und and (cond_satisfy and cmd_is_swi))) then
          re_svc <= rf_b;
        elsif (ldm_vld = '1' and (ldm_num = X"e") and (ldm_usr = '0' and (cpsr_m = "10011"))) then
          re_svc <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"e") and (cpsr_m = "10011")) then
          re_svc <= go_data;
        elsif (cmd_ok = '1' and cmd_is_b = '1' and cmd(24) = '1' and (cpsr_m = "10011")) then
          re_svc <= rf_b;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"e") and (cpsr_m = "10011")) then
          re_svc <= to_data;
        else
          null;
        end if;
      else
        null;
      end if;
    end if;
  end process;
  processing_72 : process (clk, rst)
  begin
    if (rst) then
      re_und <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (not ram_abort and not fiq_en and not irq_en and (cmd_flag and not code_abort and code_und)) then
          re_und <= rf_b;
        elsif (ldm_vld = '1' and (ldm_num = X"e") and (ldm_usr ='0' and (cpsr_m = "11011"))) then
          re_und <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"e") and (cpsr_m = "11011")) then
          re_und <= go_data;
        elsif (cmd_ok = '1' and cmd_is_b = '1' and cmd(24) = '1' and (cpsr_m = "11011")) then
          re_und <= rf_b;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"e") and (cpsr_m = "11011")) then
          re_und <= to_data;
        else
          null;
        end if;
      else
        null;
      end if;
    end if;
  end process;
  processing_73 : process (clk, rst)
  begin
    if (rst) then
      re_usr <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ldm_vld = '1' and (ldm_num = X"e") and (ldm_usr = '1' or ((cpsr_m /= "10001") and (cpsr_m /= "11011") and (cpsr_m /= "10010") and (cpsr_m /= "10111") and (cpsr_m /= "10011")))) then
          re_usr <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"e") and ((cpsr_m /= "10001") and (cpsr_m /= "11011") and (cpsr_m /= "10010") and (cpsr_m /= "10111") and (cpsr_m /= "10011"))) then
          re_usr <= go_data;
        elsif (cmd_ok = '1' and cmd_is_b = '1' and cmd(24) = '1' and ((cpsr_m /= "10001") and (cpsr_m /= "11011") and (cpsr_m /= "10010") and (cpsr_m /= "10111") and (cpsr_m /= "10011"))) then
          re_usr <= rf_b;
        elsif (cmd_ok = '1' and to_vld = '1' and (to_num = X"e") and ((cpsr_m /= "10001") and (cpsr_m /= "11011") and (cpsr_m /= "10010") and (cpsr_m /= "10111") and (cpsr_m /= "10011"))) then
          re_usr <= to_data;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_74 : process (clk, rst)
  begin
    if (rst) then
      reg_ans <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (not hold_en) then
          reg_ans <= mult_ans;
        elsif (cmd_is_ldm) then
          if (cmd_sum_m = "00001") then
            reg_ans(6 downto 2) <= sum_m;
          elsif (cmd(23)) then
            reg_ans(6 downto 2) <= std_logic_vector(to_unsigned(to_integer(unsigned(reg_ans(6 downto 2))) + 1, 5));
          else
            reg_ans(6 downto 2) <= std_logic_vector(to_unsigned(to_integer(unsigned(reg_ans(6 downto 2))) - 1, 5));
          end if;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_75 : process (clk, rst)
  begin
    if (rst) then
      rf <= 32b"0";
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (cpu_restart) then
          rf <= X"0000_0000";
        elsif (fiq_en) then
          rf <= X"0000_001c";
        elsif (ram_abort) then
          rf <= X"0000_0010";
        elsif (irq_en) then
          rf <= X"0000_0018";
        elsif (cmd_flag and code_abort) then
          rf <= X"0000_000c";
        elsif (cmd_flag and code_und) then
          rf <= X"0000_0004";
        elsif (cmd_flag and cond_satisfy and cmd_is_swi) then
          rf <= X"0000_0008";
        elsif (ldm_vld = '1' and (ldm_num = X"f")) then
          rf <= ldm_data;
        elsif (go_vld = '1' and (go_num = X"f")) then
          rf <= go_data;
        elsif (cmd_ok = '1' and (cmd_is_dp0 = '1' or cmd_is_dp1 = '1' or cmd_is_dp2 = '1' ) and (cmd(24 downto 23) /= "10") and (cmd(15 downto 12) = X"f")) then
          rf <= dp_ans;
        elsif (cmd_ok and (cmd_is_b or cmd_is_bx)) then
          rf <= sum_rn_rm;
        elsif (not hold_en and not wait_en) then
          --rf <= std_logic_vector(to_unsigned(to_integer(unsigned(rf)) + 4, 32)); -- rf + 4;
          rf <= std_logic_vector(unsigned(rf) + x"00000004");
          --rf <= rf + 4;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_76 : process (clk, rst)
  begin
    if (rst) then
      rm_msb <= '0';
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (not hold_en) then
          rm_msb <= code_rma(31);
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_77 : process (all)
  begin
    if (cmd_is_bx or (cmd_is_multlx and not cmd(21))) then
      rn <= 32x"0";
    elsif (cmd_is_mult or cmd_is_multl) then
      if (cmd(21)) then
        rn <= rna;
      else
        rn <= 32x"0";
      end if;
    elsif (cmd_is_b) then
      rn <= rf;
    elsif (cmd_is_dp0 or cmd_is_dp1 or cmd_is_dp2) then
      if ((cmd(24 downto 21) = "1101") or (cmd(24 downto 21) = "1111")) then
        rn <= 32x"0";
      elsif ((cmd(24 downto 21) = "0011") or (cmd(24 downto 21) = "0111")) then
        rn <= not rnb;
      else
        rn <= rnb;
      end if;
    elsif (hold_en_dly) then
      rn <= rn_register;
    else

      rn <= rnb;
    end if;
  end process;

  processing_78 : process (clk, rst)
  begin
    if (rst) then
      rn_register <= 32b"0";
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (hold_en and not hold_en_dly) then
          rn_register <= rnb;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;

  processing_79 : process (all)
  begin
    case ((cmd(15 downto 12))) is
      when X"0" =>
        rna <= r0;
      when X"1" =>
        rna <= r1;
      when X"2" =>
        rna <= r2;
      when X"3" =>
        rna <= r3;
      when X"4" =>
        rna <= r4;
      when X"5" =>
        rna <= r5;
      when X"6" =>
        rna <= r6;
      when X"7" =>
        rna <= r7;
      when X"8" =>
        rna <= r8;
      when X"9" =>
        rna <= r9;
      when X"a" =>
        rna <= ra;
      when X"b" =>
        rna <= rb;
      when X"c" =>
        rna <= rc;
      when X"d" =>
        rna <= rd;
      when X"e" =>
        rna <= re;
      when X"f" =>
        rna <= rf;
      when others =>
        rna <= (others => '0');
    end case;
  end process;


  processing_80 : process (all)
  begin
    case ((cmd(19 downto 16))) is
    when X"0" =>
      rnb <= r0;
    when X"1" =>
      rnb <= r1;
    when X"2" =>
      rnb <= r2;
    when X"3" =>
      rnb <= r3;
    when X"4" =>
      rnb <= r4;
    when X"5" =>
      rnb <= r5;
    when X"6" =>
      rnb <= r6;
    when X"7" =>
      rnb <= r7;
    when X"8" =>
      rnb <= r8;
    when X"9" =>
      rnb <= r9;
    when X"a" =>
      rnb <= ra;
    when X"b" =>
      rnb <= rb;
    when X"c" =>
      rnb <= rc;
    when X"d" =>
      rnb <= rd;
    when X"e" =>
      rnb <= re;
    when X"f" =>
      rnb <= rf;
    when others =>
      rnb <= (others => '0');
    end case;
  end process;


  processing_81 : process (clk, rst)
  begin
    if (rst) then
      rs_msb <= '0';
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (not hold_en) then
          rs_msb <= code_rsa(31);
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_82 : process (all)
  begin
    if (cmd_is_dp0 or cmd_is_ldr1) then
      case ((cmd(6 downto 5))) is
      when "00" =>
        sec_operand <= reg_ans(31 downto 0);
      when "01" =>
        sec_operand <= reg_ans(63 downto 32);
      when "10" =>
        sec_operand <= not reg_ans(63 downto 32) when rm_msb else
                       reg_ans(63 downto 32);
      when "11" =>
        sec_operand <= cpsr_c & reg_ans(31 downto 1) when (cmd(11 downto 7) = "00000") else
                       reg_ans(63 downto 32) or reg_ans(31 downto 0);
      when others =>
        sec_operand <= (others => '0');
      end case;
    elsif (cmd_is_dp1) then
      case ((cmd(6 downto 5))) is
      when "00" =>
        sec_operand <= (others => '0')
        when (code_rs_flag(2 downto 1) /= "00") else reg_ans(31 downto 0);
      when "01" =>
        sec_operand <= (others => '0')
        when (code_rs_flag(2 downto 1) /= "00") else reg_ans(31 downto 0)
        when code_rs_flag(0) else reg_ans(63 downto 32);
      when "10" =>
        sec_operand <= concatenate(32, rm_msb)
        when (code_rs_flag(2 downto 1) /= "00") else not reg_ans(31 downto 0)
        when rm_msb else reg_ans(31 downto 0)
        when code_rs_flag(0) else not reg_ans(63 downto 32)
        when rm_msb else reg_ans(63 downto 32);
      when "11" =>
        sec_operand <= reg_ans(31 downto 0)
        when (code_rs_flag(1) or code_rs_flag(0)) else (reg_ans(63 downto 32) or reg_ans(31 downto 0));
      when others => sec_operand <= (others => '0');
      end case;
    elsif (cmd_is_msr1 or cmd_is_dp2) then
      sec_operand <= reg_ans(63 downto 32) or reg_ans(31 downto 0);
    elsif (cmd_is_multlx) then
      sec_operand <= reg_ans(63 downto 32);
    else
      sec_operand <= reg_ans(31 downto 0);
    end if;
  end process;
  processing_83 : process (all)
  begin
    if (cpsr_m = "10011") then
      spsr <= spsr_svc;
    elsif (cpsr_m = "10111") then
      spsr <= spsr_abt;
    elsif (cpsr_m = "10010") then
      spsr <= spsr_irq;
    elsif (cpsr_m = "10001") then
      spsr <= spsr_fiq;
    elsif (cpsr_m = "11011") then
      spsr <= spsr_und;
    else
      spsr <= cpsr;
    end if;
  end process;
  processing_84 : process (clk, rst)
  begin
    if (rst) then
      spsr_abt <= 11b"0";
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (ram_abort or (not fiq_en and not irq_en and (cmd_flag and code_abort))) then
          spsr_abt <= cpsr;
        elsif (cmd_ok = '1' and (cpsr_m = "10111") and (cmd_is_msr0 = '1' or cmd_is_msr1 = '1') and cmd(22) = '1') then
          -- spsr_abt <= (sec_operand(31 downto 28) when cmd(19) else spsr_abt(10 downto 7)) & 
          -- ((sec_operand(7 downto 6) & sec_operand(4 downto 0)) when cmd(16) else spsr_abt(6 downto 0));
          spsr_abt <= get_spsr(cmd, sec_operand, spsr_abt);
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_85 : process (clk, rst)
  begin
    if (rst) then
      spsr_fiq <= 11b"0";
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (fiq_en) then
          if (ram_abort) then
            spsr_fiq <= (cpsr_n & cpsr_z & cpsr_c & cpsr_v & '1' & cpsr_f & "10111");
          else
            spsr_fiq <= cpsr;
          end if;
        elsif (cmd_ok = '1' and (cpsr_m = "11011") and (cmd_is_msr0 = '1' or cmd_is_msr1= '1' ) and cmd(22) = '1') then
          -- spsr_fiq <= ((sec_operand(31 downto 28)
          -- when cmd(19) else spsr_fiq(10 downto 7)) & ((sec_operand(7 downto 6) & sec_operand(4 downto 0))
          -- when cmd(16) else spsr_fiq(6 downto 0)));
          spsr_fiq <= get_spsr(cmd, sec_operand, spsr_fiq);
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_86 : process (clk, rst)
  begin
    if (rst) then
      spsr_irq <= 11b"0";
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (not ram_abort and not fiq_en and irq_en) then
          spsr_irq <= cpsr;
        elsif (cmd_ok = '1' and (cpsr_m = "10010") and (cmd_is_msr0 = '1' or cmd_is_msr1 = '1') and cmd(22) = '1') then
          -- spsr_irq <= ((sec_operand(31 downto 28)
          -- when cmd(19) else spsr_irq(10 downto 7)) & ((sec_operand(7 downto 6) & sec_operand(4 downto 0))
          -- when cmd(16) else spsr_irq(6 downto 0)));
          spsr_irq <= get_spsr(cmd, sec_operand, spsr_irq);
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_87 : process (clk, rst)
  begin
    if (rst) then
      spsr_svc <= 11b"0";
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (not ram_abort and not fiq_en and not irq_en and (cmd_flag and not code_abort and not code_und and (cond_satisfy and cmd_is_swi))) then
          spsr_svc <= cpsr;
        elsif (cmd_ok = '1' and (cpsr_m = "10011") and (cmd_is_msr0 = '1' or cmd_is_msr1 = '1') and cmd(22) = '1') then
          -- spsr_svc <= ((sec_operand(31 downto 28)
          -- when cmd(19) else spsr_svc(10 downto 7)) & ((sec_operand(7 downto 6) & sec_operand(4 downto 0))
          -- when cmd(16) else spsr_svc(6 downto 0)));
          spsr_svc <= get_spsr(cmd, sec_operand, spsr_svc);
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_88 : process (clk, rst)
  begin
    if (rst) then
      spsr_und <= (others => '0');
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (not ram_abort and not fiq_en and not irq_en and (cmd_flag and not code_abort and code_und)) then
          spsr_und <= cpsr;
        elsif (cmd_ok = '1' and (cpsr_m = "11011") and (cmd_is_msr0 = '1' or cmd_is_msr1 = '1') and cmd(22) = '1') then
          -- spsr_und <= ((sec_operand(31 downto 28)
          -- when cmd(19) else spsr_und(10 downto 7)) & ((sec_operand(7 downto 6) & sec_operand(4 downto 0))
          -- when cmd(16) else spsr_und(6 downto 0)));
          spsr_und <= get_spsr(cmd, sec_operand, spsr_und);
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;
  processing_89 : process (clk, rst)
  begin
    if (rst) then
      sum_m <= "00000";
    elsif (rising_edge(clk)) then
      if (cpu_en) then
        if (not hold_en) then
          sum_m <= code_sum_m;
        else
          null;
        end if;
      else

        null;
      end if;
    end if;
  end process;

  processing_90 : process (all)
  begin
    if (cmd_is_mrs) then
      to_data <= (spsr(10 downto 7) & 20b"0" & spsr(6 downto 5) & '0' & spsr(4 downto 0)) when cmd(22) else 
                 (cpsr(10 downto 7) & 20b"0" & cpsr(6 downto 5) & '0' & cpsr(4 downto 0));
    elsif (cmd_is_dp0 or cmd_is_dp1 or cmd_is_dp2) then
      to_data <= dp_ans;
    else
      to_data <= sum_rn_rm;
    end if;
  end process;

  processing_91 : process (all)
  begin
    if (cmd_is_mrs or (cmd_is_dp0 or cmd_is_dp1 or cmd_is_dp2) or cmd_is_multl) then
      to_num <= cmd(15 downto 12);
    else
      to_num <= cmd(19 downto 16);
    end if;
  end process;

end RTL;
