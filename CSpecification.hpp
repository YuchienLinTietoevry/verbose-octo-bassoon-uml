#include <functional>

template <typename Entity> using Specification = std::function<bool(Entity)>;

template <typename Entity>
constexpr auto And =
    [](Specification<Entity> spec1, Specification<Entity> spec2) {
      return [&](Entity e) { return spec1(e) && spec2(e); };
    };

template <typename Entity>
constexpr auto operator&&(Specification<Entity> spec1, Specification<Entity> spec2) {
    return And<Entity>(spec1, spec2);
}

template <typename Entity>
constexpr auto Or =
    [](Specification<Entity> spec1, Specification<Entity> spec2) {
      return [&](Entity e) { return spec1(e) || spec2(e); };
    };

template <typename Entity>
constexpr auto operator||(Specification<Entity> spec1, Specification<Entity> spec2) {
    return Or<Entity>(spec1, spec2);
}

template <typename Entity>
constexpr auto Not = [](Specification<Entity> spec) {
  return [&](Entity e) { return !spec(e); };
};

template <typename Entity>
constexpr auto operator!(Specification<Entity> spec) {
    return Not<Entity>(spec);
}

template <typename Entity> class IList;

template <typename Entity> class IRepository {
public:
  virtual void Add(Entity e) = 0;
  virtual void Remove(Specification<Entity> query) = 0;
  virtual IList<Entity> Query(Specification<Entity> query) = 0;
};

struct Data {
    int instanceId;
    // some data here
};

constexpr auto FOO_INSTANCE_ID = 1;
constexpr auto BAR_INSTANCE_ID = 2;

constexpr auto GetDataByInstanceIdFactory = [](int instanceId) {
    auto fn =  [instanceId](Data data) { return data.instanceId == instanceId; };
    return Specification<Data>(fn);
};

constexpr Specification<Data> GetFoo = GetDataByInstanceIdFactory(FOO_INSTANCE_ID);
constexpr Specification<Data> GetBar = GetDataByInstanceIdFactory(BAR_INSTANCE_ID);

constexpr auto GetFooAndBar = GetFoo && GetBar;
