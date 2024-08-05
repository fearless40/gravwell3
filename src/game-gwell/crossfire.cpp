#include "crossfire.h"

#include <algorithm>
#include <array>
#include <optional>
#include <ranges>
#include <span>
#include <vector>

#include "../engine/Event.h"
#include "../engine/GameEvent.h"
#include "../engine/KeyMapper.hpp"
#include "../util/FixedFunctionFloat.h"
#include "../util/Math/RectT.h"
#include "../util/soa.hpp"

namespace soa = util::soa;

namespace crossfire::gametime {
using Ticks = std::uint64_t;

Ticks ticks{0};

void on_logic_event() { ++ticks; }
Ticks get_ticks() { return ticks; }
Ticks elapsed_ticks( Ticks value ) { return ticks - value; }
std::int64_t tick_to_ms( Ticks value ) { return value * 30; }
}


namespace crossfire::entities {

const Entity environment{1};
soa::SOA<soa::FixedArray<64>, Entity, Team> mSOA;

constinit Entity nextId = 1;

Entity create(Team team) {
  const auto id = ++nextId;
  mSOA.push_back(id, team);
  return id;
}

bool is_same_team(Entity leftid, Entity rightid) {
  if (leftid == rightid)
    return true;

  auto leftDataIt = mSOA.find<Entity>(leftid);
  auto rightDataIt = mSOA.find<Entity>(rightid);

  if (leftDataIt != mSOA.end() && rightDataIt != mSOA.end()) {
    return leftDataIt.get<Team>() == rightDataIt.get<Team>();
  }
}

Team get_team(Entity id) {
  auto leftDataIt = mSOA.find<Entity>(id);
  if (leftDataIt != mSOA.end())
    return leftDataIt.get<Team>();
}

Entity get_environment() { return environment; }

bool is_environment(Entity id) { return environment == id; }

void remove(Entity id) {}
} // namespace crossfire::entities

namespace crossfire::linear {

using coord = util::FixedFunctionFloat<4>;

struct Position {
  Heading heading;
  coord x;
  coord y;
  coord velocity;

  constexpr auto getX() const { return x.as<Coordinate>(); }
  constexpr auto getY() const { return y.as<Coordinate>(); }
};

using soa = util::soa::SOA<util::soa::FixedArray<128>, Entity, Position>;
soa m_arrays;

std::array<Entity, 128> entityids;
std::array<Position, 128> positions;
std::array<CurrentPosition, 128> visiblePositions;
std::size_t nbrEntitiesAndPositions = 0;

std::array<Entity, 64> flaggedForRemoval;
std::size_t nbrFlaggedForRemoval = 0;

struct DeltaXY {
  util::FixedFunctionMagnitude x;
  util::FixedFunctionMagnitude y;
};

const DeltaXY headingDelta[] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {0, 0}};

const coord Velocities[] = {coord{1}, coord{0.75f}, coord{0.5f}, coord{0.25f},
                            coord{2}, coord{3},     coord{4}};

constexpr auto asHeadingDelta(Heading head) {
  return headingDelta[static_cast<unsigned int>(head)];
}

constexpr auto asSpeed(Velocity vel) {
  return Velocities[static_cast<unsigned int>(vel)];
}

void changePosition(Entity id, Coordinate x, Coordinate y) {
  // auto found = find_first(id);
  auto foundIt = m_arrays.find<Entity>(id);

  if (foundIt != m_arrays.end()) {
    auto &pos = foundIt.get<Position>();
    pos.x = x;
    pos.y = y;
  }
}

void changeHeading(Entity id, Heading heading, Velocity vel) {
  // Happens immedietly should wait to the end of the current run
  auto found = m_arrays.find<Entity>(id);
  if (found != m_arrays.end()) {
    auto &pos = found.get<Position>();

    if (pos.heading == Heading::Up || pos.heading == Heading::Down) {
      if (heading == Heading::Up || heading == Heading::Down)
        return;
    }

    if (pos.heading == Heading::Left || pos.heading == Heading::Right) {
      if (heading == Heading::Left || heading == Heading::Right)
        return;
    }

    auto valid = map::isValidIntersectionToChangeHeading(
        pos.heading, pos.getX() + 5, pos.getY() + 5);
    if (valid) {
      pos.heading = heading;
      pos.x = coord{valid.newX};
      pos.y = coord{valid.newY};
      // pos.velocity = asSpeed(vel);
    }
  }
}

void create(Entity id, Heading heading, Coordinate x, Coordinate y,
            Velocity vel) {
  m_arrays.push_back(id, {heading, coord{x}, coord{y}, asSpeed(vel)});
}

