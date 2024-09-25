- Head spitter should probably just remove spread itself since its clearer
- Objective tokens: Complete an objective in level grants permanent range boost
- Double jumping?
- Light volume texture
- Full meter bonus (larger radius for a time when max out meter)
- Something with moving entities and targeting (ie. whiff on moving, reduced speed)
- Reduced speed on target bounce
- Targetable flag on entities
- Coyote time jumps
- Multiply jump height based on charge (higher charge = more carry over)
- 3-5 frame windows for highest velocity before jump

- Homing attack
- Jump item
- Shorten cut time

- lv_weedline_patches needs some moving objects

- Slow mower that slowly follows player and cuts
    - Needs way to teleport to new spot. Item?

- Use terrain cursor on curves and bubbles

- Add items

- Spread Mower
    - Item? Maybe make a separate item for this
    - Follower
        - Attached via chain, have some object in level to break it?
        - Break chain using attack?
        - Break chain by falling in hole?
            - Then needs jumps mechanic
            - Keeping jump but this doesn't really work well
            - Player cant see the mower fall
    - How to reattach?
         - Hit again: makes player take action when they want
         - Timer: Forces player to go back to detacher
            - How does the object return to player?
                - Teleport back?
    - Hit object to gain seed, but have to get mower
    - Mower absorbs part of removed spread
    - Once detached, mower will give new seeds
        - Should also just give a minimum amount

- Rename Sleeper to Switcher
- New layout for lv_weeds
    - Maybe follows a line?
    - Needs to establish a good flow
    - Likely needs to be pretty big

- lv_tele flow redesign
    - Instead of teleporting, pod is stationary
    - Then, pod disappears for a length of time before coming back
    - Multiple pods though
    - Player needs to flow between pods to beat level
    - Transparency on pads

- Multi entity placement using terrain cursor
    - Needs some way to control density

- Remove SpreadKey
    
- Remove LevelProperties
- Items
- Save before closing prompt

- Make singletons

- Phase token
    - Spread particles
        - Move towards token when ready to be hit
        - Clear spread once hit like being blown out

- Fix layer sizes affecting positining

- Pillars on test level are bell towers 
    - Sound wave effect
- Inbetween are bell flowers that bloom

- Timed entities

- Use fixed vector on StaticModel materials

- Level editor
    - Property override
    - Modes init boolean defaults
    - Level properties editor
    - Multidelete
    - Multispawn
        - Randomize rotaiton and scale, decide uniform

- Generic threading(workers n shit)

- Weave obstacles
- Level clearance
- Particle velocity based on object rotation
- Particles on break
- Bloom
- Skybox

- Levels progress with time, certain things spawn in, events etc.
    - Plan around these events and setup the level as quikly as possible for optimal flowers

- Remove state setting from Render functions

- Rename InitBuffer to InitView

- Separate skeleton and animation

- Rename spread to flower

- Remove cutting

- Some enemies stay in center, others at border

- Each character comes back unless the player takes them
  out in a level
    - Stun enemies without killing
    - What about "vermin" ie. stuff just getting in the way

- Each level is an episode
- Characters you haven't seen interact will at some point in
  weird ways

- Hitbox clank
- Enemy model
- Enemy spread removal
- Enemy jumP
- Freeze emitters with entity

- Projectiles are hittable too, as up and strong hits
- Add reflecting hits
- Rework projectiles

- Convert all vectors to fixed
- Spread array instead of map

- Object that spurts out and player has to destroy it.
    - After some time, the object creates a permiter that slows player and removes spread

- Skating uphill creates a brake state
    - Spawn particles
    - Special animation (can reuse it in turn around brake)
    - Special jump / attack features

- Action keyframes to spawn particles

- Convert discards to alpha

- Use color uniform for seed

- Seperate assets from Renderer

- Hittable that clears way more spread on landing, but gives
  lots of seed. Also does more damage

- Enemy where you hit it, it poofs away. Then you have to chase it
  down to get a full combo/finish

- Pillars that shoot out fire when an object is over it
    - Combo: Launch -> Burned over pillar -> Hit after burn

- Going fast = ghost dimension
- Going slow = human dimension
- Changes interactions

- "Goomba"
    - Grow flowers around them? They take a second then burst
    - Hit them after the burst

- To exit level, player must have grown certain number of flowers
    - If some get cut the exit disappears
    - This means a player can stun enemies instead of elim
    - Elim gives lots of meter however
    - ie. player makes exit, but enemy takes out flower to prevent leaving

- How force boss fights with flower exit system?

- Attack button?
    - High attack (follow up, no homing)
    - Low attack (too fast to follow up, homing)
    - Burst jump large AOE high attack and homing?
    - Attack uses meter?
    - Only low attack can finish
    - High attack in air does more damage than a low attack

- "Goomba"
    - 2/3 hits to destroy
    - Walk around
    - Destroys seed in a radius
    - Jumps occasionally (really high)?

- Gust cut enemy
    - Blows large beam that cuts lots of spread down

- Fog
- Crystal
- Usables
- Raining seed
- Bloom
- Ground moving cutter enemy (leafy disk)

- const refs for strings

- Optimize Skeleton pose calculation
    - Going from Matrix -> Transform -> Matrix is inefficient. How to avoid this?    
    - Figure out some way to lerp Mat4?
    - Converting Transform to Mat4 takes the most time, specifically the multiplication
    - Or can just calculate a few skeletons every few ticks, then interpolate poses
    - Precompute bones in model space instead of local space?

- Flower model
    - Bending
    - Reduce overdraw with mesh shape

- Enemy
- Holes (terrain modification)

- Frustum culling

- Spawn list
- Store loaded entities in resource manager and copy instead of reconstructing
  from json

- Assets are being linked using their actual filenames. It'll probably be
    better to use indexes per level to link the assets. Either that or just use
    mapped IDs.
    - The main thing with per level indexing is keeping loaded assets between
      levels and keeping their index

- Camera needs to collide with world

- Instances need to have an ID

- Tree:
    - Enemy that breaks the tree
    - Add fruit spawning with random positions
    - Fruits can act as buffs ie. magnet

- Stump:
    - Not sure how it will interact

- Breakable bush (meter pad basically)
    - Tree probably better since height is necessary
    - Random offsets in interval spawner
