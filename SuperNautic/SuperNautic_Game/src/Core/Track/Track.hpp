#pragma once
#ifndef TRACK_HPP
#define TRACK_HPP

#include <glm/mat4x4.hpp>
#include <vector>

#include "Core/Track/Segment.hpp"
#include "Core/Track/SegmentHandler.hpp"
#include "Core/Track/SegmentInstance.hpp"

#include "GFX/Rendering/ViewportPipeline.hpp"

#include "Core/Geometry/Octree.hpp"
#include "Core/Track/ObstacleHandler.hpp"
#include "GFX/Resources/Window.hpp"

#include "GFX/Resources/TemperatureZone.hpp"

class Track {
 public:
  static const unsigned int _MIN_LENGTH;
  static const unsigned int _MAX_LENGTH;

  static const unsigned int _MIN_CURVINESS;
  static const unsigned int _MAX_CURVINESS;

  static const unsigned int _MIN_DIFFICULTY;
  static const unsigned int _MAX_DIFFICULTY;

  Track(SegmentHandler* segmentHandler, ObstacleHandler* obstacleHandler);
  virtual ~Track();
  int getTargetLength() const;
  int getGeneratedLength() const;
  unsigned int getCurviness() const;
  unsigned int getDifficulty() const;
  void setLength(const unsigned int length);
  void setSeed(const std::string& seed);
  const std::string& getSeed() const;
  void setCurviness(const unsigned int curviness);
  void setDifficulty(const unsigned int difficulty);
  void startNewTrack();
  void setNrOfPlayers(unsigned int nrOfPlayers);
  bool bGenerate();
  int getNrOfSegments() const;
  SegmentInstance* getInstance(int index);
  float getProgression() const;
  void render(GFX::ViewportPipeline& pipeline, const unsigned int playerIndex,
              const unsigned int shipIndex);
  void update(const float dt, const std::vector<unsigned int> playerIndexes);

  // Test collision against octree
  bool bTestCollision(const CollisionMesh& mesh) const;

  // Returns the forward vector for a given ship position and segment index
  // (segment index may update). Returns appropriate respawn position in
  // returnPos and length from start of segment
  glm::vec3 Track::findForward(const glm::vec3 globalPosition,
                               unsigned int& segmentIndex, glm::vec3& returnPos,
                               float& lengthInSegment,
                               glm::vec3& directionDifference);

 private:
  int getIndex() const;
  int getInRow(const int index) const;
  WaypointInfo findNextWaypointInfo(const WaypointInfo& current,
                                    unsigned int segmentIndex) const;
  bool bInsertNormalSegment(const int index);
  void insertStructure(const int index);
  void addWindowsAndZonesToSegment(const Segment* segment);
  void deleteSegments(const float lengthToDelete);
  bool bEndTrack();
  void placeObstacles();
  void placeDarkAreas();
  size_t findTrackIndex(const float totalLength,
                        float& lastFullSegmentLength) const;
  bool bInsertIntoOctree(SegmentInstance* segment);

  struct DarkAreaPlayerInfo {
    float factor;
    int startIndex;
    unsigned int length;
  };
  struct DarkArea {
    unsigned int startIndex;
    unsigned int length;
    float timer;
  };

  std::vector<GFX::Window> _segmentWindows;
  std::vector<GFX::TemperatureZone> _temperatureZones;
  SegmentHandler* _segmentHandler;
  ObstacleHandler* _obstacleHandler;
  std::vector<SegmentInstance*> _track;
  std::string _seed;
  float _curviness;
  float _difficulty;
  float _targetLength;
  float _generatedLength;
  float _totalProgress;
  float _lengthAfterLastCall;
  const float _progressionLength;
  const float _endMargin;
  glm::mat4 _endMatrix;
  char _endConnection;
  int _prevIndex;
  const Segment* _lastSegment;
  float _deletionLength;
  float _lengthWithCurrentConnectionType;
  std::unique_ptr<Octree<SegmentInstance*>> _octree;
  std::vector<DarkArea> _darkAreas;
  std::vector<DarkAreaPlayerInfo> _darkAreaPlayerValues;
};

#endif  // !TRACK_HPP