void remove(Entity id) {
  flaggedForRemoval[nbrFlaggedForRemoval] = id;
  ++nbrFlaggedForRemoval;
}

void run(float delta) {
  auto update_positions = [](auto &pos) {
    auto delta = asHeadingDelta(pos.heading);
    pos.x = pos.x + (pos.velocity * delta.x);
    pos.y = pos.y + (pos.velocity * delta.y);

    /*pos.x += delta.x;\
    pos.y += delta.y;*/

    auto testx = pos.getX();
    auto testy = pos.getY();
    /*
    if (pos.getX() + 10 > extents.x2)
      pos.x = coord{extents.x2 - 10};
    if (pos.getX() < extents.x)
      pos.x = coord{extents.x};
    if (pos.getY() + 10 > extents.y2)
      pos.y = coord{extents.y2 - 10};
    if (pos.getY() < extents.y)
      pos.y = coord{extents.y};
      */
  };

  std::ranges::for_each(m_arrays.row_span<Position>(), update_positions);
}

void force_entity_onto_map(Entity id) {
  const auto extents{map::getMapExtents()};
  auto it = m_arrays.find<Entity>(id);
  if (it == m_arrays.end())
    return;

  auto &pos = it.get<Position>();

  if (pos.getX() + 10 > extents.x2)
    pos.x = coord{extents.x2 - 10};
  if (pos.getX() < extents.x)
    pos.x = coord{extents.x};
  if (pos.getY() + 10 > extents.y2)
    pos.y = coord{extents.y2 - 10};
  if (pos.getY() < extents.y)
    pos.y = coord{extents.y};
}

EntityAndData getEntitiesPositions() {
  std::transform(
      m_arrays.row_begin<Position>(), m_arrays.row_end<Position>(),
      visiblePositions.begin(), [](const auto &input) {
        return CurrentPosition{input.heading, input.getX(), input.getY()};
      });

  return {m_arrays.row_span<Entity>(),
          {visiblePositions.cbegin(), m_arrays.size()}};
}

void endFrame() {}
} // namespace crossfire::linear

namespace crossfire::map {

path getRowPath(int x, int y) {
  return {x, y, x + ((nbrRows - 1) * 2 * corridorWidth + corridorWidth),
          y + corridorWidth};
}

path getColPath(int x, int y) {
  return {x, y, corridorWidth + x,
          y + ((nbrCols - 1) * 2 * corridorWidth + corridorWidth)};
}

std::array<path, nbrRows + nbrCols> getPaths() {
  std::array<path, nbrRows + nbrCols> paths;

  unsigned count = 0;
  for (int row = 0; row < nbrRows; ++row) {
    paths[row] = getRowPath(0, count * corridorWidth);
    count += 2;
  };

  count = 0;
  for (int col = 0; col < nbrCols; ++col) {
    paths[col + nbrRows] = getColPath(count * corridorWidth, 0);
    count += 2;
  }

  return paths;
}

template <class T, class Compare = std::less_equal<>>
bool between(T const &val, T const &lo, T const &hi, Compare comp = Compare{}) {
  return comp(lo, val) && comp(val, hi);
}

ValidateHeadingChange isValidIntersectionToChangeHeading(Heading current,
                                                         Coordinate x,
                                                         Coordinate y) {
  auto xIntersection = std::div(x, corridorWidth * 2);
  auto yIntersection = std::div(y, corridorWidth * 2);

  // Find the remainder that is acceptable
  const int remainder = (corridorWidth / 4) % corridorWidth;

  bool okToChange = false;

  okToChange = between(xIntersection.rem, remainder - fudgeFactor,
                       remainder + fudgeFactor) &&
               between(yIntersection.rem, remainder - fudgeFactor,
                       remainder + fudgeFactor);

  if (okToChange) {
    return {true, xIntersection.quot * corridorWidth * 2,
            yIntersection.quot * corridorWidth * 2};
  }

  return {};
}

} // namespace crossfire::map

