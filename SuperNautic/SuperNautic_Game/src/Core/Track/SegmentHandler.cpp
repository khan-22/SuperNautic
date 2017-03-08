#include <fstream>
#include <limits>

#include "Core/Track/SegmentHandler.hpp"
#include "Core/Io/Log.hpp"

// Path to res\models
std::string SegmentHandler::basePath = "res/models/";

// Loads SegmentInfos and connections from file (relative to res/models)
SegmentHandler::SegmentHandler(std::string segmentInfoPath, std::string connectionInfoPath)
{
	// Read SegmentInfos
	std::ifstream infoFile { basePath + segmentInfoPath };
	if (!infoFile.is_open())
	{
		// File could not be opened
		LOG_ERROR("Could not open SegmentInfo file ", basePath + segmentInfoPath);
		return;
	}

	// Load segments
	int amount;
	infoFile >> amount;
	for (int i = 0; i < amount; i++)
	{
		// File names
		std::string segmentDataName, segmentVisualName, materialName, segmentWindowName, segmentZoneName;
		infoFile >> segmentDataName >> segmentVisualName >> materialName >> segmentWindowName >> segmentZoneName;

		// Connection types
		char startConnection, endConnection;
		// blah_aa.kmf
		//      ^^
		// size - 6, size - 5 contains connection info
		startConnection = segmentDataName[segmentDataName.size() - 6];
		endConnection = segmentDataName[segmentDataName.size() - 5];

		// Low curviness data
		int probability1, minInRow1, maxInRow1, rotationOffset1;
		infoFile >> probability1 >> minInRow1 >> maxInRow1 >> rotationOffset1;
		// High curviness data
		int probability2, minInRow2, maxInRow2, rotationOffset2;
		infoFile >> probability2 >> minInRow2 >> maxInRow2 >> rotationOffset2;

		// Add SegmentInfo to vector
		_segmentInfos.push_back(SegmentInfo{ std::move(segmentDataName), std::move(segmentVisualName)
			, std::move(materialName), std::move(segmentWindowName), std::move(segmentZoneName)
			, startConnection, endConnection, probability1, probability2, minInRow1, minInRow2
			, maxInRow1, maxInRow2, rotationOffset1, rotationOffset2 });

		// Skip rest of line
		infoFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	// Load structures
	infoFile >> amount;
	for (int i = 0; i < amount; i++)
	{
		float curviness;
		int probability1, probability2, minInRow, maxInRow, pieces;
		infoFile >> curviness >> probability1 >> probability2 >> minInRow >> maxInRow >> pieces;
		_structures.push_back(Structure(curviness, probability1, probability2, minInRow, maxInRow));
		for (int j = 0; j < pieces; j++)
		{
			int index, minRotation, maxRotation;
			infoFile >> index >> minRotation >> maxRotation;
			_structures[i].addPiece(new StructurePiece(index, minRotation, maxRotation));
		}
		// Skip rest of line
		infoFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	infoFile.close();


	// Read connections
	infoFile.open(basePath + connectionInfoPath);
	if (!infoFile.is_open())
	{
		// File could not be opened
		LOG_ERROR("Could not open ConnectionInfo file ", basePath + connectionInfoPath);
		return;
	}

	char type;
	int rotation;
	infoFile >> amount;
	for (int i = 0; i < amount; i++)
	{
		infoFile >> type;
		infoFile >> rotation;
		std::pair<char, int> temp(type, rotation);
		_connections.insert(temp);
	}
	infoFile.close();

	// Load all segments
	for (size_t i = 0; i < _segmentInfos.size(); ++i)
	{
		loadSegment(static_cast<unsigned int>(i));
	}
}

// Destructor
SegmentHandler::~SegmentHandler()
{
	for (unsigned int i = 0; i < _segments.size(); i++)
	{
		delete _segments[i];
	}
	for (unsigned int i = 0; i < _structures.size(); i++)
	{
		for (unsigned int j = 0; j < _structures[i].pieces.size(); j++)
		{
			delete _structures[i].pieces[j];
		}
	}
}

// Load segment from fbx file using info from _segments[i], returns reference to loaded segment
// Just returns reference if already loaded
const Segment* SegmentHandler::loadSegment(unsigned int i)
{
	// If already loaded, just return reference
	if (_segmentInfos[i].loadedIndex != -1)
	{
		return _segments[_segmentInfos[i].loadedIndex];
	}
	// Else load Segment, then return
	else
	{
		// Load Segment from file
		_segments.push_back(new Segment(&_segmentInfos[i]));

		// Set index in corresponding SegmentInfo
		_segmentInfos[i].loadedIndex = static_cast<int>(_segments.size()) - 1;

		return _segments[_segmentInfos[i].loadedIndex];
	}
}

const SegmentHandler::Structure * SegmentHandler::getStructure(const unsigned int index) const
{
	return &_structures[index];
}

const int SegmentHandler::getNrOfStructures() const
{
	return static_cast<int>(_structures.size());
}

// Returns rotation info about connection type 'type'
int SegmentHandler::getConnectionRotation(const char type)
{
	return _connections[type];
}
