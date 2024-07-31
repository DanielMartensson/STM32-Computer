// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2021, Bootlin
 */

#include <drivers/clk.h>
#include <kernel/boot.h>
#include <kernel/panic.h>
#include <kernel/spinlock.h>
#include <libfdt.h>
#include <malloc.h>
#include <stddef.h>
#include <sys/queue.h>

/* Global clock tree lock */
static unsigned int clk_lock = SPINLOCK_UNLOCK;

static STAILQ_HEAD(, clk) clock_list =
		STAILQ_HEAD_INITIALIZER(clock_list);

struct clk *clk_alloc(const char *name, const struct clk_ops *ops,
		      struct clk **parent_clks, size_t parent_count)
{
	struct clk *clk = NULL;
	size_t parent = 0;

	clk = calloc(1, sizeof(*clk) + parent_count * sizeof(clk));
	if (!clk)
		return NULL;

	clk->num_parents = parent_count;
	for (parent = 0; parent < parent_count; parent++)
		clk->parents[parent] = parent_clks[parent];

	clk->name = name;
	clk->ops = ops;
	refcount_set(&clk->enabled_count, 0);

	return clk;
}

void clk_free(struct clk *clk)
{
	free(clk);
}

static bool __maybe_unused clk_check(struct clk *clk)
{
	if (!clk->ops)
		return false;

	if (clk->ops->set_parent && !clk->ops->get_parent)
		return false;

	if (clk->num_parents > 1 && !clk->ops->get_parent)
		return false;

	return true;
}

static void clk_compute_rate_no_lock(struct clk *clk)
{
	unsigned long parent_rate = 0;

	if (clk->parent)
		parent_rate = clk->parent->rate;

	if (clk->ops->get_rate)
		clk->rate = clk->ops->get_rate(clk, parent_rate);
	else
		clk->rate = parent_rate;
}

struct clk *clk_get_parent_by_index(struct clk *clk, size_t pidx)
{
	if (pidx >= clk->num_parents)
		return NULL;

	return clk->parents[pidx];
}

static void clk_init_parent(struct clk *clk)
{
	size_t pidx = 0;

	switch (clk->num_parents) {
	case 0:
		break;
	case 1:
		clk->parent = clk->parents[0];
		break;
	default:
		pidx = clk->ops->get_parent(clk);
		assert(pidx < clk->num_parents);

		clk->parent = clk->parents[pidx];
		break;
	}
}

TEE_Result clk_reparent(struct clk *clk, struct clk *parent)
{
	size_t i = 0;

	if (clk->parent == parent)
		return TEE_SUCCESS;

	for (i = 0; i < clk_get_num_parents(clk); i++) {
		if (clk_get_parent_by_index(clk, i) == parent) {
			clk->parent = parent;
			return TEE_SUCCESS;
		}
	}
	EMSG("Clock %s is not a parent of clock %s", parent->name, clk->name);

	return TEE_ERROR_BAD_PARAMETERS;
}

TEE_Result clk_register(struct clk *clk)
{
	assert(clk_check(clk));

	clk_init_parent(clk);
	clk_compute_rate_no_lock(clk);

	DMSG("Registered clock %s, freq %lu", clk->name, clk_get_rate(clk));

	STAILQ_INSERT_TAIL(&clock_list, clk, link);

	return TEE_SUCCESS;
}

static bool clk_is_enabled_no_lock(struct clk *clk)
{
	return refcount_val(&clk->enabled_count) != 0;
}

bool clk_is_enabled(struct clk *clk)
{
	return clk_is_enabled_no_lock(clk);
}

static void clk_disable_no_lock(struct clk *clk)
{
	struct clk *parent = NULL;

	if (!refcount_dec(&clk->enabled_count))
		return;

	if (clk->ops->disable)
		clk->ops->disable(clk);

	parent = clk_get_parent(clk);
	if (parent)
		clk_disable_no_lock(parent);
}

static TEE_Result clk_enable_no_lock(struct clk *clk)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	struct clk *parent = NULL;

	if (refcount_inc(&clk->enabled_count))
		return TEE_SUCCESS;

	parent = clk_get_parent(clk);
	if (parent) {
		res = clk_enable_no_lock(parent);
		if (res)
			return res;
	}

	if (clk->ops->enable) {
		res = clk->ops->enable(clk);
		if (res) {
			if (parent)
				clk_disable_no_lock(parent);

			return res;
		}
	}

	refcount_set(&clk->enabled_count, 1);

	return TEE_SUCCESS;
}

TEE_Result clk_enable(struct clk *clk)
{
	uint32_t exceptions = 0;
	TEE_Result res = TEE_ERROR_GENERIC;

	exceptions = cpu_spin_lock_xsave(&clk_lock);
	res = clk_enable_no_lock(clk);
	cpu_spin_unlock_xrestore(&clk_lock, exceptions);

	return res;
}

