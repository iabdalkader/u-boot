/*
 * (C) Copyright 2010,2011
 * Dmitry Konyshev, Emcraft Systems, probables@emcraft.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <common.h>
#include <asm/arch/stm32.h>
#include <asm/arch/fmc.h>
#include <asm/arch/stm32f2_gpio.h>

/*
 *********************************************************************************************************
 *                                             EXTERNAL FUNCTIONS
 *********************************************************************************************************
 */

void udelay(unsigned long usec);

/* SDRAM pins assignment
   +-------------------+--------------------+--------------------+
   | FMC_D0 <-> PD14    | FMC_A0  <-> PF0   | FMC_CLK  <-> PG8  |
   | FMC_D1 <-> PD15    | FMC_A1  <-> PF1   | FMC_CLKE <-> PC3  |
   | FMC_D2 <-> PD0     | FMC_A2  <-> PF2   | FMC_WE   <-> PC0  |
   | FMC_D3 <-> PD1     | FMC_A3  <-> PF3   | FMC_CS   <-> PC2  |
   | FMC_D4 <-> PE7     | FMC_A4  <-> PF4   | FMC_DQM  <-> PE0  |
   | FMC_D5 <-> PE8     | FMC_A5  <-> PF5   | FMC_BA0  <-> PG4  |
   | FMC_D6 <-> PE9     | FMC_A6  <-> PF12  | FMC_BA1  <-> PG5  |
   | FMC_D7 <-> PE10    | FMC_A7  <-> PF13  | FMC_RAS  <-> PF11 |
   |                    | FMC_A8  <-> PF14  | FMC_CAS  <-> PG15 |
   |                    | FMC_A9  <-> PF15  |                   |
   |                    | FMC_A10 <-> PG0   |                   |
   |                    | FMC_A11 <-> PG1   |                   |
   +-------------------+--------------------+--------------------+ */

static const struct stm32f2_gpio_dsc ext_ram_fmc_gpio[] = {
    {STM32F2_GPIO_PORT_D, STM32F2_GPIO_PIN_14, STM32F2_GPIO_ROLE_FSMC},  /* FMC_D0   */
    {STM32F2_GPIO_PORT_D, STM32F2_GPIO_PIN_15, STM32F2_GPIO_ROLE_FSMC},  /* FMC_D1   */
    {STM32F2_GPIO_PORT_D, STM32F2_GPIO_PIN_0,  STM32F2_GPIO_ROLE_FSMC},  /* FMC_D2   */
    {STM32F2_GPIO_PORT_D, STM32F2_GPIO_PIN_1,  STM32F2_GPIO_ROLE_FSMC},  /* FMC_D3   */
    {STM32F2_GPIO_PORT_E, STM32F2_GPIO_PIN_7,  STM32F2_GPIO_ROLE_FSMC},  /* FMC_D4   */
    {STM32F2_GPIO_PORT_E, STM32F2_GPIO_PIN_8,  STM32F2_GPIO_ROLE_FSMC},  /* FMC_D5   */
    {STM32F2_GPIO_PORT_E, STM32F2_GPIO_PIN_9,  STM32F2_GPIO_ROLE_FSMC},  /* FMC_D6   */
    {STM32F2_GPIO_PORT_E, STM32F2_GPIO_PIN_10, STM32F2_GPIO_ROLE_FSMC},  /* FMC_D7   */

    {STM32F2_GPIO_PORT_F, STM32F2_GPIO_PIN_0,  STM32F2_GPIO_ROLE_FSMC},  /* FMC_A0   */
    {STM32F2_GPIO_PORT_F, STM32F2_GPIO_PIN_1,  STM32F2_GPIO_ROLE_FSMC},  /* FMC_A1   */
    {STM32F2_GPIO_PORT_F, STM32F2_GPIO_PIN_2,  STM32F2_GPIO_ROLE_FSMC},  /* FMC_A2   */
    {STM32F2_GPIO_PORT_F, STM32F2_GPIO_PIN_3,  STM32F2_GPIO_ROLE_FSMC},  /* FMC_A3   */
    {STM32F2_GPIO_PORT_F, STM32F2_GPIO_PIN_4,  STM32F2_GPIO_ROLE_FSMC},  /* FMC_A4   */
    {STM32F2_GPIO_PORT_F, STM32F2_GPIO_PIN_5,  STM32F2_GPIO_ROLE_FSMC},  /* FMC_A5   */
    {STM32F2_GPIO_PORT_F, STM32F2_GPIO_PIN_12, STM32F2_GPIO_ROLE_FSMC},  /* FMC_A6   */
    {STM32F2_GPIO_PORT_F, STM32F2_GPIO_PIN_13, STM32F2_GPIO_ROLE_FSMC},  /* FMC_A7   */
    {STM32F2_GPIO_PORT_F, STM32F2_GPIO_PIN_14, STM32F2_GPIO_ROLE_FSMC},  /* FMC_A8   */
    {STM32F2_GPIO_PORT_F, STM32F2_GPIO_PIN_15, STM32F2_GPIO_ROLE_FSMC},  /* FMC_A9   */
    {STM32F2_GPIO_PORT_G, STM32F2_GPIO_PIN_0,  STM32F2_GPIO_ROLE_FSMC},  /* FMC_A10  */
    {STM32F2_GPIO_PORT_G, STM32F2_GPIO_PIN_1,  STM32F2_GPIO_ROLE_FSMC},  /* FMC_A11  */

    {STM32F2_GPIO_PORT_G, STM32F2_GPIO_PIN_8,  STM32F2_GPIO_ROLE_FSMC},  /* FMC_CLK  */
    {STM32F2_GPIO_PORT_C, STM32F2_GPIO_PIN_3,  STM32F2_GPIO_ROLE_FSMC},  /* FMC_CKE  */
    {STM32F2_GPIO_PORT_C, STM32F2_GPIO_PIN_0,  STM32F2_GPIO_ROLE_FSMC},  /* FMC_WE   */
    {STM32F2_GPIO_PORT_C, STM32F2_GPIO_PIN_2,  STM32F2_GPIO_ROLE_FSMC},  /* FMC_CS   */
    {STM32F2_GPIO_PORT_E, STM32F2_GPIO_PIN_0,  STM32F2_GPIO_ROLE_FSMC},  /* FMC_DQM  */
    {STM32F2_GPIO_PORT_G, STM32F2_GPIO_PIN_4,  STM32F2_GPIO_ROLE_FSMC},  /* FMC_BA0  */
    {STM32F2_GPIO_PORT_G, STM32F2_GPIO_PIN_5,  STM32F2_GPIO_ROLE_FSMC},  /* FMC_BA1  */
    {STM32F2_GPIO_PORT_F, STM32F2_GPIO_PIN_11, STM32F2_GPIO_ROLE_FSMC},  /* FMC_RAS  */
    {STM32F2_GPIO_PORT_G, STM32F2_GPIO_PIN_15, STM32F2_GPIO_ROLE_FSMC},  /* FMC_CAS  */
};

