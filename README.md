# Lennard-Jones Cluster Optimization App

![app](https://github.com/user-attachments/assets/66dd3a1e-ccd1-4ca3-a524-8cde47dbbb19)


## Overview

This application is a **Qt-based** desktop program designed for optimizing **Lennard-Jones clusters** using the **jDE (self-adaptive Differential Evolution)** algorithm. The goal is to find optimal configurations of atom clusters by minimizing their potential energy based on the Lennard-Jones potential.
The application provides real-time visualization and monitoring of the optimization process through 2D graphs. It also includes several customizable options for program execution and graph display. Users can import/export data for further analysis and export algorithm results.

## Features

- **Lennard-Jones Cluster Optimization**:
  - Uses the jDE evolutionary algorithm to optimize the arrangement of atom clusters.
  - Real-time visualization of the optimization progress.
  - Displays energy minimization and convergence during the evolutionary process.

- **Customizable Program Options**:
  - Adjustable parameters for the jDE algorithm, such as population size, generator seed, run time limit.
  - Control over optimization parameters like cluster size and interaction forces.

- **Graph Customization**:
  - Users can modify the appearance of the graph, including color schemes, axis ranges, zoom and more.
  - Export and import graph images and data values.
 ![graph](https://github.com/user-attachments/assets/be050a30-fd61-4b57-9e6d-23ded229b35d)



- **Data Export/Import**:
  - Export optimization results, including atom positions and energy values, to external files.
  - Import previous runs or pre-existing configurations to resume optimization or analyze results.
  - Automatic export of optimization results and final configurations.
  - Automatically saves the final configuration and optimization progress to files for easy retrieval and comparison.
  - 
- **Statistics**:
  - The app collects algorithm-specific and application statistics.
  - Statistics such as average run time can be viewed for analytic purposes.

  ![sdsdds](https://github.com/user-attachments/assets/b954dbd3-240c-4af9-a400-860b549df2a5)
  ![dsdssd](https://github.com/user-attachments/assets/5a7263ec-cd9f-4809-8ea5-4657f390ce10)


## How to Use

1. **Set Optimization Parameters**: Adjust the jDE parameters and cluster size according to your needs in the program options.
2. **Run Optimization**: Start the Lennard-Jones cluster optimization and monitor the progress in real-time through the provided graphs.
3. **View and Customize Graphs**: Customize the graph’s appearance to better visualize the convergence process.
4. **Export Data**: Export the optimization data, graph values, or final configurations using the export feature.
5. **Import Data**: Load previously saved configurations or optimization progress to resume work or for further analysis.

## Dependencies

- **Qt**: For GUI development and graph rendering.
- **jDE Algorithm**: A differential evolution algorithm designed for self-adaptation and optimization.

## Future Plans

- Planned integration of 3D scatter plot visualization for displaying 3D cluster configurations.
