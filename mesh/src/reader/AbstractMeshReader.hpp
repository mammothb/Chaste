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


#ifndef _ABSTRACTMESHREADER_HPP_
#define _ABSTRACTMESHREADER_HPP_

#include <string>
#include <vector>
#include <set>
#include <cassert>
#include <boost/iterator/iterator_facade.hpp>
#include "Exception.hpp"

/**
 * Helper structure that stores the nodes and any attribute value
 * associated with an Element.
 */
struct ElementData
{
    // Constructor to initialise default values, to prevent -Werror=maybe-uninitialized on some compilers
    ElementData()
            : NodeIndices(std::vector<unsigned>()),
              AttributeValue(DOUBLE_UNSET),
              ContainingElement(UNSIGNED_UNSET) {}
    std::vector<unsigned> NodeIndices; /**< Vector of Node indices owned by the element. */
    double AttributeValue; /**< Attribute value associated with the element. */
    unsigned ContainingElement; /**< Only applies to boundary elements: which element contains this boundary element. Only set if reader called with correct params */
};

/**
 * An abstract mesh reader class. Reads output generated by a mesh generator
 * and converts it to a standard format for use in constructing a finite
 * element mesh structure.
 *
 * A derived class TrianglesMeshReader exists for reading meshes generated
 * by Triangles (in 2-d) and TetGen (in 3-d).
 *
 * A derived class MemfemMeshReader reads 3D data from the Tulane University code
 *
 * A derived class FemlabMeshReader reads 2D data from Femlab or Matlab PDEToolbox
 */
template <unsigned ELEMENT_DIM, unsigned SPACE_DIM>
class AbstractMeshReader
{

public:

    virtual ~AbstractMeshReader()
    {}

    /** @return the number of elements in the mesh */
    virtual unsigned GetNumElements() const =0;

    /** @return the number of nodes in the mesh */
    virtual unsigned GetNumNodes() const =0;

    /** @return the number of faces in the mesh (also has synonym GetNumEdges()) */
    virtual unsigned GetNumFaces() const =0;

    /** @return the number of cable elements in the mesh */
    virtual unsigned GetNumCableElements() const;

    /** @return the number of element attributes in the mesh */
    virtual unsigned GetNumElementAttributes() const;

    /** @return the number of face attributes in the mesh */
    virtual unsigned GetNumFaceAttributes() const;

    /** @return the number of cable element attributes in the mesh */
    virtual unsigned GetNumCableElementAttributes() const;

    /**
     * @return the vector of node attributes
     * @return an empty vector here. Over-ride in child classes if needed.
     * Ideally, this method would be in AbstractCachedMeshReader (where it would return the cached attribuites)
     * but TrianglesMeshReader (the class this method was created for)
     * does not inherit from AbstractCachedMeshReader, so it needs to be here.
     */
    virtual std::vector<double> GetNodeAttributes();

    /** @return the number of edges in the mesh (synonym of GetNumFaces()) */
    unsigned GetNumEdges() const;

    /** @return a vector of the coordinates of each node in turn */
    virtual std::vector<double> GetNextNode()=0;

    /** Resets pointers to beginning*/
    virtual void Reset()=0;

    /** @return a vector of the node indices of each element (and any attribute information, if there is any) in turn */
    virtual ElementData GetNextElementData()=0;

    /** @return a vector of the node indices of each face (and any attribute/containment information, if there is any) in turn */
    virtual ElementData GetNextFaceData()=0;

    /** @return a vector of the node indices of each cable element (and any attribute information, if there is any) in turn */
    virtual ElementData GetNextCableElementData();

    /** @return a vector of the node indices of each edge (and any attribute/containment information, if there is any) in turn (synonym of GetNextFaceData()) */
    ElementData GetNextEdgeData();


    /**
     *  Normally throws an exception.  Only implemented for tetrahedral mesh reader of binary files.
     *
     * @param index  The global node index
     * @return a vector of the coordinates of the node
     */
     virtual std::vector<double> GetNode(unsigned index);

    /**
     *  Normally throws an exception.  Only implemented for tetrahedral mesh reader of binary files.
     *
     * @param index  The global element index
     * @return a vector of the node indices of the element (and any attribute information, if there is any)
     */
    virtual ElementData GetElementData(unsigned index);

    /**
     *  Normally throws an exception.  Only implemented for tetrahedral mesh reader of binary files.
     *
     * @param index  The global face index
     * @return a vector of the node indices of the face (and any attribute/containment information, if there is any)
     */
    virtual ElementData GetFaceData(unsigned index);

    /**
     *  Synonym of GetFaceData(index)
     *
     * @param index  The global edge index
     * @return a vector of the node indices of the edge (and any attribute/containment information, if there is any)
     */
    ElementData GetEdgeData(unsigned index);

