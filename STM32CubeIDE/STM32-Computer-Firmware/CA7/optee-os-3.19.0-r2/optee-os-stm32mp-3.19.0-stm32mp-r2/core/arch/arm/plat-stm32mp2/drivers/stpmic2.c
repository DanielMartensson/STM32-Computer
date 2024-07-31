// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (c) 2022-2023, STMicroelectronics - All Rights Reserved
 */

#include <assert.h>
#include <drivers/stpmic2.h>
#include <kernel/panic.h>
#include <platform_config.h>
#include <stdint.h>
#include <string.h>
#include <trace.h>
#include <util.h>

#define VOLTAGE_INDEX_INVALID		((size_t)~(U(0)))

struct regul_struct {
	const char *name;
	const uint16_t *volt_table;
	uint8_t volt_table_size;
	uint8_t volt_cr;
	uint8_t volt_shift;
	uint8_t en_cr;
	uint8_t alt_en_cr;
	uint8_t alt_volt_cr;
	uint8_t pwrctrl_cr;
	uint8_t msrt_reg;
	uint8_t msrt_mask;
	uint8_t pd_reg;
	uint8_t pd_val;
	uint8_t pd_mask;
	uint8_t ocp_reg;
	uint8_t ocp_mask;
};

#define BUCK_VOLT_TABLE_SIZE 128
#define LDO235678_VOLT_TABLE_SIZE 32
#define LDO_SINGLE_VOLT_TABLE_SIZE 1

/* Voltage tables in mV */
static const uint16_t buck1236_volt_table[BUCK_VOLT_TABLE_SIZE] = {
	U(500), U(510), U(520), U(530), U(540),
	U(550), U(560), U(570), U(580), U(590),
	U(600), U(610), U(620), U(630), U(640),
	U(650), U(660), U(670), U(680), U(690),
	U(700), U(710), U(720), U(730), U(740),
	U(750), U(760), U(770), U(780), U(790),
	U(800), U(810), U(820), U(830), U(840),
	U(850), U(860), U(870), U(880), U(890),
	U(900), U(910), U(920), U(930), U(940),
	U(950), U(960), U(970), U(980), U(990),
	U(1000), U(1010), U(1020), U(1030), U(1040),
	U(1050), U(1060), U(1070), U(1080), U(1090),
	U(1100), U(1110), U(1120), U(1130), U(1140),
	U(1150), U(1160), U(1170), U(1180), U(1190),
	U(1200), U(1210), U(1220), U(1230), U(1240),
	U(1250), U(1260), U(1270), U(1280), U(1290),
	U(1300), U(1310), U(1320), U(1330), U(1340),
	U(1350), U(1360), U(1370), U(1380), U(1390),
	U(1400), U(1410), U(1420), U(1430), U(1440),
	U(1450), U(1460), U(1470), U(1480), U(1490),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500)
};

static const uint16_t buck457_volt_table[BUCK_VOLT_TABLE_SIZE] = {
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1500), U(1500), U(1500), U(1500),
	U(1500), U(1600), U(1700), U(1800), U(1900),
	U(2000), U(2100), U(2200), U(2300), U(2400),
	U(2500), U(2600), U(2700), U(2800), U(2900),
	U(3000), U(3100), U(3200), U(3300), U(3400),
	U(3500), U(3600), U(3700), U(3800), U(3900),
	U(4000), U(4100), U(4200)
};

static const uint16_t ldo235678_volt_table[LDO235678_VOLT_TABLE_SIZE] = {
	U(900), U(1000), U(1100), U(1200), U(1300),
	U(1400), U(1500), U(1600), U(1700), U(1800),
	U(1900), U(2000), U(2100), U(2200), U(2300),
	U(2400), U(2500), U(2600), U(2700), U(2800),
	U(2900), U(3000), U(3100), U(3200), U(3300),
	U(3400), U(3500), U(3600), U(3700), U(3800),
	U(3900), U(4000)
};

static const uint16_t ldo1_volt_table[LDO_SINGLE_VOLT_TABLE_SIZE] = {
	U(1800),
};

static const uint16_t ldo4_volt_table[LDO_SINGLE_VOLT_TABLE_SIZE] = {
	U(3300),
};

