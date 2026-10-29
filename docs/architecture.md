# Architecture

Code organization and development focused in complying SOLID principles. 
The diagrams are written in [Mermaid](https://mermaid.js.org/).


## Layers and the dependency rule

The code is split into four folders. **Dependencies only point downwards**:
`core` knows nothing about the other layers, and only `presentation` and `app`
know about SFML.

| Folder | Responsibility | SFML dependency |
|---|---|---|
| `src/core` | Game state and rules (`Board`, `Game`, `Rules`, `Match`) and the `IPlayer` abstraction | No |
| `src/players` | Implementations of `IPlayer`: `HumanPlayer`, `AIPlayer` | No |
| `src/presentation` | Drawing and input helpers (`BoardView`, `Button`, `InputMapper`, `UiConfig`) | Mostly (`InputMapper` and `BoardLayout` are SFML-free) |
| `src/app` | Window, main loop and scenes (`Application`, `MenuScene`, `GameScene`) | Yes |

Because `core` and `players` are free of SFML, they are covered by unit tests
that run without opening a window.

## Class diagrams

### Domain: `core` and `players` (no SFML)

```mermaid
classDiagram
direction TB

class Mark {
    <<enumeration>>
    None
    X
    O
}
class Move {
    <<struct>>
    +int row
    +int col
}
class Board {
    +Size int = 3
    +inBounds(Move m)$ bool
    +at(Move m) Mark
    +isEmpty(Move m) bool
    +place(Move m, Mark mark) bool
    +isFull() bool
    +clear() void
}
class Rules {
    <<functions>>
    +findWinner(Board board) Mark
}
class GameStatus {
    <<enumeration>>
    InProgress
    XWon
    OWon
    Draw
}
class MoveResult {
    <<enumeration>>
    Ok
    OutOfBounds
    CellOccupied
    GameOver
}
class Game {
    +tryPlay(Move m) MoveResult
    +reset() void
    +board() Board
    +currentTurn() Mark
    +status() GameStatus
    +isOver() bool
}
class IPlayer {
    <<interface>>
    +chooseMove(Board board, Mark self) optional~Move~
}
class Match {
    +update() void
    +restart() void
    +game() Game
}
class HumanPlayer {
    +submit(Move m) void
    +chooseMove(Board board, Mark self) optional~Move~
}
class Difficulty {
    <<enumeration>>
    Easy
    Medium
    Hard
}
class AIPlayer {
    +chooseMove(Board board, Mark self) optional~Move~
}

Board --> Mark : stores
Board ..> Move
Game *-- Board
Game ..> Move
Game --> GameStatus
Game ..> MoveResult
Game ..> Rules : uses
Match *-- Game
Match *-- "2" IPlayer : owns
IPlayer <|.. HumanPlayer
IPlayer <|.. AIPlayer
AIPlayer --> Difficulty
AIPlayer ..> Rules : uses
```

`Move` is the value type passed around everywhere (`Board`, `Game`,
`IPlayer`, `HumanPlayer`, `AIPlayer`); only two of its arrows are drawn to improve diagram readability.

### Application and presentation (SFML)

```mermaid
classDiagram
direction TB

class IScene {
    <<interface>>
    +handleEvent(Event event) void
    +update() void
    +draw(RenderTarget target) void
}
class ISceneNavigator {
    <<interface>>
    +goTo(IScene next) void
    +quit() void
}
class Application {
    +run() int
    +goTo(IScene next) void
    +quit() void
}
class MenuScene
class GameScene
class Button {
    +contains(float x, float y) bool
    +setHovered(bool hovered) void
    +draw(RenderTarget target) void
}
class BoardView {
    +draw(RenderTarget target, Board board) void
}
class InputMapper {
    +cellAt(float px, float py) optional~Move~
}
class Match {
    <<core>>
}
class HumanPlayer {
    <<players>>
}
class AIPlayer {
    <<players>>
}

ISceneNavigator <|.. Application
Application *-- "1..2" IScene : current / pending
IScene <|.. MenuScene
IScene <|.. GameScene
MenuScene --> ISceneNavigator
GameScene --> ISceneNavigator
MenuScene *-- "4" Button
GameScene *-- "3" Button
MenuScene ..> GameScene : creates
GameScene ..> MenuScene : creates
GameScene *-- Match
GameScene *-- BoardView
GameScene *-- InputMapper
GameScene o-- HumanPlayer : non-owning
GameScene ..> AIPlayer : creates
```

Notes:

- `BoardView` and `InputMapper` both take a `BoardLayout`, so drawing and click detection always
  agree on the geometry.

- `Match`, `HumanPlayer` and `AIPlayer` are shown collapsed; their details are
  in the domain diagram above.
  
- `UiConfig.h` (colours, window size, board position) and `TextUtil.h` are
  shared helpers used across `presentation` and `app`; they are left out to keep
  the diagram readable.
  
- `Rules` is a set of free functions, shown as a class only for convenience.

- `main()` creates one `Application` and calls `run()`.

## Sequence diagram: one human move and the AI reply

```mermaid
sequenceDiagram
    actor P as Player
    participant App as Application
    participant GS as GameScene
    participant IM as InputMapper
    participant HP as HumanPlayer
    participant M as Match
    participant G as Game
    participant AI as AIPlayer

    P->>App: left click
    App->>GS: handleEvent(MouseButtonPressed)
    GS->>IM: cellAt(x, y)
    IM-->>GS: Move
    GS->>HP: submit(move)

    Note over App,GS: next frame
    App->>GS: update()
    GS->>M: update()
    M->>HP: chooseMove(board, X)
    HP-->>M: Move
    M->>G: tryPlay(move)
    G-->>M: Ok

    loop every frame until the short delay has passed
        App->>GS: update()
    end
    GS->>M: update()
    M->>AI: chooseMove(board, O)
    AI-->>M: Move
    M->>G: tryPlay(move)
    G-->>M: Ok
    App->>GS: draw(window)
```

`Match::update()` polls the current
player once per frame, and `std::nullopt` means "no decision yet". That is why
the same loop works for a mouse click and an AI, or (hopefully in the future) a
network opponent.
