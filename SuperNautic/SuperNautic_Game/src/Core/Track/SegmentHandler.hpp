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
		float curviness;
		int probability1;
		int probability2;
		int minInRow;
		int maxInRow;
		std::vector<StructurePiece*> pieces;

		Structure(float curviness, int prob1, int prob2, int minRow, int maxRow)
		{
			this->curviness = curviness;
			probability1 = prob1;
			probability2 = prob2;
			minInRow = minRow;
			maxInRow = maxRow;
		}
		void addPiece(StructurePiece * newPiece)
		{
			pieces.push_back(newPiece);
		}
		int getProbability(const float p) const
		{
			float realCurviness = (p - curviness) / (1 - curviness);
			return int((1 - realCurviness) * probability1 + realCurviness * probability2);
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
	const Segment* loadSegment(unsigned int i);

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
