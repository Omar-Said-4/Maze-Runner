
# Game Engine & Maze Runner <a name="game-engine--maze-runner"></a>

# Table of Contents

- [Game Engine Features](#game-engine-features)
  - [Physics](#physics)
  - [Lighting](#lighting)
  - [Sound](#sound)
  - [Text Rendering](#text-rendering)
  - [Post-Processing](#post-processing)
- [Game](#game)
  - [Controls](#controls)
  - [Game Scenario](#game-scenario)
  - [Create Custom Mazes](#create-custom-mazes)
  - [Powerups](#powerups)
- [Game Video](#game-video)
- [Technologies Used](#tech-used)
- [Contributors](#contributors)

---

## Game Engine Features <a name="game-engine-features"></a> 🎮

### Physics <a name="physics"></a> ⚛️

- **Full Collision System**: Implementing a robust collision detection mechanism based on bounding boxes. Bounding boxes are axis-aligned rectangular prisms that encapsulate the geometry of game objects. This approach efficiently determines intersections between these boxes, providing accurate detection of mesh collisions.
- **Collision Handling**: Integrates comprehensive collision detection across physical components, enabling border handling and object picking mechanisms.

### Lighting <a name="lighting"></a> 🔦

- **Light Sources**:
  1. **Directional Light**: Uniform light emitted in a specific direction with consistent intensity across the entire scene.
  2. **Point Light**: Emulates a light source originating from a point, with intensity decreasing with distance from the source.
  3. **Spotlight**: Mimics a focused beam of light emitted within a defined cone angle, with intensity attenuating both with distance and angle from the center of the cone.

- **Lit Materials 🎨**: Each material integrates 3 texture inputs to implement the Phong lighting model:
  1. **Albedo**: Defines the diffuse color and texture properties of the material.
  2. **Specular and Roughness**: Controls the metallic properties of the material, specifying its specular reflection and surface roughness.
  3. **Ambient Occlusion**: Enhances shading by simulating areas where light is occluded or scattered, adding depth and realism to textures.
  4. **Emissive**: Allows materials to emit light, enhancing their appearance and enabling the simulation of glowing or emissive surfaces.

- **Lighting Shaders 🖥️**: Utilizes vertex and fragment shaders to calculate pixel intensity based on material properties. These shaders ensure accurate color representation for lit materials, incorporating diffuse, specular, ambient occlusion, and emissive effects as defined by the material textures.

## Sound <a name="sound"></a> 🎵

- **Sound System Implementation**: Utilizes the `miniaudio.h` header-only library to implement a comprehensive sound system.
   - Handles playback, stopping, integration, and concurrent management of multiple sound sources.

## Text Rendering <a name="text-rendering"></a> 📝

- **Dear ImGui Integration**: Implements text rendering using Dear ImGui for displaying text on small windows above the main game window.

## Post-Processing <a name="post-processing"></a> 🖼️

- **Vignette Effect**: Applies a vignette effect to enhance the focus on the center of the screen by darkening the edges.
   
- **Color Saturation Enhancement**: Improves the saturation of pixel colors to make visuals more vibrant and vivid.
   
- **Fog Effect Addition**: Introduces a fog effect to simulate atmospheric conditions, enhancing depth and atmosphere in the game environment.

# Game <a name="game"></a> 🎲

## Controls <a name="controls"></a> 🕹️

- **W**: Move forward.
- **A**: Move left.
- **S**: Move backward.
- **D**: Move right.
- **F**: Toggle flashlight.
- **M**: Control music.
- **Left Mouse Click and Move**: Rotate view.

## Game Scenario <a name="game-scenario"></a> 🎯

1. Collect all normal keys.
2. Obtain the master exit key.
3. Open the exit door.
4. Collect coins to enhance score (optional).
5. Gather power-ups to aid your progress.
6. Complete objectives before the timer runs out.

## Create Custom Mazes <a name="create-custom-mazes"></a> 🧩

Go to `assets/mazes` and create a maze of your choice using the following symbols:
- **p** : Portal
- **k** : Key
- **m** : Master key
- **|** : Vertical wall
- **-** : Horizontal wall
- **o** : Coins
- **r** : Rocket
- **s** : Speed boost
- **.** : Empty space

## Powerups <a name="powerups"></a> ⚡

1. **Speed Powerup**: Increases player speed.
2. **Portal Powerup**: Enables passage through walls.
3. **Gravity Powerup**: Provides a higher view of the maze.

## Game Video <a name="game-video"></a> 🎥

[Watch the Game Video](https://youtu.be/TalnwDtRQGA)

# Technologies Used 🛠️   <a name="tech-used"></a>

- <img src="https://icon.icepanel.io/Technology/svg/GCC.svg" alt="GCC" width="20" height="20"> GCC: GNU Compiler Collection for compiling C++ code.
- <img src="https://icon.icepanel.io/Technology/svg/OpenGL.svg" alt="OpenGL" width="20" height="20"> OpenGL: Graphics library used for rendering 2D and 3D vector graphics.
- <img src="https://upload.wikimedia.org/wikipedia/commons/1/18/ISO_C%2B%2B_Logo.svg" alt="C++" width="20" height="20"> C++: Programming language used for game engine development.
- <img src="https://upload.wikimedia.org/wikipedia/commons/1/13/Cmake.svg" alt="CMake" width="20" height="20"> CMake: Cross-platform build system used to manage the build process.
- `Dear ImGui`: GUI library used for text rendering and UI elements.
- <img src="https://products.fileformat.com/audio/cpp/miniaudio/header-image.png" alt="miniaudio" width="20" height="20"> miniaudio: Header-only library used for sound playback and management.



## ✍️Contributors <a name = "contributors"></a>

<table>
  <tr>
   <td align="center">
    <a href="https://github.com/Omar-Said-4" target="_black">
    <img src="https://avatars.githubusercontent.com/u/87082462?v=4"  alt="Omar Said"/>
    <br />
    <sub><b>Omar Said</b></sub></a>
    </td>
   <td align="center">
    <a href="https://github.com/ebramatef00" target="_black">
    <img src="https://avatars.githubusercontent.com/u/93336170?v=4" alt="Ebram Atef"/>
    <br />
    <sub><b>Ebram Atef</b></sub></a>
    </td>
<td align="center">
    <a href="https://github.com/Ibraam-Nashaat" target="_black">
    <img src="https://avatars.githubusercontent.com/u/93844847?v=4" alt="Ibraam Nashaat"/>
    <br />
    <sub><b>Ibraam Nashaat</b></sub></a>
    </td>
<td align="center">
    <a href="https://github.com/michaelehab" target="_black">
    <img src="https://avatars.githubusercontent.com/u/29122581?v=4" alt="Michael Ehab"/>
    <br />
    <sub><b>Michael Ehab</b></sub></a>
    </td>
  </tr>
</table>