    /**
     *  Normally throws an exception.  When implemented by derived classes, returns a list of the elements
     *  that contain the node (only available for binary files).
     *
     * @param index  The global node index
     * @return a vector of the node indices of the face (and any attribute/containment information, if there is any)
     */
    virtual std::vector<unsigned> GetContainingElementIndices(unsigned index);

    /**
     * @return the base name (less any extension) for mesh files.  Only implemented for some mesh types.
     */
    virtual std::string GetMeshFileBaseName();

    /**
     * @return the expected order of the element file (1=linear, 2=quadratic)
     */
    virtual unsigned GetOrderOfElements();

    /**
     * @return the expected order of the boundary element file (1=linear, 2=quadratic)
     */
    virtual unsigned GetOrderOfBoundaryElements();

    /**
     * @return true if the boundary element file is linear, but contains information about neighbouring elements
     */
    virtual bool GetReadContainingElementOfBoundaryElement();


    /**
     * @return true if reading binary files, false if reading ascii files.
     *
     * Note, this will always return false unless over-ridden by a derived class that is able to support binary file
     * formats.
     */
    virtual bool IsFileFormatBinary();

    /**
     * @return true if there is a node connectivity list (NCL) file available.
     *
     * Note, this will always return false unless over-ridden by a derived class that is able to support NCL files.
     *
     */
    virtual bool HasNclFile();

    /**
     * @return true if there is a node permutation applied.
     *
     * Note, this will always return false unless over-ridden by a derived class that is able to support NCL files.
     *
     */
    virtual bool HasNodePermutation();

    /**
     * @return the node permutation if a node permutation has been applied to this reader (or an empty permutation)
     *
     * Note, this will always throw an exception unless over-ridden by a derived class that is able to support NCL files.
     *
     */
    virtual const std::vector<unsigned>& rGetNodePermutation();


    // Iterator classes

    /**
     * An iterator class for element data.
     */
    class ElementIterator : public boost::iterator_facade<ElementIterator, const ElementData,
                                                          boost::single_pass_traversal_tag>
    {
    public:
        /**
         * Constructor for pointing to a specific item.
         *
         * Note that, in the case of an ASCII mesh file, this will actually
         * start wherever the file pointer currently is.  The user is responsible
         * for resetting the reader prior to creating an iterator.
         *
         * @param index  the index of the item to point at
         * @param pReader  the mesh reader to iterate over
         */
        ElementIterator(unsigned index, AbstractMeshReader<ELEMENT_DIM, SPACE_DIM>* pReader)
            : mIndex(index),
              mpIndices(nullptr),
              mpReader(pReader)
        {
            CacheData(mIndex, true);
        }

        /**
         * Constructor for iterating over a subset of the items in the mesh.
         *
         * @param rIndices  a set of item indices over which to iterate
         * @param pReader  the mesh reader to iterate over
         *
         */
        ElementIterator(const std::set<unsigned>& rIndices,
                        AbstractMeshReader<ELEMENT_DIM, SPACE_DIM>* pReader);

        /**
         * @return the index of the item pointed at.
         */
        unsigned GetIndex() const
        {
            return mIndex;
        }

    private:
        friend class boost::iterator_core_access;

        /**
         * Read the pointed-at item data (if we're pointing at anything) and cache it
         * within the iterator, for use in dereference.
         *
         * @param index  the item to read
         * @param firstRead  Set to true in the constructor.  This ensures that the line 0 is always read
         */
        void CacheData(unsigned index, bool firstRead = false);

        /**
         * Increment the iterator to point at the next item in the file.
         */
        void increment();

        /**
         * @return true if two iterators point at the same item.
         * @param rOther  the other iterator
         */
        bool equal(const ElementIterator& rOther) const
        {
            return mIndex == rOther.mIndex;
        }

        /**
         * Dereference this iterator to get the data for the item pointed at.
         *
         * Note that the returned reference is only valid for as long as this iterator
         * is pointing at the item.
         * @return reference
         */
        const ElementData& dereference() const
        {
            assert(mpReader);
            assert(mIndex < mpReader->GetNumElements());
            // This was cached in increment()
            return mLastDataRead;
        }

        /** The index of the item pointed at. */
        unsigned mIndex;

        /** The set which we're iterating over, if we are iterating over a subset of the items. */
        const std::set<unsigned>* mpIndices;

        /** Iterator over the indices in that subset. */
        std::set<unsigned>::const_iterator mIndicesIterator;

        /** The mesh reader we're iterating over. */
        AbstractMeshReader<ELEMENT_DIM, SPACE_DIM>* mpReader;

        /** Data for the last item read. */
        ElementData mLastDataRead;
    };