namespace crossfire::collider {
const std::size_t MAX_COLLISIONS{255};
const int ITEM_WIDTH = crossfire::map::corridorWidth;

std::array<Collision, MAX_COLLISIONS> lastCollisions;
std::size_t nbrCollisions;

struct AABB {
  int x, y;
  int x2, y2;
};

using soa = util::soa::SOA<util::soa::FixedArray<24>, Entity, AABB>;
soa m_static_colliders;

constexpr bool aabb_collision(const AABB &pos1, const AABB &pos2) noexcept {
  if (((pos1.x >= pos2.x && pos1.x <= pos2.x2) ||
       (pos1.x2 >= pos2.x && pos1.x2 <= pos2.x2)) &&
      ((pos1.y >= pos2.y && pos1.y <= pos2.y2) ||
       (pos1.y2 >= pos2.y && pos1.y2 <= pos2.y2))) {
    return true;
  }
  return false;
}

constexpr AABB make_AABB_from_position(const CurrentPosition &pos) {
  return {pos.x, pos.y, pos.x + ITEM_WIDTH, pos.y + ITEM_WIDTH};
}

void add_static_collider(Entity id, Coordinate x, Coordinate y, Coordinate x2,
                         Coordinate y2) {
  m_static_colliders.push_back(id, {x, y, x2, y2});
}

void do_static_collisions(const linear::EntityAndData &data) {
  auto static_aabbs = m_static_colliders.row_span<AABB>();
  for (std::size_t outerloop = 0; outerloop < data.size() - 1; ++outerloop) {
    const auto outerItemAABB =
        make_AABB_from_position(data.positions[outerloop]);

    for (std::size_t i = 0; i < static_aabbs.size(); ++i) {
      if (aabb_collision(outerItemAABB, static_aabbs[i])) {
        lastCollisions[nbrCollisions] = {data.entities[outerloop],
                                         m_static_colliders.view_at<Entity>(i),
                                         nbrCollisions};
        ++nbrCollisions;
      }
    }
  }
}

void do_collisions(const linear::EntityAndData &data) {
  nbrCollisions = 0;
  do_static_collisions(data);

  if (data.size() == 1)
    return;

  for (std::size_t outerloop = 0; outerloop < data.size() - 1; ++outerloop) {
    const auto outerItemAABB =
        make_AABB_from_position(data.positions[outerloop]);
    for (std::size_t innerloop = outerloop + 1; innerloop < data.size();
         ++innerloop) {
      if (aabb_collision(outerItemAABB,
                         make_AABB_from_position(data.positions[innerloop]))) {
        lastCollisions[nbrCollisions] = {
            data.entities[outerloop], data.entities[innerloop], nbrCollisions};
        ++nbrCollisions;
      }
    }
  }
}

std::span<const Collision> get_collisions() {
  return {lastCollisions.data(), nbrCollisions};
}

} // namespace crossfire::collider

namespace crossfire::collision_behavior {
// using Behavior = int;
using EntityList = std::vector<crossfire::Entity>;
using BehaviorCallback = std::function<void(crossfire::Entity behavior_on,
                                            crossfire::Entity collided_with)>;
using soa = util::soa::SOA<util::soa::FixedArray<24>, Behavior,
                           BehaviorCallback, EntityList>;
soa mSOA;

struct EntityToBehavior {
  crossfire::Entity entity;
  Behavior behavior;
};

std::vector<EntityToBehavior> m_entity_to_behavior_map;

void execute_behavior(crossfire::Entity behavior_entity,
                      crossfire::Entity collided_with) {
  auto map = std::find_if(
      m_entity_to_behavior_map.begin(), m_entity_to_behavior_map.end(),
      [behavior_entity](auto &item) { return item.entity == behavior_entity; });

  if (map != m_entity_to_behavior_map.end()) {
    auto behaviorIt = mSOA.find<Behavior>((*map).behavior);
    if (behaviorIt != mSOA.end()) {

      const auto &callback = behaviorIt.get<BehaviorCallback>();
      if (callback)
        callback(behavior_entity, collided_with);
    }
  }
}

void run(std::span<const crossfire::collider::Collision> collisions) {
  for (auto &collision : collisions) {
    execute_behavior(collision.id1, collision.id2);
    execute_behavior(collision.id2, collision.id1);
  }
}

Behavior create(BehaviorCallback callback) {
  const int current_pos = mSOA.size();
  mSOA.push_back(current_pos, callback, {});
  return current_pos;
}

void set_entity(crossfire::Entity id, Behavior b) {
  auto find_it = mSOA.find<Behavior>(b);
  if (find_it != mSOA.end()) {

    auto &[b, callback, entities] = *find_it;
    entities.push_back(b);
  }

  m_entity_to_behavior_map.emplace_back(id, b);
}

} // namespace crossfire::collision_behavior

