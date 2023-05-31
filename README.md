# Fluid Mechanic

Made by Baptiste SUZON & Alex FIGUEIREDO

# How to Launch it

You need Unreal Engine 4.27.2
Launch FluidMechanic.uproject and launch NewMap.umap.

# Code

All the mathematics parts is in FluidSystem.h and FluidSystem.cpp in the folder Source/FluidMechanic.
It contains all parts of the SPH algorithm, which doesn't work properly yet

# Note

In Unreal Engine, the unit of measure is in centimeter. That caused some trouble to iterate as some formulas
are based on meters, and it was troublesome to determine where it could break the simulation.

# R&D

While looking for an engine adequate for this project, we tested out Unity's particle system and Unreal Engine's niagara
particle system.

- The Unity particle system has too much drawbacks to use it properly to simulate
  fluid.
- Niagara for Unreal Engine has been used many times for implementing fluids in Unreal Engine (It is now native to
  Unreal Engine 5). One crucial feature needed to quickly implement the fluid was the Neighbor Grid but it is still in
  beta and unfortunately didn't work at all.

# Results

Because of the shortened deadline, we decided to stick Unreal Engine 4 and use instanced mesh to render the fluid.
Its simulation is done with an AActor which handle the particles, and draw them using instanced meshes.
The fluids seems to work on a plane, but the pressure forces don't handle gravity and such, which make them collapse.

We didn't have time to troubleshoot enough, and lost precious time due to the research made before the shortened
deadline (And last-minute organization)