static const uint16_t refddr_volt_table[LDO_SINGLE_VOLT_TABLE_SIZE] = {
	U(0),
};

#define DEFINE_BUCK(regu_name, ID, pd, table) { \
	.name			= regu_name, \
	.volt_table		= table, \
	.volt_table_size	= BUCK_VOLT_TABLE_SIZE, \
	.en_cr			= ID ## _MAIN_CR2, \
	.volt_cr		= ID ## _MAIN_CR1, \
	.alt_en_cr		= ID ## _ALT_CR2, \
	.alt_volt_cr		= ID ## _ALT_CR1, \
	.pwrctrl_cr		= ID ## _PWRCTRL_CR, \
	.msrt_reg		= BUCKS_MRST_CR, \
	.msrt_mask		= ID ## _MRST, \
	.pd_reg			= pd, \
	.pd_val			= ID ## _PD_FAST, \
	.pd_mask		= ID ## _PD_MASK, \
	.ocp_reg		= FS_OCP_CR1, \
	.ocp_mask		= FS_OCP_ ## ID, \
}

#define DEFINE_LDOx(regu_name, ID, table, size) { \
	.name			= regu_name, \
	.volt_table		= table, \
	.volt_table_size	= size, \
	.volt_shift		= LDO_VOLT_SHIFT, \
	.en_cr			= ID ## _MAIN_CR, \
	.volt_cr		= ID ## _MAIN_CR, \
	.alt_en_cr		= ID ## _ALT_CR, \
	.alt_volt_cr		= ID ## _ALT_CR, \
	.pwrctrl_cr		= ID ## _PWRCTRL_CR, \
	.msrt_reg		= LDOS_MRST_CR, \
	.msrt_mask		= ID ## _MRST, \
	.pd_reg			= LDOS_PD_CR1, \
	.pd_val			= ID ## _PD, \
	.pd_mask		= ID ## _PD, \
	.ocp_reg		= FS_OCP_CR2, \
	.ocp_mask		= FS_OCP_ ## ID, \
}

#define DEFINE_REFDDR(regu_name, ID, table) { \
	.name			= regu_name, \
	.volt_table		= table, \
	.volt_table_size	= LDO_SINGLE_VOLT_TABLE_SIZE, \
	.en_cr			= ID ## _MAIN_CR, \
	.volt_cr		= ID ## _MAIN_CR, \
	.alt_en_cr		= ID ## _ALT_CR, \
	.alt_volt_cr		= ID ## _ALT_CR, \
	.pwrctrl_cr		= ID ## _PWRCTRL_CR, \
	.msrt_reg		= BUCKS_MRST_CR, \
	.msrt_mask		= ID ## _MRST, \
	.pd_reg			= LDOS_PD_CR2, \
	.pd_val			= ID ## _PD, \
	.pd_mask		= ID ## _PD, \
	.ocp_reg		= FS_OCP_CR1, \
	.ocp_mask		= FS_OCP_ ## ID, \
}