void clk_disable(struct clk *clk)
{
	uint32_t exceptions = 0;

	exceptions = cpu_spin_lock_xsave(&clk_lock);
	clk_disable_no_lock(clk);
	cpu_spin_unlock_xrestore(&clk_lock, exceptions);
}

unsigned long clk_get_rate(struct clk *clk)
{
	unsigned long rate = clk->rate;

	if (clk->ops->get_rate) {
		unsigned long parent_rate =  0UL;

		if (clk->parent)
			parent_rate = clk_get_rate(clk->parent);

		return clk->ops->get_rate(clk, parent_rate);
	}

	if (clk->parent)
		rate = clk_get_rate(clk->parent);

	return rate;
}

struct clk *clk_get_parent(struct clk *clk)
{
	return clk->parent;
}

TEE_Result clk_get_parent_idx(struct clk *clk, struct clk *parent,
			      size_t *pidx)
{
	size_t i = 0;

	for (i = 0; i < clk_get_num_parents(clk); i++) {
		if (clk_get_parent_by_index(clk, i) == parent) {
			*pidx = i;
			return TEE_SUCCESS;
		}
	}
	EMSG("Clock %s is not a parent of clock %s", parent->name, clk->name);

	return TEE_ERROR_BAD_PARAMETERS;
}

static TEE_Result clk_set_parent_no_lock(struct clk *clk, struct clk *parent,
					 size_t pidx)
{
	struct clk *old_parent = clk->parent;
	TEE_Result res = TEE_ERROR_GENERIC;
	bool was_enabled = false;

	/* Requested parent is already the one set */
	if (clk->parent == parent)
		return TEE_SUCCESS;

	was_enabled = clk_is_enabled_no_lock(clk);

	if (clk->flags & CLK_OPS_PARENT_ENABLE) {
		res = clk_enable_no_lock(parent);
		if (res)
			panic("Failed to re-enable clock after setting parent");

		res = clk_enable_no_lock(old_parent);
		if (res)
			panic("Failed to re-enable clock after setting parent");
	}

	res = clk->ops->set_parent(clk, pidx);
	if (res)
		goto out;

	clk->parent = parent;

	/* The parent changed and the rate might also have changed */
	clk_compute_rate_no_lock(clk);

	/* Call is needed to decrement refcount on current parent tree */
	if (was_enabled) {
		res = clk_enable_no_lock(parent);
		if (res)
			panic("Failed to re-enable clock after setting parent");

		clk_disable_no_lock(old_parent);
	}
out:
	if (clk->flags & CLK_OPS_PARENT_ENABLE) {
		clk_disable_no_lock(old_parent);
		clk_disable_no_lock(parent);
	}

	return res;
}

TEE_Result clk_set_parent(struct clk *clk, struct clk *parent)
{
	size_t pidx = 0;
	uint32_t exceptions = 0;
	TEE_Result res = TEE_ERROR_GENERIC;

	if (clk_get_parent_idx(clk, parent, &pidx) || !clk->ops->set_parent)
		return TEE_ERROR_BAD_PARAMETERS;

	exceptions = cpu_spin_lock_xsave(&clk_lock);
	if (clk->flags & CLK_SET_PARENT_GATE && clk_is_enabled_no_lock(clk)) {
		res = TEE_ERROR_BAD_STATE;
		goto out;
	}

	res = clk_set_parent_no_lock(clk, parent, pidx);
out:
	cpu_spin_unlock_xrestore(&clk_lock, exceptions);

	return res;
}

TEE_Result clk_get_rates_array(struct clk *clk, size_t start_index,
			       unsigned long *rates, size_t *nb_elts)
{
	if (!clk->ops->get_rates_array)
		return TEE_ERROR_NOT_SUPPORTED;

	return clk->ops->get_rates_array(clk, start_index, rates, nb_elts);
}

TEE_Result clk_get_rates_steps(struct clk *clk, unsigned long *min,
			       unsigned long *max, unsigned long *step)
{
	if (!clk->ops->get_rates_steps)
		return TEE_ERROR_NOT_SUPPORTED;

	return clk->ops->get_rates_steps(clk, min, max, step);
}

static void clk_init_rate_req(struct clk *clk,
			      struct clk_rate_request *req)
{
	struct clk *parent = clk->parent;

	if (parent) {
		req->best_parent = parent;
		req->best_parent_rate = parent->rate;
	} else {
		req->best_parent = NULL;
		req->best_parent_rate = 0;
	}
}

