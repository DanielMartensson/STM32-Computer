/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2023, STMicroelectronics
 */

#ifndef SCMI_AGENT_CONFIGURATION_H
#define SCMI_AGENT_CONFIGURATION_H

#include <drivers/clk.h>
#include <drivers/regulator.h>
#include <drivers/rstctrl.h>
#include <scmi_regulator_consumer.h>
#include <stdint.h>

/* Structure used to describe the SCMI agents */

/*
 * struct scmi_clock - Description of a clock domain
 * @name: Domain name
 * @clk: Clock instance controlled by the domain
 * @enabled: Default state of the clock
 */
struct scmi_clock {
	const char *name;
	struct clk *clk;
	bool enabled;
};

/*
 * struct scmi_reset - Description of a reset domain
 * @name: Domain name
 * @rstctrl: Reset controlled by the domain
 */
struct scmi_reset {
	const char *name;
	struct rstctrl *rstctrl;
};

/*
 * struct scmi_voltd - Description of a voltage domaine resource
 * @name: Domain name
 * @rdev: Regulator controlled by the voltage domain
 * @enabled: Default state of the regulator
 */
struct scmi_voltd {
	const char *name;
	struct rdev *rdev;
	bool enabled;
};

/*
 * struct scmi_perfd - DVFS/performance domaindescription
 * @name: DVFS name
 * @dvfs_opp_count: Number of cells in @dvfs_opp_khz and @dvfs_opp_mv
 * @initial_opp: Initial operating point
 * @dvfs_opp_khz: Operating point frequencies in Hertz
 * @dvfs_opp_mv: Operating point voltage levels in millivolts
 * @clk: Clock used by the DVFS service
 * @rdev: Regulator used by the DVFS service
 *
 * SCP firmware configuration expects array couple @dvfs_opp_khz and
 * @dvfs_opp_mv values are ordered by increasing performance operating points.
 */
struct scmi_perfd {
	const char *name;
	size_t dvfs_opp_count;
	size_t initial_opp;
	unsigned int *dvfs_opp_khz;
	unsigned int *dvfs_opp_mv;
	struct clk *clk;
	struct rdev *rdev;
};

/*
 * struct scmi_pd - Description of a power domain resource
 * @name: Domain name
 * @clk: Clock controlled by the power domain or NULL
 * @regu: Voltage regulator controlled by the power domain or NULL
 */
struct scmi_pd {
	const char *name;
	struct clk *clk;
	struct rdev *regu;
};

/*
 * struct scpfw_channel_config - SCMI channel resources
 * @name: Channel name
 * @channel_id: ID for the channel in OP-TEE SCMI bindings
 * @clock: Description of the clocks exposed on the channel
 * @clock_count: Number of cells of @clock
 * @reset: Description of the reset conntrollers exposed on the channel
 * @reset_count: Number of cells of @reset
 * @voltd: Description of the regulators exposed on the channel
 * @voltd_count: Number of cells of @voltd
 * @perfd: Description of the DVFS/perf exposed on the channel
 * @perfd_count: Number of cells of @perfd
 * @pd: Description of the power domain exposed on the channel
 * @pd_count: Number of cells of @pd
 */
struct scpfw_channel_config {
	const char *name;
	unsigned int channel_id;
	struct scmi_clock *clock;
	size_t clock_count;
	struct scmi_reset *reset;
	size_t reset_count;
	struct scmi_voltd *voltd;
	size_t voltd_count;
	struct scmi_perfd *perfd;
	size_t perfd_count;
	struct scmi_pd *pd;
	size_t pd_count;
};

/*
 * struct scpfw_agent_config - SCMI agent description
 * @name: Agent name exposed through SCMI
 * @agent_id: Agent ID exposed through SCMI
 * @channel_config: Channel exposed by the agent
 * @channel_count: Number of cells in @channel_config
 *
 * There is currently a constraint that mandate @agent_id is the
 * index minus 1 of the agent in array struct scpfw_config::agent_config
 * This is used as agent ID 0 is always the SCMI server ID value.
 */
struct scpfw_agent_config {
	const char *name;
	unsigned int agent_id;
	struct scpfw_channel_config *channel_config;
	size_t channel_count;
};

/*
 * struct scpfw_config - SCP firmware configuration root node
 * @agent_config: Agents exposed with SCMI
 * @agent_count: Number of cells in @agent_config
 */
struct scpfw_config {
	struct scpfw_agent_config *agent_config;
	size_t agent_count;
};

/* Get the platform configuration data for the SCP firmware */
struct scpfw_config *scmi_scpfw_get_configuration(void);

/* Release resources allocated to create SCP-firmware configuration data */
void scmi_scpfw_release_configuration(void);

/* Add a DVFS service in the SCMI server */
TEE_Result scmi_scpfw_cfg_add_dvfs(unsigned int agent, unsigned int channel,
				   unsigned int domain_id,
				   struct rdev *regulator, struct clk *clk,
				   unsigned int *dvfs_khz,
				   unsigned int *dvfs_mv, size_t dvfs_count,
				   size_t initial_index);

/* Add a voltage domains/regulators service to an SCMI channel */
TEE_Result scmi_scpfw_cfg_add_regu(unsigned int agent, unsigned int channel,
				   struct scmi_server_regu *regu,
				   size_t regu_count);

/* SCP firmware SCMI server configuration entry point */
void scpfw_configure(struct scpfw_config *cfg);

#endif /* SCMI_AGENT_CONFIGURATION_H */