/* Table of Regulators in PMIC SoC */
static const struct regul_struct regul_table[] = {
	[STPMIC2_BUCK1] = DEFINE_BUCK("buck1", BUCK1, BUCKS_PD_CR1,
				      buck1236_volt_table),
	[STPMIC2_BUCK2] = DEFINE_BUCK("buck2", BUCK2, BUCKS_PD_CR1,
				      buck1236_volt_table),
	[STPMIC2_BUCK3] = DEFINE_BUCK("buck3", BUCK3, BUCKS_PD_CR1,
				      buck1236_volt_table),
	[STPMIC2_BUCK4] = DEFINE_BUCK("buck4", BUCK4, BUCKS_PD_CR1,
				      buck457_volt_table),
	[STPMIC2_BUCK5] = DEFINE_BUCK("buck5", BUCK5, BUCKS_PD_CR2,
				      buck457_volt_table),
	[STPMIC2_BUCK6] = DEFINE_BUCK("buck6", BUCK6, BUCKS_PD_CR2,
				      buck1236_volt_table),
	[STPMIC2_BUCK7] = DEFINE_BUCK("buck7", BUCK7, BUCKS_PD_CR2,
				      buck457_volt_table),

	[STPMIC2_REFDDR] = DEFINE_REFDDR("refddr", REFDDR, refddr_volt_table),

	[STPMIC2_LDO1] = DEFINE_LDOx("ldo1", LDO1, ldo1_volt_table,
				     LDO_SINGLE_VOLT_TABLE_SIZE),
	[STPMIC2_LDO2] = DEFINE_LDOx("ldo2", LDO2, ldo235678_volt_table,
				     LDO235678_VOLT_TABLE_SIZE),
	[STPMIC2_LDO3] = DEFINE_LDOx("ldo3", LDO3, ldo235678_volt_table,
				     LDO235678_VOLT_TABLE_SIZE),
	[STPMIC2_LDO4] = DEFINE_LDOx("ldo4", LDO4, ldo4_volt_table,
				     LDO_SINGLE_VOLT_TABLE_SIZE),
	[STPMIC2_LDO5] = DEFINE_LDOx("ldo5", LDO5, ldo235678_volt_table,
				     LDO235678_VOLT_TABLE_SIZE),
	[STPMIC2_LDO6] = DEFINE_LDOx("ldo6", LDO6, ldo235678_volt_table,
				     LDO235678_VOLT_TABLE_SIZE),
	[STPMIC2_LDO7] = DEFINE_LDOx("ldo7", LDO7, ldo235678_volt_table,
				     LDO235678_VOLT_TABLE_SIZE),
	[STPMIC2_LDO8] = DEFINE_LDOx("ldo8", LDO8, ldo235678_volt_table,
				     LDO235678_VOLT_TABLE_SIZE),

};

TEE_Result stpmic2_register_read(struct pmic_handle_s *pmic,
				 uint8_t register_id, uint8_t *value)
{
	int res =
	stm32_i2c_read_write_membyte(pmic->pmic_i2c_handle,
				     pmic->pmic_i2c_addr,
				     register_id, value,
				     false /* !write */);
	if (res) {
		EMSG("Failed to read reg:0x%x", register_id);
		return TEE_ERROR_COMMUNICATION;
	}

	FMSG("REG:0x%x v=0x%x", register_id, *value);

	return TEE_SUCCESS;
}

TEE_Result stpmic2_register_write(struct pmic_handle_s *pmic,
				  uint8_t register_id, uint8_t value)
{
	int res =
	stm32_i2c_read_write_membyte(pmic->pmic_i2c_handle,
				     pmic->pmic_i2c_addr,
				     register_id, &value,
				     true /* write */);
	if (res) {
		EMSG("Failed to write reg:0x%x", register_id);
		return TEE_ERROR_COMMUNICATION;
	}

	FMSG("REG:0x%x writen v=0x%x", register_id, value);

	return TEE_SUCCESS;
}

TEE_Result stpmic2_register_update(struct pmic_handle_s *pmic,
				   uint8_t register_id, uint8_t value,
				   uint8_t mask)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	uint8_t val = U(0);

	res = stpmic2_register_read(pmic, register_id, &val);
	if (res)
		return res;

	val = (val & ~mask) | (value & mask);

	FMSG("REG:0x%x v=0x%x mask=0x%x -> 0x%x", register_id, value, mask,
	     val);

	return stpmic2_register_write(pmic, register_id, val);
}

TEE_Result stpmic2_regulator_set_state(struct pmic_handle_s *pmic,
				       uint8_t id, bool enable)
{
	const struct regul_struct *regul = &regul_table[id];

	if (enable)
		return stpmic2_register_update(pmic,
					       regul->en_cr, U(1), U(1));
	else
		return stpmic2_register_update(pmic,
					       regul->en_cr, U(0), U(1));
}

TEE_Result stpmic2_regulator_get_state(struct pmic_handle_s *pmic,
				       uint8_t id, bool *enabled)
{
	const struct regul_struct *regul = &regul_table[id];
	TEE_Result res = TEE_ERROR_GENERIC;
	uint8_t val = U(0);

	res = stpmic2_register_read(pmic, regul->en_cr, &val);
	if (res)
		return res;

	*enabled = (val & U(1)) == U(1);

	return TEE_SUCCESS;
}

