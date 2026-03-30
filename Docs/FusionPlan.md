# Fusion — Standalone UI Library Plan

## Vision

Fusion is a declarative, retained-mode C++ UI library targeting **graphics and engine tool developers**.
It is extracted from CrystalEngine and consumed back by the engine as a third-party dependency,
validating the abstraction design from day one.

**Positioning**: A modern alternative to Dear ImGui for complex tools and editors, inspired by Unreal Engine's Slate.

---

## Target Audience

**Primary**: C++ graphics and engine tool developers building:
- Game engine editors
- Rendering debuggers and profilers (RenderDoc, Tracy-style tools)
- Asset pipeline tools, level editors, shader editors
- In-game dev consoles and debug overlays

**Why this audience**:
- Already use ImGui and feel its limitations acutely
- Control their own render loop — the `IFusionRenderer` abstraction fits naturally
- C++ experts who appreciate the declarative API and spring animations
- Small but highly influential — adoption spreads fast through this community

**Not targeting (v1)**: General C++ app developers, Unity/Godot users, beginners.

---

## License

**MIT** — matches Dear ImGui, zero friction for commercial studios and indie developers alike.

Consider adding a **CLA** before the first external PR lands, to preserve relicensing flexibility.

---

## Architecture

```
Host Application (drives tick, feeds events)
        │
        ▼
  Fusion Core (widgets, layout, painting, animation)
        │                          │
        ▼                          ▼
IFusionWindowBackend         IFusionRenderer
        │                          │
SDL3 / Win32 / GLFW /      Vulkan / D3D12 / Metal /
custom                     engine RPI / custom
```

### Key Design Rules
- Fusion does **not** own the frame graph — `RenderSurface()` receives an opaque pass/scope context so engines inject Fusion as a pass in their own frame graph
- Fusion does **not** own the render loop — the host application calls `tick()` and Fusion paints when told
- `FRenderSnapshot` (CPU-side draw list) is public API — advanced users can write backends without implementing the full interface
- No engine RTTI, no code generation, no external build step required
- **No global or static mutable state** — `FusionApp` is a plain instantiable object, not a singleton. Every piece of state (services, font atlases, style sets, renderer resources) lives inside the instance. Two `FusionApp` objects share zero state

### Context Isolation

`FusionApp` must be a plain object the user creates and owns:

```cpp
// Correct: two fully isolated instances, no shared state
FusionApp appA(rendererA, windowBackendA);
FusionApp appB(rendererB, windowBackendB);

appA.Tick();  // touches only appA's state
appB.Tick();  // touches only appB's state
```

This is critical for:
- **Audio plugins** — hosts (Ableton, Reaper, etc.) load plugin DLLs multiple times in the same process; each instance needs a fully isolated UI context
- **Multi-window tools** — editor + runtime UI running simultaneously with separate render targets
- **Testing** — headless context per test with no state bleeding between runs
- **DLL boundaries** — globals in DLLs on Windows are per-DLL, causing subtle cross-boundary bugs

Fusion does **not** need to be thread-safe or reentrant. The guarantee is simpler: if two instances never share memory, the threading question is irrelevant entirely. No locks required — just no globals.

The checklist:
- `FusionApp` — plain instantiable object, no `Get()` singleton
- All services (animation, event, image, etc.) — instance state, not static
- Font and image atlases — per-instance, never shared statics
- Internal allocators and caches — must live inside the app object

### Module Layout

```
Fusion/                        ← standalone repo, git submodule in CrystalEngine
  FusionCore/                  ← no GPU or windowing dependencies
  FusionVulkanRenderer/        ← reference renderer backend
  FusionWin32Backend/          ← windowing backend
  FusionSDL3Backend/           ← windowing backend (requires find_package SDL3)

CrystalEngine/
  Engine/Source/FusionRPIRenderer/   ← engine-specific renderer backend, lives in engine repo
```

---

## V1 Feature Set