/**
 * @brief  Setup the external memory controller.
 *         Called in startup_stm32f429_439xx.s before jump to main.
 *         This function configures the external SDRAM mounted on STM32F429I DISCO board
 *         This SDRAM will be used as program data memory (including heap and stack).
 * @param  None
 * @retval None
 */
static void ExtMemInit(void)
{
    // FMC Configuration
    // Enable the FMC interface clock
    STM32_RCC->ahb3enr |= 0x00000001;

    // SDRAM bank control register configuration
    STM32_FMC_DRAM->sdcr[0] = (
            SDRAM_DIV    << FMC_SDCR_SDCLK_SHIFT |
            SDRAM_CAS    << FMC_SDCR_CAS_SHIFT   |
            SDRAM_NB     << FMC_SDCR_NB_SHIFT    |
            SDRAM_MWID   << FMC_SDCR_MWID_SHIFT  |
            SDRAM_NR     << FMC_SDCR_NR_SHIFT    |
            SDRAM_NC     << FMC_SDCR_NC_SHIFT    |
            SDRAM_RPIPE  << FMC_SDCR_RPIPE_SHIFT |
            SDRAM_RBURST << FMC_SDCR_RBURST_SHIFT
            );

    // SDRAM bank timing register configuration
    STM32_FMC_DRAM->sdtr[0] = (
            SDRAM_TRCD  << FMC_SDTR_TRCD_SHIFT  |
            SDRAM_TRP   << FMC_SDTR_TRP_SHIFT   |
            SDRAM_TWR   << FMC_SDTR_TWR_SHIFT   |
            SDRAM_TRC   << FMC_SDTR_TRC_SHIFT   |
            SDRAM_TRAS  << FMC_SDTR_TRAS_SHIFT  |
            SDRAM_TXSR  << FMC_SDTR_TXSR_SHIFT  |
            SDRAM_TMRD  << FMC_SDTR_TMRD_SHIFT
            );

    // Start Clock
    STM32_FMC_DRAM->sdcmr = FMC_SDCMR_BANK_1 | FMC_SDCMR_MODE_START_CLOCK;

    // PALL configuration
    udelay(200);/* Power-Up delay */
    FMC_BUSY_WAIT();
    STM32_FMC_DRAM->sdcmr = FMC_SDCMR_BANK_1 | FMC_SDCMR_MODE_PRECHARGE;

    // Auto refresh configuration
    udelay(100);
    FMC_BUSY_WAIT();
    STM32_FMC_DRAM->sdcmr = (
            FMC_SDCMR_BANK_1 |
            FMC_SDCMR_MODE_AUTOREFRESH |
            SDRAM_NRFS << FMC_SDCMR_NRFS_SHIFT
            );

    // Program external memory mode
    udelay(100);
    FMC_BUSY_WAIT();
    STM32_FMC_DRAM->sdcmr =
         FMC_SDCMR_BANK_1 | (
         SDRAM_MODE_BL << SDRAM_MODE_BL_SHIFT |
         SDRAM_MODE_CAS << SDRAM_MODE_CAS_SHIFT
        ) << FMC_SDCMR_MODE_REGISTER_SHIFT | FMC_SDCMR_MODE_WRITE_MODE;

    // Program external memory mode
    udelay(100);
    FMC_BUSY_WAIT();
    STM32_FMC_DRAM->sdcmr = FMC_SDCMR_BANK_1 | FMC_SDCMR_MODE_NORMAL;

    // Set refresh rate
    udelay(100);
    FMC_BUSY_WAIT();
    STM32_FMC_DRAM->sdrtr = SDRAM_TREF<<1;

}//ExtMemInit


int fmc_dram_init(void)
{
    static int common_init_done = 0;
    uint32_t *ptr, len;
    int rv = 0;

    if (!common_init_done) {
        int i;

        /*
         * Connect GPIOs to FMC controller
         */
        for (i = 0; i < ARRAY_SIZE(ext_ram_fmc_gpio); i++) {
            rv = stm32f2_gpio_config(&ext_ram_fmc_gpio[i]);
            if (rv != 0)
                return rv;
        }

        /*
         * Initialize FMC controller
         */
        ExtMemInit();

        common_init_done = 1;
    }

    return 0;
}
