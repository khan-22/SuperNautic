#include <fstream>
#include <limits>

#include "Core/Track/SegmentHandler.hpp"
#include "Core/Io/Log.hpp"

// Path to res\models
std::string SegmentHandler::basePath = "res/models/";

// Loads SegmentInfos and connections from file (relative to res/models)
SegmentHandler::SegmentHandler(std::string segmentInfoPath, std::string connectionInfoPath)
{
	//segmentInfoPath = "segmentinfos.txt";
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
	for (unsigned int i = 0; i < amount; i++)
	{
		// File names
		std::string segmentDataName, segmentVisualName;
		infoFile >> segmentDataName;
		infoFile >> segmentVisualName;

		// Connection types
		char startConnection, endConnection;
		// blah_aa.blend
		//      ^^
		// size - 8, size - 7 contains connection info
		startConnection = segmentDataName[segmentDataName.size() - 8];
		endConnection = segmentDataName[segmentDataName.size() - 7];

		// Metadata about segment
		int probability, minInRow, maxInRow, rotationOffset;
		infoFile >> probability;
		infoFile >> minInRow;
		infoFile >> maxInRow;
		infoFile >> rotationOffset;

		// Add SegmentInfo to vector
		_segmentInfos.push_back(SegmentInfo{ std::move(segmentDataName), std::move(segmentVisualName)
			, startConnection, endConnection, probability, minInRow, maxInRow, rotationOffset });

		// Skip rest of line
		infoFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	// Load structures
	infoFile >> amount;
	for (unsigned int i = 0; i < amount; i++)
	{
		int probability, minInRow, maxInRow, pieces;
		infoFile >> probability;
		infoFile >> minInRow;
		infoFile >> maxInRow;
		infoFile >> pieces;
		_structures.push_back(Structure(probability, minInRow, maxInRow));
		for (unsigned int j = 0; j < pieces; j++)
		{
			int index, minRotation, maxRotation;
			infoFile >> index;
			infoFile >> minRotation;
			infoFile >> maxRotation;
			_structures[i].addPiece(new StructurePiece(index, minRotation, maxRotation));
		}
	}

	infoFile.close();


	// Read connections
	infoFile.open(basePath + connectionInfoPath);
	if (!infoFile.is_open())
	{
		// File could not be opened
		LOG_ERROR("Could not open SegmentInfo file ", basePath + connectionInfoPath);
		return;
	}

	char type;
	int rotation;
	infoFile >> amount;
	for (unsigned int i = 0; i < amount; i++)
	{
		infoFile >> type;
		infoFile >> rotation;
		std::pair<char, int> temp(type, rotation);
		_connections.insert(temp);
	}
	infoFile.close();
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
const Segment* SegmentHandler::loadSegment(unsigned i)
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

// Returns rotation info about connection type 'type'
int SegmentHandler::getConnectionRotation(const char type)
{
	return _connections[type];
}
