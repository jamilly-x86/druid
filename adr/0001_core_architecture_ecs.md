# ADR 0001: Core Architecture – ECS with Scene Graph Integration

# Status
Accepted

# Context
Early engine prototypes treated gameplay data, simulation, and rendering as a single object-oriented scene graph. While this made it easy to render hierarchical UI, it created tight coupling between gameplay state and drawable nodes, making it hard to unit-test, parallelize updates, or support headless simulations. Exploring dedicated entity component system (ECS) frameworks showed clear advantages for data-driven gameplay, but pure ECS made it awkward to express GUI hierarchies, animations, and rendering-specific relationships.

# Decision
Adopt a hybrid architecture:

- Use an ECS to own game entities, components, and systems responsible for simulation, AI, physics, and other gameplay logic.
- Keep an object-oriented scene graph to represent renderable GUI elements and other drawing-time hierarchies.
- Introduce an action/event layer so ECS systems can publish gameplay state changes, and UI nodes can subscribe to these events to stay in sync without holding direct references to ECS data.

The following pseudocode illustrates the intended developer experience when wiring gameplay entities with GUI views:

```cpp
// Engine engine;
EngineWindow engine; // Engine with window?
engine.add_system<Physics>();
engine.add_system<Navigation>();
engine.add_system<Input>();

auto paddle = engine.create_entity();
paddle.add_component<Transform>();
paddle.add_component<ShapeRectangle>();
paddle.add_component<RigidBody>();
paddle.add_component<Kinematic>();
    
auto& input = paddle.add_component<PlayerController>();
input.add("move_up").trigger(Keyboard::Key::Up);
input.add("move_down").trigger(Keyboard::Key::Down);
input.add("pause").trigger(Keyboard::Key::Esc).event(EventPause{});
input.on_action("pause") = [] {
   engine.event(EventPause{});
};

auto paddle_ai = paddle.clone();
paddle_ai.remove_component<InputActions>();

auto ball = paddle_ai.clone();

controls = engine.add_service<ControlService>();
auto& root = controls.root_node();

// MVC View for entities and their components? Do render systems render here?
auto& entity_view = root.create_node<EntityView>();
entity_view.add_render_system<Geometry>();
entity_view.set_entities(engine.entities());

auto& layout = root.create_node<RowLayout>();
layout.create_node<Button>().on_click([]{});
layout.create_node<Button>().on_click([]{});
layout.create_node<Button>().on_click([]{});

auto& cursor = root.create_node<Icon>();
cursor.create_node<Mouse>().on_move([]{ 
    // cursor.set_position();
});

engine.run();
```

# Consequences
- **Pros:** Gameplay systems remain decoupled, testable, and data-driven; UI and rendering keep their natural hierarchical representation; the event/action layer formalizes communication between ECS and scene graph and encourages explicit state propagation.
- **Cons:** Maintaining two models requires clear synchronization patterns; developers must reason about event lifetimes and avoid redundant data copies; the hybrid approach introduces additional plumbing when compared to a pure ECS or pure scene graph.