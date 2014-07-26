/*
 * (C) Copyright 2011
 *
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef _MACH_FMC_H_
#define _MACH_FMC_H_


/**
  * FMC SDRAM Bank address
  */
#define FMC_SDRAM_BANK_ADDR     ((uint32_t)0xC0000000)

/**
  * FMC SDRAM Size
  */
#define FMC_SDRAM_BANK_SIZE     ((uint32_t)0x00800000)

/* SDRAM Configuration */
#define SDRAM_DIV       (2)
#define SDRAM_CAS       (3)
#define SDRAM_NB        (1)     /* 4 banks */
#define SDRAM_MWID      (0)     /* 8 bit memory */
#define SDRAM_NR        (0x1)   /* 12-bit row */
#define SDRAM_NC        (0x2)   /* 10-bit col */
#define SDRAM_RPIPE     (1)
#define SDRAM_RBURST    (0)

/* SDRAM Timing */
#define SDRAM_TXSR      (7 - 1)
#define SDRAM_TMRD      (2 - 1)
#define SDRAM_TRRD      (2 - 1)
#define SDRAM_TRCD      (3 - 1)
#define SDRAM_TRP       (3 - 1)
#define SDRAM_TRAS      (7 - 1)
#define SDRAM_TRC       (10- 1)
#define SDRAM_TREF      (1386)
#define SDRAM_TWR       (2 - 1)
#define SDRAM_NRFS      (8 - 1)

#define SDRAM_MODE_BL           (0)
#define SDRAM_MODE_CAS          (SDRAM_CAS)
#define SDRAM_MODE_BL_SHIFT     (0)
#define SDRAM_MODE_CAS_SHIFT    (4)

/* Control register SDCR */
#define FMC_SDCR_WP_SHIFT       (9)     /* Write protection shift */
#define FMC_SDCR_CAS_SHIFT      (7)     /* CAS latency shift */
#define FMC_SDCR_NB_SHIFT       (6)     /* Number of banks shift */
#define FMC_SDCR_MWID_SHIFT     (4)     /* Memory width shift */
#define FMC_SDCR_NR_SHIFT       (2)     /* Number of row address bits shift */
#define FMC_SDCR_NC_SHIFT       (0)     /* Number of col address bits shift */
#define FMC_SDCR_SDCLK_SHIFT    (10)    /* SDRAM clock divisor shift */
#define FMC_SDCR_RPIPE_SHIFT    (13)    /* RPIPE bit shift */
#define FMC_SDCR_RBURST_SHIFT   (12)    /* RBURST bit shift */
#define FMC_SDCMR_NRFS_SHIFT    (5)
#define FMC_SDCMR_MODE_REGISTER_SHIFT   (9)


/* Timings register SDTR */
#define FMC_SDTR_TMRD_SHIFT     (0 )    /* Load mode register to active */
#define FMC_SDTR_TXSR_SHIFT     (4 )    /* Exit self-refresh time */
#define FMC_SDTR_TRAS_SHIFT     (8 )    /* Self-refresh time */
#define FMC_SDTR_TRC_SHIFT      (12)    /* Row cycle delay */
#define FMC_SDTR_TWR_SHIFT      (16)    /* Recovery delay */
#define FMC_SDTR_TRP_SHIFT      (20)    /* Row precharge delay */
#define FMC_SDTR_TRCD_SHIFT     (24)    /* Row-to-column delay */

#define FMC_SDCMR_BANK_1            (1 << 4)
#define FMC_SDCMR_BANK_2            (1 << 3)
#define FMC_SDCMR_MODE_NORMAL       (0)
#define FMC_SDCMR_MODE_START_CLOCK  (1)
#define FMC_SDCMR_MODE_PRECHARGE    (2)
#define FMC_SDCMR_MODE_AUTOREFRESH  (3)
#define FMC_SDCMR_MODE_WRITE_MODE   (4)
#define FMC_SDCMR_MODE_SELFREFRESH  (5)
#define FMC_SDCMR_MODE_POWERDOWN    (6)

#define FMC_SDSR_BUSY       (1 << 5)
#define FMC_BUSY_WAIT() do { \
    __asm__ __volatile__ ("dsb" : : : "memory"); \
    while(STM32_FMC_DRAM->sdsr & FMC_SDSR_BUSY); \
} while(0);

/**
 * Initialize the specified SDRAM controller
 * @returns             0 on success, < 0 on failure
 */
int fmc_dram_init(void);

#endif /* _MACH_FMC_H_ */
