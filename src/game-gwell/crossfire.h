#pragma once

#include <array>
#include <functional>
#include <optional>
#include <span>

namespace crossfire {

using Entity = unsigned int;

const Entity INVALID_ENTITY = 0 - 1;

enum class Heading : unsigned int {
  Up = 0,
  Down = 1,
  Left = 2,
  Right = 3,
  Stopped = 4
};

using Coordinate = int;

struct CurrentPosition {
  Heading heading;
  Coordinate x;
  Coordinate y;
};

enum class Velocity : unsigned int {
  Normal = 0,
  ThreeQuarter = 1,
  Half = 2,
  Quarter = 3,
  Double = 4,
  Triple = 5,
  Quadruple = 6
};


namespace gametime {
    using Ticks = std::uint64_t;
    void on_logic_event();
    Ticks get_ticks();
    Ticks elapsed_ticks(Ticks value);
    std::int64_t tick_to_ms(Ticks value);

}

namespace entities {

using Team = unsigned char; 

Entity create(Team team);
Team get_team(Entity id);
bool is_same_team(Entity leftid, Entity rightid);

Entity get_environment();
bool is_environment(Entity id);
void remove(Entity id);
} // namespace entities

namespace linear {

struct EntityAndData {
  const std::span<const Entity, std::dynamic_extent> entities;
  const std::span<const CurrentPosition, std::dynamic_extent> positions;

  const auto size() const { return entities.size(); };
};

void changeHeading(Entity id, Heading heading, Velocity vel = Velocity::Normal);
void changePosition(Entity id, Coordinate x, Coordinate y);
void force_entity_onto_map(Entity id);
void create(Entity id, Heading heading, Coordinate x, Coordinate y,
            Velocity vel = Velocity::Normal);
void remove(Entity id);

void run(float delta);
EntityAndData getEntitiesPositions();

void endFrame();

// onEntityStopped()
} // namespace linear

namespace missle_shooter {
void create(Entity id, unsigned int maxLivingMissles, unsigned int delay);
void fireMissle(Entity id);

void run(float delta);

std::optional<bool> canFireMissles(Entity id);
std::optional<unsigned int> nbrMisslesLeft(Entity id);
} // namespace missle_shooter

namespace actions {
enum class Actions {
  no_action = 0,
  turn_right,
  turn_left,
  turn_up,
  turn_down,
  fire_missle,
  fire_special
};

void do_action(Entity id, Actions action);

void turn_right(Entity id);
void turn_left(Entity id);
void turn_up(Entity id);
void turn_down(Entity id);
void fire_missle(Entity id);
void fire_special(Entity id);
} // namespace actions

namespace keyboardinput {

const std::size_t MAX_LOCAL_PLAYERS{4};
struct KeyBoardMapping {};
// Consolidate Input per Entity

void intialize();
void on_logic_tick();

void create(Entity id, const KeyBoardMapping &map);
void remove(Entity id);

void on_turn_right(Entity id);
void on_turn_left(Entity id);
void on_turn_up(Entity id);
void on_turn_down(Entity id);
void on_fire_missle(Entity id);
void on_fire_special(Entity id);

} // namespace keyboardinput

namespace missle_shooter {

    void update_missle_shooter(Entity id, unsigned int max_missles, unsigned int delay_ms);

    void fire(Entity owner, CurrentPosition pos);




} // namespace missle

namespace collider {
struct Collision {
  Entity id1;
  Entity id2;
  std::size_t collisionId;
};

void do_collisions(const linear::EntityAndData &data);
std::span<const Collision> get_collisions();
void add_static_collider(Entity id, Coordinate x, Coordinate y, Coordinate x2,
                         Coordinate y2);

} // namespace collider

namespace collision_behavior {
using Behavior = int;

void run(std::span<const crossfire::collider::Collision> collisions);

Behavior create(std::function<void(Entity self, Entity other)> callback);
void set_entity(Entity id, Behavior b);

} // namespace collision_behavior

namespace map {

struct path {
  int x;
  int y;
  int x2;
  int y2;
};

struct ValidateHeadingChange {
  bool canChangeHeading{false};
  Coordinate newX{0};
  Coordinate newY{0};

  operator bool() { return canChangeHeading; }
};

constexpr const int fudgeFactor{3};
constexpr const int nbrRows{8};
constexpr const int nbrCols{8};
constexpr const int corridorWidth{10};
constexpr const int spaceBetween = corridorWidth * 2;
constexpr const int startingX{0};
constexpr const int startingY{0};

std::array<path, nbrRows + nbrCols> getPaths();
path getRowPath(int x = 0, int y = 0);
path getColPath(int x = 0, int y = 0);
constexpr const path getMapExtents() {
  return {0, 0, ((nbrRows - 1) * 2 * corridorWidth + corridorWidth),
          ((nbrCols - 1) * 2 * corridorWidth + corridorWidth)};
}

ValidateHeadingChange
isValidIntersectionToChangeHeading(Heading current, Coordinate x, Coordinate y);

} // namespace map

} // namespace crossfire