from median_cut import median_cut
from PIL import Image
import numpy as np
import matplotlib.pyplot as plt

def main():
    # Load an image
    image = np.array(Image.open('elmo.png', 'r').convert('RGB'))

    # Reduce the number of colors in the image to 16
    num_colors = 2
    representative_colors = median_cut(image, num_colors)

    # Create a new image using the representative colors and approximate every pixel
    new_image = np.zeros_like(image)
    for i, color in enumerate(representative_colors):
        new_image[np.all(abs(image - color) < (10,10,10), axis=-1)] = color



    print(representative_colors)

    # Display the original and new images
    plt.figure(figsize=(10, 5))
    plt.subplot(1, 3, 1)
    plt.imshow(image)
    plt.title('Original Image')
    plt.axis('off')

    plt.subplot(1, 3, 2)
    #show reduced colors as a grid of color swatches
    plt.imshow([representative_colors], aspect='auto')
    plt.title('Reduced Color Image')
    plt.axis('off')

    plt.subplot(1, 3, 3)
    plt.imshow(new_image)

    plt.show()

if __name__ == '__main__':
    main()