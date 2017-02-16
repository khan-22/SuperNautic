#pragma once
#ifndef SEGMENTHANDLER_HPP
#define SEGMENTHANDLER_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include "Core/Track/Segment.hpp"

// Holds uninstantiated versions of track segments
class SegmentHandler
{
public:
	struct StructurePiece
	{
		int index;
		int minRotation;
		int maxRotation;

		StructurePiece(int i, int minR, int maxR)
		{
			index = i;
			minRotation = minR;
			maxRotation = maxR;
		}
	};
	struct Structure
	{
		int probability;
		int minInRow;
		int maxInRow;
		bool bAllowNegativRot;
		std::vector<StructurePiece*> pieces;

		Structure(int prob, int minRow, int maxRow, bool bAllowNegativRot)
		{
			probability = prob;
			minInRow = minRow;
			maxInRow = maxRow;
			this->bAllowNegativRot = bAllowNegativRot;
		}
		void addPiece(StructurePiece * newPiece)
		{
			pieces.push_back(newPiece);
		}
	};

	// Loads SegmentInfos from file with name fileName in path
	SegmentHandler(std::string segmentInfoPath, std::string connectionInfoPath);

	// Destructor
	virtual ~SegmentHandler();


	// Returns reference to segment infos
	const std::vector<SegmentInfo>& infos() const
	{
		return _segmentInfos;
	}

	// Load segment from fbx file using info from _segments[i], returns reference to loaded segment
	// Just returns reference if already loaded
	const Segment* loadSegment(unsigned i);

	const SegmentHandler::Structure * getStructure(const unsigned int index) const;

	const int getNrOfStructures() const;

	// Returns rotation info about connection type 'type'
	int getConnectionRotation(const char type);

private:
	// Segments that have been loaded from an fbx file
	std::vector<Segment*>		_segments;

	// Basic info about all possible segments, loaded from a txt file
	std::vector<SegmentInfo>	_segmentInfos;

	std::vector<Structure>		_structures;

	// Keeps track of connection types
	std::unordered_map<char, int>	_connections;

	// Path to res\models
	static std::string basePath;
};

#endif // SEGMENTHANDLER_H