TEE_Result stpmic2_regulator_levels_mv(struct pmic_handle_s *pmic __unused,
				       uint8_t id, const uint16_t **levels,
				       size_t *levels_count)
{
	const struct regul_struct *regul = &regul_table[id];

	if (!regul)
		return TEE_ERROR_BAD_PARAMETERS;

	if (levels_count)
		*levels_count = regul->volt_table_size;
	if (levels)
		*levels = regul->volt_table;

	return TEE_SUCCESS;
}

TEE_Result stpmic2_regulator_get_voltage(struct pmic_handle_s *pmic,
					 uint8_t id, uint16_t *val)
{
	const struct regul_struct *regul = &regul_table[id];
	uint8_t value = U(0);
	uint8_t mask = U(0);

	if (regul->volt_table_size == 0)
		return TEE_ERROR_GENERIC;

	mask = regul->volt_table_size - U(1);
	if (mask) {
		TEE_Result res = stpmic2_register_read(pmic, regul->volt_cr,
						       &value);
		if (res)
			return res;

		value = (value >> regul->volt_shift) & mask;
	}

	if (value > regul->volt_table_size)
		panic();

	*val = regul->volt_table[value];

	return TEE_SUCCESS;
}

static size_t voltage_to_index(const struct regul_struct *regul,
			       uint16_t millivolts)
{
	unsigned int i = 0;

	assert(regul->volt_table);
	for (i = 0; i < regul->volt_table_size; i++)
		if (regul->volt_table[i] == millivolts)
			return i;

	return VOLTAGE_INDEX_INVALID;
}

TEE_Result stpmic2_regulator_set_voltage(struct pmic_handle_s *pmic,
					 uint8_t id, uint16_t millivolts)
{
	const struct regul_struct *regul = &regul_table[id];
	size_t index = U(0);
	uint8_t mask = U(0);

	if (!regul->volt_table_size)
		return TEE_SUCCESS;

	mask = regul->volt_table_size - U(1);

	index = voltage_to_index(regul, millivolts);
	if (index == VOLTAGE_INDEX_INVALID)
		return TEE_ERROR_BAD_PARAMETERS;

	return stpmic2_register_update(pmic, regul->volt_cr,
				       index << regul->volt_shift,
				       mask << regul->volt_shift);
}

/* update both normal and alternate register */
static TEE_Result stpmic2_update_en_crs(struct pmic_handle_s *pmic, uint8_t id,
					uint8_t value, uint8_t mask)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	const struct regul_struct *regul = &regul_table[id];

	res = stpmic2_register_update(pmic, regul->en_cr, value, mask);
	if (res)
		return res;

	return stpmic2_register_update(pmic, regul->alt_en_cr, value, mask);
}

TEE_Result stpmic2_regulator_get_prop(struct pmic_handle_s *pmic, uint8_t id,
				      enum stpmic2_prop_id prop, uint8_t *arg)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	const struct regul_struct *regul = &regul_table[id];
	uint8_t val = U(0);

	DMSG("%s: get prop 0x%x", regul->name, prop);

	switch (prop) {
	case STPMIC2_BYPASS:
		*arg = PROP_BYPASS_RESET;

		if (id <= STPMIC2_BUCK7 || id == STPMIC2_LDO1 ||
		    id == STPMIC2_LDO4 || id == STPMIC2_REFDDR)
			return TEE_SUCCESS;

		res = stpmic2_register_read(pmic, regul->en_cr, &val);
		if (res)
			return res;

		if (val & LDO_BYPASS)
			*arg = PROP_BYPASS_SET;

		break;
	default:
		EMSG("Invalid prop %#"PRIx16, prop);
		panic();
	}

	return TEE_SUCCESS;
}

