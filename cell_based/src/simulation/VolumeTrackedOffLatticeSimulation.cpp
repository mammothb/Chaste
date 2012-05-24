/*

Copyright (c) 2005-2012, University of Oxford.
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

#include "VolumeTrackedOffLatticeSimulation.hpp"
#include "MeshBasedCellPopulation.hpp"

template<unsigned DIM>
VolumeTrackedOffLatticeSimulation<DIM>::VolumeTrackedOffLatticeSimulation(AbstractCellPopulation<DIM>& rCellPopulation,
                                                      bool deleteCellPopulationInDestructor,
                                                      bool initialiseCells)
    : OffLatticeSimulation<DIM>(rCellPopulation, deleteCellPopulationInDestructor, initialiseCells)
{
}

template<unsigned DIM>
VolumeTrackedOffLatticeSimulation<DIM>::~VolumeTrackedOffLatticeSimulation()
{
}

template<unsigned DIM>
void VolumeTrackedOffLatticeSimulation<DIM>::SetupSolve()
{
    /*
     * We must update CellData in SetupSolve(), otherwise it will not have been
     * fully initialised by the time we enter the main time loop.
     */
    UpdateCellData();
}

template<unsigned DIM>
void VolumeTrackedOffLatticeSimulation<DIM>::UpdateAtEndOfTimeStep()
{
    UpdateCellData();
}

template<unsigned DIM>
void VolumeTrackedOffLatticeSimulation<DIM>::UpdateCellData()
{
    // Make sure the cell population is updated
    this->mrCellPopulation.Update();

    /**
     * This hack is needed because in the case of a MeshBasedCellPopulation in which
     * multiple cell divisions have occurred over one time step, the Voronoi tessellation
     * (while existing) is out-of-date. Thus, if we did not regenerate the Voronoi
     * tessellation here, an assertion may trip as we try to access a Voronoi element
     * whose index exceeds the number of elements in the out-of-date tessellation.
     * 
     * \todo work out how to properly fix this (#1986)
     */
    if (dynamic_cast<MeshBasedCellPopulation<DIM>*>(&(this->mrCellPopulation)))
    {
        static_cast<MeshBasedCellPopulation<DIM>*>(&(this->mrCellPopulation))->CreateVoronoiTessellation();
    }

    // Iterate over cell population
    for (typename AbstractCellPopulation<DIM>::Iterator cell_iter = this->mrCellPopulation.Begin();
         cell_iter != this->mrCellPopulation.End();
         ++cell_iter)
    {
        // Get the volume of this cell
        double cell_volume = this->mrCellPopulation.GetVolumeOfCell(*cell_iter);

        // Store the cell's volume in CellData
///\todo #2115        cell_iter->GetCellData()->SetItem("volume", cell_volume);
        cell_iter->GetCellData()->SetItem(0, cell_volume);
    }
}

/////////////////////////////////////////////////////////////////////////////
// Explicit instantiation
/////////////////////////////////////////////////////////////////////////////

template class VolumeTrackedOffLatticeSimulation<1>;
template class VolumeTrackedOffLatticeSimulation<2>;
template class VolumeTrackedOffLatticeSimulation<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(VolumeTrackedOffLatticeSimulation)
