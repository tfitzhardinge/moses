/*
 * moses/moses/optimization/particle-swarm.h
 *
 * Copyright (C) 2002-2008 Novamente LLC
 * Copyright (C) 2012 Poulin Holdings
 * All Rights Reserved
 *
 * Written by Arley Ristar
 *            Nil Geisweiller
 *            Linas Vepstas
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#ifndef _MOSES_PARTICLE_SWARM_H
#define _MOSES_PARTICLE_SWARM_H

#include <opencog/util/oc_assert.h>

#include "../representation/instance_set.h"
#include "optimization.h"
#include "hill-climbing.h"

namespace opencog { namespace moses {

/// Hill-climbing paramters
struct ps_parameters
{
     // TODO: pso paramters


    // Range of scores for which to keep instances.  This *should* be
    // set to the value given by metapopulation::useful_score_range().
    // XXX TODO make sure this value is appropriately updated.
    //
    // The range of scores is used to keep the size of the deme in check.
    // The issue is that, for large feature sets, a large number of knobs
    // get created, which means that instances are huge.  It is easy to
    // end up with demes in the tens-of-gigabytes in size, and that's bad,
    // especially when most of the instances have terrible scores.
    score_t score_range;

    // Maximum allowed size of the deme.
    // This is used to keep the size of the deme in check.
    // The issue is that, for large feature sets, a large number of knobs
    // get created, which means that instances are huge.  It is easy to
    // end up with demes in the tens-of-gigabytes in size, and that's bad.
    size_t max_allowed_instances;

    // Flag to allow resizing the deme to keep memory usage under
    // control. Note, however, since it depends on the size of the
    // installed RAM, that two different runs of MOSES on two different
    // machines but otheriwse identical inputs and parameters, may not
    // behave identically to one-another.
    bool resize_to_fit_ram;

    // Deme stat name. String to indicate that TAB seperated deme
    // statistics are logged. By default 'Demes'.
    std::string prefix_stat_deme;
};

// TODO: pso description
// TODO: code pso, hill_climbing for now
struct particle_swarm : optimizer_base
{
    particle_swarm(const optim_parameters& op = optim_parameters(),
            const hc_parameters& ps = hc_parameters()) //const hc_parameters& ps = ps_parameters())
        : optimizer_base(op), ps_params(ps), _total_RAM_bytes(getTotalRAM())
    {}

protected:
    // log legend for graph stats
    void log_stats_legend();

    bool resize_deme(deme_t& deme, score_t score_cutoff);
    size_t resize_by_score(deme_t& deme, score_t score_cutoff);

public:
    /**
     * Perform search of the local neighborhood of an instance.  The
     * search is exhaustive if the local neighborhood is small; else
     * the local neighborhood is randomly sampled.
     *
     * @param deme      Where to store the candidates searched. The deme
     *                  is assumed to be empty.  If it is not empty, it
     *                  will be overwritten.
     * @prama init_inst Start the seach from this instance.
     * @param iscorer   the Scoring function.
     * @param max_evals The maximum number of evaluations to perform.
     */
    void operator()(deme_t& deme,
                    const instance& init_inst,
                    const iscorer_base& iscorer,
                    unsigned max_evals,
                    time_t max_time);

    // Like above but assumes that init_inst is null (backward compatibility)
    // XXX In fact, all of the current code uses this entry point, no one
    // bothers to supply an initial instance.
    void operator()(deme_t& deme,
                    const iscorer_base& iscorer,
                    unsigned max_evals,
                    time_t max_time)
    {
        instance init_inst(deme.fields().packed_width());
        operator()(deme, init_inst, iscorer, max_evals, max_time);
    }

protected:
    const hc_parameters ps_params;
    const uint64_t _total_RAM_bytes;
    size_t _instance_bytes;
};


} // ~namespace moses
} // ~namespace opencog

#endif
