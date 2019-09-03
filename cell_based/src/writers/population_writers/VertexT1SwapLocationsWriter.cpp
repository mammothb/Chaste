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

#include "VertexT1SwapLocationsWriter.hpp"
#include "AbstractCellPopulation.hpp"
#include "MeshBasedCellPopulation.hpp"
#include "CaBasedCellPopulation.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "PottsBasedCellPopulation.hpp"
#include "VertexBasedCellPopulation.hpp"

template <unsigned ELEMENT_DIM, unsigned SPACE_DIM>
VertexT1SwapLocationsWriter<ELEMENT_DIM, SPACE_DIM>::VertexT1SwapLocationsWriter()
    : AbstractCellPopulationWriter<ELEMENT_DIM, SPACE_DIM>("T1SwapLocations.dat")
{
}

template <unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void VertexT1SwapLocationsWriter<ELEMENT_DIM, SPACE_DIM>::Visit(MeshBasedCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation)
{
}

template <unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void VertexT1SwapLocationsWriter<ELEMENT_DIM, SPACE_DIM>::Visit(CaBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
}

template <unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void VertexT1SwapLocationsWriter<ELEMENT_DIM, SPACE_DIM>::Visit(NodeBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
}

template <unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void VertexT1SwapLocationsWriter<ELEMENT_DIM, SPACE_DIM>::Visit(PottsBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
}

template <unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void VertexT1SwapLocationsWriter<ELEMENT_DIM, SPACE_DIM>::Visit(VertexBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
    std::vector< c_vector<double, SPACE_DIM> > t1_swap_locations = pCellPopulation->rGetMesh().GetLocationsOfT1Swaps();

    *this->mpOutStream << t1_swap_locations.size() << "\t";
    for (unsigned index = 0;  index < t1_swap_locations.size(); index++)
    {
        for (unsigned i=0; i<SPACE_DIM; i++)
        {
            *this->mpOutStream << t1_swap_locations[index][i] << "\t";
        }
    }

    pCellPopulation->rGetMesh().ClearLocationsOfT1Swaps();
}

// Explicit instantiation
template class VertexT1SwapLocationsWriter<1,1>;
template class VertexT1SwapLocationsWriter<1,2>;
template class VertexT1SwapLocationsWriter<2,2>;
template class VertexT1SwapLocationsWriter<1,3>;
template class VertexT1SwapLocationsWriter<2,3>;
template class VertexT1SwapLocationsWriter<3,3>;

#include "SerializationExportWrapperForCpp.hpp"
// Declare identifier for the serializer
EXPORT_TEMPLATE_CLASS_ALL_DIMS(VertexT1SwapLocationsWriter)
