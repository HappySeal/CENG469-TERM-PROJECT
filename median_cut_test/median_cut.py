import numpy as np
from collections import namedtuple

# Define a namedtuple to represent a cube in the color space
Cube = namedtuple('Cube', ['colors', 'min_vals', 'max_vals'])

def median_cut(image, num_colors):
    # Flatten the image into a list of RGB pixels
    pixels = np.reshape(image, (-1, 3))

    # Initial cube contains all the pixels
    initial_cube = Cube(colors=pixels, min_vals=np.min(pixels, axis=0), max_vals=np.max(pixels, axis=0))
    cubes = [initial_cube]

    while len(cubes) < num_colors:
        # Find the cube with the maximum side length
        max_cube_idx = max(range(len(cubes)), key=lambda i: np.max(cubes[i].max_vals - cubes[i].min_vals))
        max_cube = cubes.pop(max_cube_idx)

        # Determine the longest dimension of the cube
        split_axis = np.argmax(max_cube.max_vals - max_cube.min_vals)

        # Sort colors along the chosen axis and split the cube
        colors = max_cube.colors[max_cube.colors[:, split_axis].argsort()]
        median_index = len(colors) // 2
        left_cube = Cube(colors=colors[:median_index], min_vals=max_cube.min_vals,
                         max_vals=np.max(colors[:median_index], axis=0))
        right_cube = Cube(colors=colors[median_index:],
                          min_vals=np.min(colors[median_index:], axis=0), max_vals=max_cube.max_vals)

        # Add the split cubes back to the list
        cubes.extend([left_cube, right_cube])

    # Return the median colors of the cubes
    return [np.median(cube.colors, axis=0).astype(int) for cube in cubes]

# Example usage:
# Assuming you have an image represented as a numpy array called 'image'
# with shape (height, width, 3) and num_colors is the desired number of colors
# representative_colors = median_cut(image, num_colors)
