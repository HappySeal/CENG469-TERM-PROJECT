import numpy as np
from itertools import product

def scale_by_cos_phi(image):
    """
    Scale the image by cos(φ) where φ is the pixel’s angle of inclination.
    """
    height, width = image.shape[:2]
    y_indices, x_indices = np.indices((height, width))
    phi = (y_indices / height - 0.5) * np.pi  # Angle of inclination
    cos_phi = np.cos(phi)
    scaled_image = image * cos_phi[:, :, np.newaxis]
    return scaled_image

def compute_monochrome_image(image):
    """
    Compute the monochrome version of the image using the specified formula.
    """
    return 0.2125 * image[:, :, 0] + 0.7154 * image[:, :, 1] + 0.0721 * image[:, :, 2]

def compute_total_energy(image):
    """
    Compute the total energy within the image using a summed area table.
    """
    summed_area_table = np.zeros_like(image)
    summed_area_table[0, 0] = image[0, 0]
    for i in range(1, image.shape[0]):
        summed_area_table[i, 0] = summed_area_table[i - 1, 0] + image[i, 0]
    for j in range(1, image.shape[1]):
        summed_area_table[0, j] = summed_area_table[0, j - 1] + image[0, j]
    for i, j in product(range(1, image.shape[0]), range(1, image.shape[1])):
        summed_area_table[i, j] = (image[i, j] +
                                   summed_area_table[i - 1, j] +
                                   summed_area_table[i, j - 1] -
                                   summed_area_table[i - 1, j - 1])
    return summed_area_table

def partition_image(image, n):
    """
    Partition the image into 2^n regions of similar light energy.
    """
    scaled_image = scale_by_cos_phi(image)
    monochrome_image = compute_monochrome_image(scaled_image)
    total_energy = compute_total_energy(monochrome_image)

    regions = [(0, 0, image.shape[0], image.shape[1])]  # Initialize with the entire image as a region

    for _ in range(n):
        new_regions = []
        for region in regions:
            y, x, height, width = region
            subregions = []
            if height > width:
                # Divide along the vertical axis
                total_vertical_energy = total_energy[y + height - 1, x] - (total_energy[y - 1, x] if y > 0 else 0)
                cumulative_energy = 0
                for i in range(y, y + height):
                    cumulative_energy += total_energy[i, x]
                    if cumulative_energy >= total_vertical_energy / 2:
                        subregions.append((y, x, i - y + 1, width))
                        subregions.append((y + i - y + 1, x, height - (i - y + 1), width))
                        break
            else:
                # Divide along the horizontal axis
                total_horizontal_energy = total_energy[y, x + width - 1] - (total_energy[y, x - 1] if x > 0 else 0)
                cumulative_energy = 0
                for j in range(x, x + width):
                    cumulative_energy += total_energy[y, j]
                    if cumulative_energy >= total_horizontal_energy / 2:
                        subregions.append((y, x, height, j - x + 1))
                        subregions.append((y, x + j - x + 1, height, width - (j - x + 1)))
                        break
            new_regions.extend(subregions)
        regions = new_regions

    return regions