namespace crossfire::keyboardinput {

const unsigned short VK_LEFT = 0x25;
const unsigned short VK_UP = 0x26;
const unsigned short VK_RIGHT = 0x27;
const unsigned short VK_DOWN = 0x28;
const unsigned short VK_SPACE = 0x20;

KeyMapper<void(Entity), Entity> mapper;

struct InputCachePerLogicEvent {
  Entity id{};
  actions::Actions movement{actions::Actions::no_action};
  actions::Actions special{actions::Actions::no_action};
};

std::array<InputCachePerLogicEvent, MAX_LOCAL_PLAYERS> inputCache;
std::size_t nbrPlayers{0};

std::optional<InputCachePerLogicEvent *> find_entity(Entity id) {
  for (std::size_t index = 0; index < nbrPlayers; ++index) {
    if (id == inputCache[index].id)
      return &inputCache[index];
  }
  return {};
}

void on_key_event(const Engine::KeyEvent &evt) { mapper.inject_key(evt); }

void create_key_mapping(Entity id, const KeyBoardMapping &map) {
  mapper.set_key(VK_LEFT, on_turn_left, id);
  mapper.set_key(VK_RIGHT, on_turn_right, id);
  mapper.set_key(VK_UP, on_turn_up, id);
  mapper.set_key(VK_DOWN, on_turn_down, id);
  mapper.set_key(VK_SPACE, on_fire_missle, id);
}

void intialize() { Events::Event<Engine::KeyEvent>::Listen(on_key_event); }

void create(Entity id, const KeyBoardMapping &map) {
  auto playerSearch = find_entity(id);
  if (playerSearch) {
    // Todo: overwrite the key mappings
    return;
  } else {
    if (nbrPlayers > MAX_LOCAL_PLAYERS)
      return;
    inputCache[nbrPlayers].id = id;
    ++nbrPlayers;
    create_key_mapping(id, map);
  }
}

void remove(Entity id) {
  // Do nothing
}

void on_logic_tick() {
  for (std::size_t index = 0; index < nbrPlayers; ++index) {
    auto &player = inputCache[index];
    actions::do_action(player.id, player.movement);
    actions::do_action(player.id, player.special);
    // player.movement = actions::Actions::no_action;
    // player.special = actions::Actions::no_action;
  }
}

void on_motion(Entity id, actions::Actions action) {
  auto optActionCache{find_entity(id)};
  if (optActionCache) {
    auto actionCache = optActionCache.value();
    actionCache->movement = action;
  }
}

void on_special(Entity id, actions::Actions action) {
  auto optActionCache{find_entity(id)};
  if (optActionCache) {
    auto actionCache = optActionCache.value();
    actionCache->special = action;
  }
}

void on_turn_right(Entity id) { on_motion(id, actions::Actions::turn_right); }
void on_turn_left(Entity id) { on_motion(id, actions::Actions::turn_left); }
void on_turn_up(Entity id) { on_motion(id, actions::Actions::turn_up); }
void on_turn_down(Entity id) { on_motion(id, actions::Actions::turn_down); }
void on_fire_missle(Entity id) {
  on_special(id, actions::Actions::fire_missle);
}
void on_fire_special(Entity id) {}
} // namespace crossfire::keyboardinput

namespace crossfire::actions {
void do_action(Entity id, Actions action) {
  switch (action) {
  case Actions::turn_right:
    turn_right(id);
    return;
  case Actions::turn_left:
    turn_left(id);
    return;
  case Actions::turn_up:
    turn_up(id);
    return;
  case Actions::turn_down:
    turn_down(id);
    return;
  case Actions::fire_missle:
    fire_missle(id);
    return;
  case Actions::fire_special:
    fire_special(id);
    return;
  case Actions::no_action:
    return;
  }
}

void turn_right(Entity id) { linear::changeHeading(id, Heading::Right); }
void turn_left(Entity id) { linear::changeHeading(id, Heading::Left); }
void turn_up(Entity id) { linear::changeHeading(id, Heading::Up); }
void turn_down(Entity id) { linear::changeHeading(id, Heading::Down); }
void fire_missle(Entity id) {}
void fire_special(Entity id) {}
} // namespace crossfire::actions

namespace crossfire::missle_shooter {
struct MissleDef {
  unsigned max_on_screen_at_once;
  std::uint64_t ms_delay;
  unsigned nbr_missles_on_screen;
  std::uint64_t ms_last_fired;
};

soa::SOA<soa::FixedArray<16>, Entity, MissleDef> mSOA;

void update_missle_shooter(Entity id, unsigned int max_missles,
                           unsigned int delay_ms);

void fire(Entity owner, CurrentPosition pos);
} // namespace crossfire::missle_shooter
