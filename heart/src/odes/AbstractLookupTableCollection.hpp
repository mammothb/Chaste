/*

Copyright (c) 2005-2018, University of Oxford.
All rights reserved.

University of Oxford means the Chancellor, Masters and Scholars of the
University of Oxford, having an administrative office at Wellington
Square, Oxford OX1 2JD, UK.

This file is part of Chaste.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
 * Neither the name of the University of Oxford nor the names of its
   contributors may be used to endorse or promote products derived from this
   software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef ABSTRACTLOOKUPTABLECOLLECTION_HPP_
#define ABSTRACTLOOKUPTABLECOLLECTION_HPP_

#include <string>
#include <vector>

#include "GenericEventHandler.hpp"

/**
 * Base class for lookup tables used in optimised cells generated by PyCml.
 * Contains methods to query and adjust table parameters (i.e. size and spacing),
 * and an event handler to time table generation.
 */
class AbstractLookupTableCollection
{
public:
    /**
     * Default constructor.
     */
    AbstractLookupTableCollection();

    /**
     * @return the names of variables used to index lookup tables.
     */
    std::vector<std::string> GetKeyingVariableNames() const;

    /**
     * @return the number of lookup tables keyed by the given variable.
     *
     * @param rKeyingVariableName  the table key name
     */
    unsigned GetNumberOfTables(const std::string& rKeyingVariableName) const;

    /**
     * @return the properties of lookup tables keyed by the given variable.
     *
     * @param rKeyingVariableName  the table key name
     * @param rMin  will be filled with the lower table bound
     * @param rStep  will be filled with the table spacing
     * @param rMax  will be filled with the upper table bound
     */
    void GetTableProperties(const std::string& rKeyingVariableName, double& rMin, double& rStep, double& rMax) const;

    /**
     * Set the properties of lookup tables keyed by the given variable.
     *
     * @param rKeyingVariableName  the table key name
     * @param min  the lower table bound
     * @param step  the table spacing; must divide the interval between min and max exactly
     * @param max  the upper table bound
     */
    void SetTableProperties(const std::string& rKeyingVariableName, double min, double step, double max);

    /**
     * With some PyCml settings, the cell model timestep may be included within lookup tables.
     * If the cell's dt is changed, this method must be called to reflect this, and RegenerateTables
     * called to update the tables to match.
     *
     * @param dt  the new timestep
     */
    void SetTimestep(double dt);

    /**
     * Subclasses implement this method to generate the lookup tables based on the current settings.
     */
    virtual void RegenerateTables()=0;

    /**
     * You can call this method to free the memory used by lookup tables when they're no longer needed.
     *
     * In most usage scenarios you won't need to do this, but if you're running several simulations in turn that use different
     * cell models, you may find it useful to prevent running out of memory.
     *
     * @note After calling this method, you \b must call RegenerateTables before trying to simulate any cell using this
     * lookup tables object, or you'll get a segfault.
     */
    virtual void FreeMemory()=0;

    /** Virtual destructor since we have a virtual method. */
    virtual ~AbstractLookupTableCollection();

    /**
     * A little event handler with one event, to time table generation.
     */
    class EventHandler : public GenericEventHandler<1, EventHandler>
    {
    public:
        /** Names of the timing events. */
        static const char* EventName[1];

        /** Definition of timing event types. */
        typedef enum
        {
            GENERATE_TABLES=0
        } EventType;
    };

protected:
    /**
     * @return the index of the given keying variable within our vector.
     *
     * @param rKeyingVariableName  the table key name
     */
    unsigned GetTableIndex(const std::string& rKeyingVariableName) const;

    /** Names of variables used to index lookup tables */
    std::vector<std::string> mKeyingVariableNames;

    /** Number of tables indexed by each variable */
    std::vector<unsigned> mNumberOfTables;

    /** Spacing of tables indexed by each variable */
    std::vector<double> mTableSteps;

    /** Contains the reciprocals of #mTableSteps */
    std::vector<double> mTableStepInverses;

    /** Lower bound of tables indexed by each variable */
    std::vector<double> mTableMins;

    /** Upper bound of tables indexed by each variable */
    std::vector<double> mTableMaxs;

    /** Whether the parameters for each set of tables have changed */
    std::vector<bool> mNeedsRegeneration;

    /** Timestep to use in lookup tables */
    double mDt;
};

#endif // ABSTRACTLOOKUPTABLECOLLECTION_HPP_
