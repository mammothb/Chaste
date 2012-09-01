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

#ifndef CYLINDRICAL2DVERTEXMESH_HPP_
#define CYLINDRICAL2DVERTEXMESH_HPP_

#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>

#include "MutableVertexMesh.hpp"

/**
 * A subclass of MutableVertexMesh<2,2> for a rectangular mesh with
 * periodic left and right boundaries, representing a cylindrical geometry.
 *
 * The class works by overriding calls such as ReMesh() and
 * GetVectorFromAtoB() so that simulation classes can treat this
 * class in exactly the same way as a MutableMesh<2,2>.
 */
class Cylindrical2dVertexMesh : public MutableVertexMesh<2,2>
{
    friend class TestCylindrical2dVertexMesh;

private:

    /** The circumference of the cylinder. */
    double mWidth;

    /** Needed for serialization. */
    friend class boost::serialization::access;
    /**
     * Archives the member variables of the object which
     * have to be preserved during its lifetime.
     *
     * The remaining member variables are re-initialised before being used
     * by each ReMesh() call so they do not need to be archived.
     *
     * @param archive the archive
     * @param version the current version of this class
     */
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<MutableVertexMesh<2,2> >(*this);
        archive & mWidth;
    }

    /**
     * Constructor - used for serialization only.
     */
    Cylindrical2dVertexMesh();

public:

    /**
     * Default constructor.
     *
     * @param width the width (circumference) of the mesh
     * @param nodes vector of pointers to nodes
     * @param vertexElements vector of pointers to VertexElements
     * @param cellRearrangementThreshold the minimum threshold distance for element rearrangement (defaults to 0.01)
     * @param t2Threshold the maximum threshold distance for Type 2 swaps (defaults to 0.001)
     */
    Cylindrical2dVertexMesh(double width,
                            std::vector<Node<2>*> nodes,
                            std::vector<VertexElement<2,2>*> vertexElements,
                            double cellRearrangementThreshold=0.01,
                            double t2Threshold=0.001);

    /**
     * Destructor.
     */
    ~Cylindrical2dVertexMesh();

    /**
     * Overridden GetVectorFromAtoB() method.
     * This method evaluates the (surface) distance between
     * two points in a 2D cylindrical geometry.
     *
     * @param rLocation1 the x and y co-ordinates of point 1
     * @param rLocation2 the x and y co-ordinates of point 2
     *
     * @return the vector from location1 to location2
     */
    c_vector<double, 2> GetVectorFromAtoB(const c_vector<double, 2>& rLocation1, const c_vector<double, 2>& rLocation2);

    /**
     * Overridden SetNode() method.
     *
     * If the location should be set outside a cylindrical boundary
     * move it back onto the cylinder.
     *
     * @param nodeIndex is the index of the node to be moved
     * @param point is the new target location of the node
     */
    void SetNode(unsigned nodeIndex, ChastePoint<2> point);

    /**
     * Overridden GetWidth() method.
     *
     * Calculate the 'width' of any dimension of the mesh, taking periodicity
     * into account.
     *
     * @param rDimension a dimension (0 or 1)
     * @return The maximum distance between any nodes in this dimension.
     */
    double GetWidth(const unsigned& rDimension) const;

    /**
     * Overridden AddNode() method.
     *
     * @param pNewNode the node to be added to the mesh
     *
     * @return the global index of the new node
     */
    unsigned AddNode(Node<2>* pNewNode);

    /**
     * Overridden GetVolumeOfElement() method.
     *
     * @param index  the global index of a specified vertex element
     *
     * @return the area of the element
     */
    double GetVolumeOfElement(unsigned index);

    /**
     * Overridden GetCentroidOfElement() method.
     *
     * @param index  the global index of a specified vertex element
     *
     * @return (centroid_x,centroid_y).
     */
     c_vector<double, 2> GetCentroidOfElement(unsigned index);
};

#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(Cylindrical2dVertexMesh)

#endif /*CYLINDRICAL2DVERTEXMESH_HPP_*/