### Renderer Backend
- `IFusionRenderer` abstract interface
- Frame graph integration: `RenderSurface()` accepts opaque scope context for host frame graph injection
- **Reference backend: Vulkan** (cross-platform, respected by target audience)
- D3D12 and Metal backends post-v1

### Windowing Backend
- `IFusionWindowBackend` abstract interface
- **Ship**: Win32 backend, SDL3 backend
- SDL3 via `find_package(SDL3)` with opt-in `FUSION_USE_SDL3` CMake flag — no submodule
- GLFW backend: good community contribution target, not written by maintainer for v1

### Platform Support (V1)
- Windows, macOS, Linux
- iOS / Android: stub touch event shape in `IFusionWindowBackend` now so no redesign is needed later

### Core Widget Set

| Category | Widgets |
|----------|---------|
| Layout | `VStack`, `HStack`, `SplitView`, `ScrollBox`, `Overlay`, `Spacer` |
| Basic | `Label`, `Button`, `Image`, `Separator` |
| Input | `TextInput` (single-line), `Checkbox`, `Slider`, `DropDown` |
| Display | `ProgressBar`, `Tooltip`, `Popup`, `ColorPicker` |
| Structure | `Panel`, `TabView`, `TreeView`, `MenuBar`, `ContextMenu` |

### Docking System
First-class, not bolted on — primary differentiator over Dear ImGui:
- Floating panels
- Dockable panel groups with tab bars
- Horizontal/vertical split regions
- Persistent layout save/restore (JSON, no engine serialization required)

### Painting & Rendering
- Paths, bezier curves, rounded rects
- Solid fill, linear/radial/conic gradients
- Clipping regions
- Opacity and blend modes
- Image rendering with UV support

### Text Rendering
- TTF/OTF font loading (FreeType for quality; stb_truetype as zero-dependency fallback)
- SDF text rendering for crisp scaling at any size
- Single-line text layout (multiline deferred to v2)
- Internal font atlas management

### Animation
A genuine differentiator — ship it:
- Spring animations
- Tween with easing curves
- Per-widget property transition system

### Input
- Mouse: position, buttons, scroll wheel
- Keyboard: key codes, modifiers, text input events
- Drag and drop within Fusion (widget-to-widget): v1
- OS-level drag and drop (files from explorer): v2
- IME: v2
- Touch / stylus: interface stubbed in v1, implemented v2

### Styling
- Style/theme system with built-in dark theme
- Per-widget style overrides
- Simple explicit style structs (no CSS cascade for v1)

---

## Explicitly Deferred (Post-V1)

| Feature | Reason |
|---------|--------|
| Accessibility (screen readers) | Enormous scope, specialized expertise |
| Rich text / multiline text editor | Project in itself |
| OS file/folder dialogs | Host application responsibility |
| Scripting / language bindings | Post-traction |
| Hot reload | Nice to have, not MVP |
| Mobile backends (iOS, Android) | Interface designed for it now, built later |
| Localization / BiDi text | v2 |
| D3D12 / Metal renderer backends | v2 |
| GLFW windowing backend | Community contribution |
| Software/CPU renderer | Useful for CI/headless tests — strong v2 candidate |

---

## CrystalEngine Migration Path

1. Define `IFusionRenderer` and `IFusionWindowBackend` in `FusionCore` with no engine includes
2. Create `FusionRPIRenderer` module in the engine repo — move `FRenderService`, `FSurface`, `FNativeSurface`, `FShader` into it, refactored to implement `IFusionRenderer`
3. Strip engine RTTI (`CLASS()`, `FIELD()`, `CE_CLASS()`) from all Fusion widget headers — replace `CreateObject<T>()` with plain construction, `Ref<T>` with a lightweight standalone intrusive ref count
4. Add Fusion as a git submodule — engine consumes it the same way any third-party consumer would
5. `FApplication` takes `IFusionRenderer*` and `IFusionWindowBackend*` injected at startup

If the engine cannot consume Fusion through the public interface without special treatment, the interface is not clean enough yet.