    /**
     * @return an iterator to the first element in the file.
     *
     * Note that, in the case of an ASCII mesh file, for efficiency this will actually
     * start wherever the file pointer currently is.  The user is responsible
     * for resetting the reader prior to calling GetElementIteratorBegin().
     */
    ElementIterator GetElementIteratorBegin();

    /**
     * @return an iterator over a set of elements whose indices are given
     *
     * @param rIndices  subset of indices
     *
     * Note that, in the case of an ASCII mesh file, for efficiency this will actually
     * start wherever the file pointer currently is.  The user is responsible
     * for resetting the reader prior to calling GetElementIteratorBegin().
     */
    ElementIterator GetElementIteratorBegin(const std::set<unsigned>& rIndices);

    /**
     * @return an iterator to (one past the) end of the element data.
     */
    ElementIterator GetElementIteratorEnd();


    /**
     * An iterator class for node data.
     */
    ///\todo #1930.  We could roll most iterator functionality into a base class here
    class NodeIterator : public boost::iterator_facade<NodeIterator, const std::vector<double>,
                                                       boost::single_pass_traversal_tag>
    {
    public:
        /**
         * Constructor for pointing to a specific item.
         *
         * Note that, in the case of an ASCII mesh file, this will actually
         * start wherever the file pointer currently is.  The user is responsible
         * for resetting the reader prior to creating an iterator.
         *
         * @param index  the index of the item to point at
         * @param pReader  the mesh reader to iterate over
         */
        NodeIterator(unsigned index, AbstractMeshReader<ELEMENT_DIM, SPACE_DIM>* pReader)
            : mIndex(index),
              mpIndices(nullptr),
              mpReader(pReader)
        {
            CacheData(mIndex, true);
        }

        /**
         * Constructor for iterating over a subset of the items in the mesh.
         *
         * @param rIndices  a set of item indices over which to iterate
         * @param pReader  the mesh reader to iterate over
         *
         */
        NodeIterator(const std::set<unsigned>& rIndices,
                     AbstractMeshReader<ELEMENT_DIM, SPACE_DIM>* pReader);

        /**
         * @return the index of the item pointed at.
         */
        unsigned GetIndex() const
        {
            return mIndex;
        }

    private:
        friend class boost::iterator_core_access;

        /**
         * Read the pointed-at item data (if we're pointing at anything) and cache it
         * within the iterator, for use in dereference.
         *
         * @param index  the item to read
         * @param firstRead  Set to true in the constructor.  This ensures that the line 0 is always read
         */
        void CacheData(unsigned index, bool firstRead = false);

        /**
         * Increment the iterator to point at the next item in the file.
         */
        void increment();

        /**
         * @return true if two iterators point at the same item.
         * @param rOther  the other iterator
         */
        bool equal(const NodeIterator& rOther) const
        {
            return mIndex == rOther.mIndex;
        }

        /**
         * Dereference this iterator to get the data for the item pointed at.
         * @return reference
         * Note that the returned reference is only valid for as long as this iterator
         * is pointing at the item.
         */
        const std::vector<double>& dereference() const
        {
            assert(mpReader);
            assert(mIndex < mpReader->GetNumNodes());
            // This was cached in increment()
            return mLastDataRead;
        }

        /** The index of the item pointed at. */
        unsigned mIndex;

        /** The set which we're iterating over, if we are iterating over a subset of the items. */
        const std::set<unsigned>* mpIndices;

        /** Iterator over the indices in that subset. */
        std::set<unsigned>::const_iterator mIndicesIterator;

        /** The mesh reader we're iterating over. */
        AbstractMeshReader<ELEMENT_DIM, SPACE_DIM>* mpReader;

        /** Data for the last item read. */
        std::vector<double> mLastDataRead;
    };

    /**
     * @return an iterator to the first node in the file.
     *
     * Note that, in the case of an ASCII mesh file, for efficiency this will actually
     * start wherever the file pointer currently is.  The user is responsible
     * for resetting the reader prior to calling GetNodeIteratorBegin().
     */
    NodeIterator GetNodeIteratorBegin();

    /**
     * @return an iterator over a set of nodes whose indices are given
     *
     * @param rIndices  subset of indices
     *
     * Note that, in the case of an ASCII mesh file, for efficiency this will actually
     * start wherever the file pointer currently is.  The user is responsible
     * for resetting the reader prior to calling GetNodeIteratorBegin().
     */
    NodeIterator GetNodeIteratorBegin(const std::set<unsigned>& rIndices);

    /**
     * @return an iterator to (one past the) end of the node data.
     */
    NodeIterator GetNodeIteratorEnd();
};

#endif //_ABSTRACTMESHREADER_HPP_