TEE_Result stpmic2_regulator_set_prop(struct pmic_handle_s *pmic, uint8_t id,
				      enum stpmic2_prop_id prop, uint8_t arg)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	const struct regul_struct *regul = &regul_table[id];

	DMSG("%s: set prop 0x%x arg=%u", regul->name, prop, arg);

	switch (prop) {
	case STPMIC2_PULL_DOWN:
		return stpmic2_register_update(pmic, regul->pd_reg,
					       regul->pd_val,
					       regul->pd_val);
	case STPMIC2_MASK_RESET:
		if (!regul->msrt_mask)
			return TEE_ERROR_BAD_PARAMETERS;
		/* enable mask reset */
		return stpmic2_register_update(pmic, regul->msrt_reg,
					       regul->msrt_mask,
					       regul->msrt_mask);
	case STPMIC2_BYPASS:
		if (id <= STPMIC2_BUCK7 || id == STPMIC2_LDO1 ||
		    id == STPMIC2_LDO4 || id == STPMIC2_REFDDR)
			return TEE_ERROR_BAD_PARAMETERS;

		/* clear sink source mode */
		if (id == STPMIC2_LDO3) {
			res = stpmic2_update_en_crs(pmic, id, U(0),
						    LDO3_SNK_SRC);
			if (res)
				return res;
		}

		/* set or reset bypass mode */
		return stpmic2_update_en_crs(pmic, id,
					     (arg == PROP_BYPASS_SET) ?
						LDO_BYPASS : U(0),
					     LDO_BYPASS);
	case STPMIC2_SINK_SOURCE:
		if (id != STPMIC2_LDO3)
			return TEE_ERROR_BAD_PARAMETERS;

		/* clear bypass mode */
		res = stpmic2_update_en_crs(pmic, id, U(0), LDO_BYPASS);
		if (res)
			return res;

		return stpmic2_update_en_crs(pmic, id, LDO3_SNK_SRC,
					     LDO3_SNK_SRC);
	case STPMIC2_OCP:
		/* enable mask reset */
		return stpmic2_register_update(pmic, regul->ocp_reg,
					       U(0), regul->ocp_mask);
	case STPMIC2_PWRCTRL_EN:
		/* set pwrctrl enable, disable pwrctrl reset */
		return stpmic2_register_update(pmic, regul->pwrctrl_cr,
					       PWRCTRL_EN,
					       PWRCTRL_EN | PWRCTRL_RS);
	case STPMIC2_PWRCTRL_RS:
		/* set pwrctrl reset, disable pwrctrl enable */
		return stpmic2_register_update(pmic, regul->pwrctrl_cr,
					       PWRCTRL_RS,
					       PWRCTRL_EN | PWRCTRL_RS);
	case STPMIC2_PWRCTRL_SEL:
		/* select pwr control line */
		return stpmic2_register_update(pmic, regul->pwrctrl_cr,
					       arg << PWRCTRL_SEL_SHIFT,
					       PWRCTRL_SEL_MASK);
	case STPMIC2_MAIN_PREG_MODE:
		/* only a buck has preg mode */
		if (id > STPMIC2_BUCK7)
			return TEE_ERROR_BAD_PARAMETERS;
		if (arg > 2)
			return TEE_ERROR_BAD_PARAMETERS;
		/* set preg mode on BUCKx_MAIN_CR2 */
		return stpmic2_register_update(pmic, regul->en_cr,
					       arg << PREG_MODE_SHIFT,
					       PREG_MODE_MASK);
	case STPMIC2_ALT_PREG_MODE:
		/* only a buck has preg mode */
		if (id > STPMIC2_BUCK7)
			return TEE_ERROR_BAD_PARAMETERS;
		if (arg > 2)
			return TEE_ERROR_BAD_PARAMETERS;
		/* set preg mode on BUCKx_MAIN_CR2 */
		return stpmic2_register_update(pmic, regul->alt_en_cr,
					       arg << PREG_MODE_SHIFT,
					       PREG_MODE_MASK);
	case STPMIC2_ALTERNATE_INPUT_SOURCE:
		if (id != STPMIC2_LDO1)
			return TEE_ERROR_BAD_PARAMETERS;

		return stpmic2_update_en_crs(pmic, id,
					     LDO1_INPUT_SRC, LDO1_INPUT_SRC);
	default:
		EMSG("Invalid prop %#"PRIx16, prop);
		panic();
	}

	return 0;
}