static TEE_Result clk_set_rate_no_lock(struct clk *clk, unsigned long rate)
{
	TEE_Result res = TEE_SUCCESS;
	unsigned long parent_rate = 0;
	unsigned long new_rate = rate;
	struct clk *parent = clk->parent;

	if (parent)
		parent_rate = parent->rate;

	/* find the closest rate and parent clk/rate */
	if (clk->ops->determine_rate) {
		struct clk_rate_request req = { };
		struct clk *old_parent = clk->parent;

		req.rate = rate;

		clk_init_rate_req(clk, &req);

		res = clk->ops->determine_rate(clk, &req);
		if (res)
			return res;

		parent_rate = req.best_parent_rate;
		new_rate = req.rate;
		parent = req.best_parent;

		res = clk_set_rate_no_lock(parent, parent_rate);
		if (res)
			return res;

		if (parent && parent != old_parent) {
			if (parent && clk->num_parents > 1) {
				size_t pidx = 0;

				if (clk_get_parent_idx(clk, parent, &pidx))
					return TEE_ERROR_BAD_PARAMETERS;

				res = clk_set_parent_no_lock(clk, parent, pidx);
				if (res)
					return res;
				clk->parent = parent;
			}
		}
	} else if (!parent || !(clk->flags & CLK_SET_RATE_PARENT)) {
		/* pass-through clock without adjustable parent */
		new_rate  = rate;
	} else {
		/* pass-through clock with adjustable parent */
		res = clk_set_rate_no_lock(parent, rate);
		if (res)
			return res;
		new_rate = parent->rate;
	}

	if (clk->ops->set_rate) {
		if (clk->flags & CLK_SET_RATE_UNGATE)
			clk_enable_no_lock(clk);

		res = clk->ops->set_rate(clk, new_rate, parent_rate);

		if (clk->flags & CLK_SET_RATE_UNGATE)
			clk_disable_no_lock(clk);

		if (res)
			return res;
	}

	clk_compute_rate_no_lock(clk);

	return res;
}

TEE_Result clk_set_rate(struct clk *clk, unsigned long rate)
{
	uint32_t exceptions = 0;
	TEE_Result res = TEE_ERROR_GENERIC;

	/* bail early if nothing to do */
	if (rate == clk_get_rate(clk))
		return TEE_SUCCESS;

	exceptions =  cpu_spin_lock_xsave(&clk_lock);

	if (clk->flags & CLK_SET_RATE_GATE && clk_is_enabled_no_lock(clk))
		res = TEE_ERROR_BAD_STATE;
	else
		res = clk_set_rate_no_lock(clk, rate);

	cpu_spin_unlock_xrestore(&clk_lock, exceptions);

	return res;
}

TEE_Result clk_get_duty_cycle(struct clk *clk, struct clk_duty *duty)
{
	if (clk->ops->get_duty_cycle)
		return clk->ops->get_duty_cycle(clk, duty);

	if (clk->parent && (clk->flags & CLK_DUTY_CYCLE_PARENT))
		return clk_get_duty_cycle(clk->parent, duty);

	duty->num = 1;
	duty->den = 2;

	return TEE_SUCCESS;
}

unsigned long clk_round_rate(struct clk *clk, unsigned long rate)
{
	struct clk *parent = clk->parent;

	if (clk->ops->round_rate)
		return clk->ops->round_rate(clk, rate, clk->parent->rate);

	if (parent && (clk->flags & CLK_SET_RATE_PARENT))
		return clk_round_rate(parent, rate);

	return clk->rate;
}

#include <stdio.h>

static void clk_stm32_display_tree(struct clk *clk, int indent)
{
	unsigned long rate;
	const char *name;
	int counter = clk->enabled_count.val;
	int i;
	int state;

	name = clk_get_name(clk);
	rate = clk_get_rate(clk);

	state = clk->ops->is_enabled ? clk->ops->is_enabled(clk) : (counter > 0);

	printf("%02d %s ", counter, state ? "Y" : "N");

	for (i = 0; i < indent * 4; i++) {
		if ((i % 4) != 0)
			printf("-");
		else
			printf("|");
	}

	if (i != 0)
		printf(" ");

	printf("%s (%ld)\n", name, rate);
}

static void clk_stm32_tree(struct clk *clk_root, int indent)
{
	struct clk *clk = NULL;

	STAILQ_FOREACH(clk, &clock_list, link) {
		if (clk_get_parent(clk) == clk_root) {
			clk_stm32_display_tree(clk, indent + 1);
			clk_stm32_tree(clk, indent + 1);
		}
	}
}

void clk_summary(void)
{
	struct clk *clk = NULL;

	STAILQ_FOREACH(clk, &clock_list, link) {
		if (clk_get_parent(clk))
			continue;

		clk_stm32_display_tree(clk, 0);
		clk_stm32_tree(clk, 0);
	}
}

#ifdef CFG_PM
TEE_Result clk_save_context(void)
{
	struct clk *clk = NULL;
	TEE_Result res = TEE_ERROR_GENERIC;

	STAILQ_FOREACH(clk, &clock_list, link) {
		if (clk->ops && clk->ops->save_context) {
			res = clk->ops->save_context(clk);
			if (res) {
				EMSG("Failed to save context of %s", clk->name);
				return res;
			}
		}
	}

	return TEE_SUCCESS;
}

void clk_restore_context(void)
{
	struct clk *clk = NULL;

	STAILQ_FOREACH(clk, &clock_list, link) {
		if (clk->ops && clk->ops->restore_context)
			clk->ops->restore_context(clk);
	}
}
#endif
