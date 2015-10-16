/*
* pmu.h - OWL PMIC driver
*
* Copyright (C) 2012, Actions Semiconductor Co. LTD.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published
* by the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*/
#ifndef __ASM_ARM_ARCH_PMU_H__
#define __ASM_ARM_ARCH_PMU_H__

//#include <afinfo.h>
#include <asm/arch/regs_map_atc2603a.h>
#include <asm/arch/regs_map_atc2603c.h>
#include <asm/arch/regs_map_atc2609a.h>
#include <i2c.h>
extern struct spi_slave g_atc260x_spi_slave;


typedef struct
{
    /* offs: 0x0 */
	unsigned char shift;
    /* offs: 0x1 */
	unsigned char mask;
    /* offs: 0x2 */
	unsigned char val;
    /* offs: 0x3 */
	unsigned char no;
} __attribute__ ((packed)) mfp_t;

typedef struct
{
    /* offs: 0x0 */
	unsigned long pwm_val;
    /* offs: 0x4 */
	mfp_t	mfp;
} __attribute__ ((packed)) pwm_config_t;

struct owl_pmu_config {
		unsigned int	bus_id;
		unsigned int	dcdc_en_bm;
		unsigned int dcdc_cfgs[7];
		unsigned int	ldo_en_bm;
		unsigned int ldo_cfgs[12];
		unsigned int sgpio_out_en;
		unsigned int sgpio_in_en;
		unsigned int sgpio_out;
		pwm_config_t  pwm_config[3];
		int i2c_config;
		//cpu_pwm_volt_table  cpu_pwm_volt_tb[];
		//int ic_type;
};

/* PMU ���� */
#define OWL_PMU_ID_ATC2603A    0
#define OWL_PMU_ID_ATC2603B    1
#define OWL_PMU_ID_ATC2603C    2
#define OWL_PMU_ID_CNT		3

/* ���ݰ������õó�PMU���͵Ķ���, ��������ͳһʹ�ú�OWL_PMU_ID */
#if defined(CONFIG_ATC2603A)
#define OWL_PMU_ID   OWL_PMU_ID_ATC2603A
#elif defined(CONFIG_ATC2603C)
#define OWL_PMU_ID   OWL_PMU_ID_ATC2603C
#elif defined(CONFIG_ATC2603B)
#define OWL_PMU_ID   OWL_PMU_ID_ATC2603B
#else
/* ԭ�����afinfo�Ǳ��õ�, ����������ʹSPL�����, �ʻ����ȶ���. */
#define OWL_PMU_ID   OWL_PMU_ID_ATC2603C /*(afinfo->pmu_id)*/
#endif


extern int pmu_init(const void *blob);
extern void pmu_prepare_for_s2(void);
extern void vdd_cpu_voltage_scan(void);
extern void vdd_cpu_voltage_store(void);
extern void set_ddr_voltage(int add_flag);

extern int atc260x_reg_read(unsigned short reg);
extern int atc260x_reg_write(unsigned short reg , unsigned short value);
extern int atc260x_set_bits(unsigned int reg, unsigned short mask, unsigned short val);
extern int atc260x_get_version(void);

/* for persistent storage */
/* ͳһ����260x�м���������/����λ��FW�üĴ���, ����������ɷ����������. */
/* ��ͬ��PMIC�и�����ķ���λ���ǲ�ͬ��, ����ͳһʹ�����׽ӿ�, ���Դ�����������Ҫ���鷳. */
enum {
	ATC260X_PSTORE_TAG_REBOOT_ADFU = 0,     /* ������ADFU��־. */
	ATC260X_PSTORE_TAG_REBOOT_RECOVERY,     /* ������recovery��־. */
	ATC260X_PSTORE_TAG_FW_S2,               /* ���S2��־ */
	ATC260X_PSTORE_TAG_DIS_MCHRG,           /* ��������mini_charger */
	ATC260X_PSTORE_TAG_RTC_MSALM,           /* RTC Alarm ������, reboot/suspend���õ�. */
	ATC260X_PSTORE_TAG_RTC_HALM,            /* RTC Alarm ������ */
	ATC260X_PSTORE_TAG_RTC_YMDALM,          /* RTC Alarm ������ */
	ATC260X_PSTORE_TAG_GAUGE_CAP,           /* ����������Ƶ��� (8bits) */
	ATC260X_PSTORE_TAG_GAUGE_BAT_RES,       /* ���������������ĵ������ (16bits) */
	ATC260X_PSTORE_TAG_GAUGE_ICM_EXIST,     /* �������Ƿ�ʹ��ICM���� (1bit) */
	ATC260X_PSTORE_TAG_GAUGE_SHDWN_TIME,    /* ����ػ�ʱ��,�������ڲ�ʹ�� (31bits) */
	ATC260X_PSTORE_TAG_GAUGE_S2_CONSUMP,    /* ��������, ��¼S2�ڼ�Ĺ���. (6bits) */
	ATC260X_PSTORE_TAG_GAUGE_CLMT_RESET,    /* ��������, coulomb_meter��λ��� (1bit) */
	ATC260X_PSTORE_TAG_RESUME_ADDR,         /* S2 resume address (low 32bit) */
	ATC260X_PSTORE_TAG_NUM
};
extern int atc260x_pstore_set(uint tag, u32 value);
extern int atc260x_pstore_get(uint tag, u32 *p_value);
extern ulong atc260x_pstore_get_noerr(uint tag);

extern int s2_resume;
extern void (*cpu_resume_fn)(void);
extern int alarm_wakeup;

#endif