void stpmic2_dump_regulators(struct pmic_handle_s *pmic)
{
	size_t i = U(0);
	char __maybe_unused const *name = NULL;

	assert(pmic);

	for (i = U(0); i < ARRAY_SIZE(regul_table); i++) {
		uint16_t val = U(0);
		bool state = false;
		uint8_t arg = U(0);

		if (!regul_table[i].volt_cr)
			continue;

		stpmic2_regulator_get_voltage(pmic, i, &val);
		stpmic2_regulator_get_state(pmic, i, &state);
		stpmic2_regulator_get_prop(pmic, i, STPMIC2_BYPASS, &arg);

		name = regul_table[i].name;
		DMSG("PMIC regul %s: %s%s, %dmV", name,
		     state ? "EN" : "DIS",
		     arg == PROP_BYPASS_SET ? " BYPASS" : "",  val);
	}
}

TEE_Result stpmic2_get_version(struct pmic_handle_s *pmic, uint8_t *val)
{
	return stpmic2_register_read(pmic, VERSION_SR, val);
}

TEE_Result stpmic2_get_product_id(struct pmic_handle_s *pmic, uint8_t *val)
{
	return stpmic2_register_read(pmic, PRODUCT_ID, val);
}

/*
 *
 * Low Power
 *
 */
TEE_Result stpmic2_lp_set_state(struct pmic_handle_s *pmic, uint8_t id,
				bool enable)
{
	const struct regul_struct *regul = &regul_table[id];

	if (enable)
		return stpmic2_register_update(pmic,
					       regul->alt_en_cr, U(1), U(1));
	else
		return stpmic2_register_update(pmic,
					       regul->alt_en_cr, U(0), U(1));
}

TEE_Result stpmic2_lp_set_voltage(struct pmic_handle_s *pmic, uint8_t id,
				  uint16_t mv)
{
	const struct regul_struct *regul = &regul_table[id];
	size_t index = U(0);
	uint8_t mask = U(0);

	if (!regul->volt_table_size)
		return TEE_SUCCESS;

	mask = regul->volt_table_size - U(1);

	index = voltage_to_index(regul, mv);
	if (index == VOLTAGE_INDEX_INVALID)
		return TEE_ERROR_GENERIC;

	return stpmic2_register_update(pmic, regul->alt_volt_cr,
				       index << regul->volt_shift,
				       mask << regul->volt_shift);
}

/*
 *
 * Interrupt handling
 *
 */
TEE_Result stpmic2_set_irq_mask(struct pmic_handle_s *pmic, uint8_t num,
				bool state)
{
	uint8_t reg = INT_MASK_R1 + num / U(8);
	uint8_t mask = BIT(num % U(8));
	uint8_t val = state ? mask : U(0);

	return stpmic2_register_update(pmic, reg, val, mask);
}

TEE_Result stpmic2_irq_gen(struct pmic_handle_s *pmic, uint8_t num)
{
	uint8_t reg = INT_DBG_LATCH_R1 + num / U(8);
	uint8_t mask = BIT(num % U(8));

	return stpmic2_register_update(pmic, reg, mask, mask);
}

static void stpmic2_handle_irq_reg(struct pmic_handle_s *pmic,
				   uint8_t reg_index, uint8_t val)
{
	int i = 0;

	for (i = 0; i < 7; i++) {
		if (val & BIT(i)) {
			uint8_t it_id = reg_index * U(8) + i;
			enum itr_return res;

			res = stpmic2_irq_callback(pmic, it_id);

			/* handle over-current protection */
			if (res != ITRR_HANDLED && it_id >= IT_BUCK1_OCP) {
				uint8_t regu_id = (reg_index - U(2)) * U(8) + i;

				EMSG("Overcurrent on %s, disable regulator",
				     regul_table[regu_id].name);

				stpmic2_regulator_set_state(pmic, regu_id,
							    false);
				panic();
			}
		}
	}
}

int stpmic2_handle_irq(struct pmic_handle_s *pmic)
{
	uint8_t read_val = U(0);
	int i = 0;

	FMSG("Stpmic2 irq handler");

	for (i = 0; i < 4; i++) {
		if (stpmic2_register_read(pmic, INT_PENDING_R1 + i,
					  &read_val))
			panic();

		if (read_val) {
			FMSG("Stpmic2 irq pending reg=%u irq=0x%x", i,
			     read_val);

			pmic->irq_count++;

			stpmic2_handle_irq_reg(pmic, i, read_val);

			if (stpmic2_register_write(pmic,  INT_CLEAR_R1 + i,
						   read_val)) {
				panic();
			}
		}
	}

	return 0;
}
