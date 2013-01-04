ISING - An Ising model simulator
=================================

## Building the source code
Simply run:

     ./configure
     make
     make install

After `make install`, the **ising** program will be found in 
the `install/bin` directory. If you want to install the ising model simulator
in a global directory you can use the `--prefix` option:

     ./configure --prefix=/usr/local
     make
     sudo make install


## Using the Ising model simulator:

### Quick start:

     ising -d 2 -L 20 -T 4.0 --nmeas 100 --nmcs 10000000 --ieq 5000 --dyn 0

This command runs a 20x20 lattice, at a T=4 temperature, performing 10 
millions of montecarlo sweeps of the lattice, printing the energy, the
magnetization and a simple estimation of average cluster radius every 100
sweeps. Before the 10 millions sweeps are performed, 5000 sweeps are done
for thermalization. Each sweep is done using a random glauber dynamic (a
simple random spin flip).

### Introduction

To be written. If you are here you should know a bit about the Ising model
and the Metropolis algorithm.

#### Program workflow
This is more or less how the program works:

 1. Parse input parameters.
 2. Initialize geometry.
 3. Initialize spins values (+1,-1).
 4. Compute Energy and Magnetization.
 5. Perform `--ieq` Metropolis sweeps to thermalize the state of the system.
 6. Perform `--nmcs` sweeps, printing the Energy, the Magnetization and an 
    estimation of the cluster radius every `--nmeas` measures.

### Geometry
The ising model simulator defines an hypercubic lattice, which allows
the following parameters to be set:

 * Dimension (-d):
     - 2: Square lattice
     - 3: Cubic lattice
     - 4: Hypercubic lattice
     - 5: 5-dimesion cube
     - ...
 * Side length (-L): The number of spins of each dimension.

 * Print state (--print-state): This option, only enabled if dimension is 2,
     will create a "state" subdirectory with several text files that contain
     a  LxL square matrix with the position of the spins (either UP or DOWN). This 
     text file can be used later on to plot an image of the system and seeing
     the formed clusters.

Examples:

 * On a "-d 2 -L 20" lattice, we will have 20^2 = 400 spins
 * On a "-d 3 -L 8" lattice, we will have 8^3 = 512 spins

#### Using non hypercubic geometries:
The Ising model simulator may be used with any given geometry. However, an
initialization routine for any other specific geometry has to be implemented.

If you want to use a different geometry, only the `main` function would need
to be changed, preferably by replacing the `InitHypercubicLattice` function
or adding another switch option to the program (i.e. `--geometry`).

Patches are welcome.

Generic initialization routines for reading geometries from files would also
be very welcome.

### Metropolis parameters

We consider a "MonteCarlo sample" a loop over all sites of the system 
(this is "full sweep of the lattice"). This means that a single sample 
on a lattice of 400 spins, consists of 400 update proposals.

Using this convention, it is easier to compare Metropolis convergence
on different system sizes. Bear in mind that the larger the system is,
the longer will take a single "MonteCarlo sample" as more updates will 
be necessary to do a single sweep.

The Metropolis algorithm allows the following parameters:

  * Thermalization iterations (--ieq): Just after the system initialization,
      it may be convenient to discard a number of samples in order to allow 
      the system to reach an equilibrium state. Set this parameter to 0 if 
      you want to see the thermalization.
  * Number of MonteCarlo samples (--nmcs): The total number of MonteCarlo 
      samples to perform after thermalization.
  * Number of samples between each measurement (--nmeas): The number of 
      samples to perform between each system printing. For instance, if 
      we want to do 100000 sweeps, but only print 1000 values of the energy
      and magnetization, we would use an `--nmeas 100`.

### Dynamics
The Metropolis algorithm does not impose a particular dynamic of the system. 
A dynamic describes "how the updates should be performed". The available
dynamics are:

  * Glauber: A spin is chosen randomly and flipped.
  * Glauber sequential: Spins are flipped sequentially, following on every 
       sweep the same order.
  * Kawasaki random: Two opposite spins are chosen randomly and and exchanged.
  * Kawasaki neighbours: Two opposite neighbouring spins are chosen randomly
       and exchanged. TODO: The implementation of this dynamic is biased for 
       geometries where some spins have more neighbours than others (not the
       hypercubic case).
  * Wolff: This is a clustering dynamic:
   1. A spin S1 is chosen randomly.
   2. Check the spin of each of the S1 neighbours. If the neighbour's spin is 
       the opposite of S1, then that neighbour is not added to the cluster but
       if spins are the same, then the neighbour is added to the cluster with
       an acceptance rate which depends on the temperature.
   3. Move to the added neighbours and repeat until a cluster is formed.
   4. Flip the cluster.


